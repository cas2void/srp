/*******************************************************************************
 * File   : rcmanager_srp.c
 * Content: Rendering context manager
 *
 * Coder  : Ean Jee
 * Time   : 2011-02-14 16:01
 ******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "malloc_ig.h"
#include "rcmanager_srp.h"
#include "datadef_srp.h"
#include "matrix_srp.h"
#include "frustum_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

#define SRP_MAX_MODELVIEW_STACK_DEPTH  32
#define SRP_MAX_PROJECTION_STACK_DEPTH  4
#define SRP_MAX_TEXTURE_STACK_DEPTH     4
#define SRP_MAX_VIEWPORT_STACK_DEPTH    4

struct SRP_TRANSFORM_ATTRIB_T
{
    int matrixMode;
};
typedef struct SRP_TRANSFORM_ATTRIB_T SRP_TRANSFORM_ATTRIB;

struct SRP_OBJECT_ATTRIB_T
{
    int cullFlag;
};
typedef struct SRP_OBJECT_ATTRIB_T SRP_OBJECT_ATTRIB;

struct SRP_POLYGON_ATTRIB_T
{
    int cullFlag;
};
typedef struct SRP_POLYGON_ATTRIB_T SRP_POLYGON_ATTRIB;

struct SRP_RC_T
{
    int width;
    int height;
    int bit;
    int pitch;
    int size;
    
    unsigned char clearRed;
    unsigned char clearGreen;
    unsigned char clearBlue;
    unsigned char clearAlpha;

    unsigned char drawRed;
    unsigned char drawGreen;
    unsigned char drawBlue;
    unsigned char drawAlpha;

    unsigned char *buffer;
    unsigned char *clearBuffer; /* Buffer with the current clearing color,
                                 * used for speeding up SrpRCClear()
                                 */

    MATRIX43F fModelViewStack[SRP_MAX_MODELVIEW_STACK_DEPTH];
    MATRIX43F fProjectionStack[SRP_MAX_PROJECTION_STACK_DEPTH];
    MATRIX43F fTextureStack[SRP_MAX_TEXTURE_STACK_DEPTH];    
    MATRIX43F fViewportStack[SRP_MAX_VIEWPORT_STACK_DEPTH];
    
    int stackPosM;    /* Current position of modelview matrix stack */
    int stackPosP;    /* Current position of projection matrix stack */
    int stackPosT;    /* Current position of texture matrix stack */
    int stackPosV;    /* Current position of viewport matrix stack */

    MATRIX43F *pfCurrentMatrix;  /* Pointer to the current matrix  */
    int *pCurrentStackPos;       /* Pointer to the current stack position */
    int currentStackMaxDepth;    /* Max depth of current stack */
    
    float fFovy;
    float fAspect;

    FRUSTUM *pFrustum;

    SRP_TRANSFORM_ATTRIB transformAttrib;
    SRP_OBJECT_ATTRIB    objectAttrib;
    SRP_POLYGON_ATTRIB   polygonAttrib;
};

typedef struct SRP_RC_T SRP_RC;

/*----------------------------------------------------------------------------*/
/*                                Private Data                                */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * pRC points to SRP's rendering context.
 */
static SRP_RC *sg_pRC = NULL;

/*----------------------------------------------------------------------------*/
/*                       Private Function Declarations                        */
/*----------------------------------------------------------------------------*/

static void SrpRCInitTransform(void);
static void SrpRCInitObject(void);
static void SrpRCInitPolygon(void);

static void SrpRCSetCapability(int cap, int state);

/*----------------------------------------------------------------------------*/
/*                             Private Functions                              */
/*----------------------------------------------------------------------------*/

static void SrpRCInitTransform(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    sg_pRC->transformAttrib.matrixMode = SRP_MODELVIEW;
}

static void SrpRCInitObject(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    sg_pRC->objectAttrib.cullFlag = FALSE;
}

static void SrpRCInitPolygon(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    sg_pRC->polygonAttrib.cullFlag = FALSE;
}

static void SrpRCSetCapability(int cap, int state)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    switch (cap)
    {
    case SRP_CULL_OBJECT:
        sg_pRC->objectAttrib.cullFlag = state;
        break;

    case SRP_CULL_FACE:
        sg_pRC->polygonAttrib.cullFlag = state;
        break;
    default:
        ASSERTMSG(FALSE, "SrpRCEnable: unknown capability.");
        break;
    }
}

/*----------------------------------------------------------------------------*/
/*                              Public Functions                              */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * int SrpCreateRC(int width, int height, int bit)
 *
 * This function create a rendering context with required window size
 * and color bits. It must be called once and once only.
 */
int SrpCreateRC(int width, int height, int bit)
{
    int i;
    
    ASSERTMSG(sg_pRC == NULL, "SrpCreateRC shoud be called only once.");

    if (!IgNewMemory((void **)&sg_pRC, sizeof(SRP_RC)))
    {
        printf("SrpCreateRC failed.\n");
        return FALSE;
    }

    sg_pRC->width  = width;
    sg_pRC->height = height;
    sg_pRC->bit    = bit;
    /* Make the buffer alligned with 4 bytes */
    sg_pRC->pitch  = (width * bit + 31) / 32 * 4;
    sg_pRC->size   = sg_pRC->pitch * sg_pRC->height;

    sg_pRC->clearRed    = 0;
    sg_pRC->clearGreen  = 0;
    sg_pRC->clearBlue   = 0;
    sg_pRC->clearAlpha  = 0;
    
    sg_pRC->drawRed    = 0;
    sg_pRC->drawGreen  = 0;
    sg_pRC->drawBlue   = 0;
    sg_pRC->drawAlpha  = 0;

    if (!IgNewMemory((void **)&(sg_pRC->buffer), sg_pRC->size))
    {
        IgFreeMemory(sg_pRC);
        printf("SrpCreateRC failed.\n");
        return FALSE;
    }
    
    memset(sg_pRC->buffer, 0, sg_pRC->size);

    if (!IgNewMemory((void **)&(sg_pRC->clearBuffer), sg_pRC->size))
    {
        IgFreeMemory(sg_pRC);
        printf("SrpCreateRC failed.\n");
        return FALSE;
    }
    
    memset(sg_pRC->clearBuffer, 0, sg_pRC->size);

    for (i = 0; i < SRP_MAX_MODELVIEW_STACK_DEPTH; i++)
    {
        SrpMatrixLoadIdentity43f(sg_pRC->fModelViewStack[i]);
    }
    for (i = 0; i < SRP_MAX_PROJECTION_STACK_DEPTH; i++)
    {
        SrpMatrixLoadIdentity43f(sg_pRC->fProjectionStack[i]);
    }
    for (i = 0; i < SRP_MAX_TEXTURE_STACK_DEPTH; i++)
    {
        SrpMatrixLoadIdentity43f(sg_pRC->fTextureStack[i]);
    }
    for (i = 0; i < SRP_MAX_VIEWPORT_STACK_DEPTH; i++)
    {
        SrpMatrixLoadIdentity43f(sg_pRC->fViewportStack[i]);
    }
    
    sg_pRC->stackPosM = 0;
    sg_pRC->stackPosP = 0;
    sg_pRC->stackPosT = 0;
    sg_pRC->stackPosV = 0;
    
    sg_pRC->pfCurrentMatrix = sg_pRC->fModelViewStack;
    sg_pRC->pCurrentStackPos = &sg_pRC->stackPosM;
    sg_pRC->currentStackMaxDepth = SRP_MAX_MODELVIEW_STACK_DEPTH;

    sg_pRC->fFovy   = 90.0f;
    sg_pRC->fAspect = 1.0f;

    SrpCreateFrustum(&sg_pRC->pFrustum, sg_pRC->fFovy, sg_pRC->fAspect, 
                     -2.0f, -100.0f);

    SrpRCInitTransform();
    SrpRCInitObject();
    SrpRCInitPolygon();

    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpDeleteRC(void)
 * 
 * This function delete the rendering context. It should be called after
 * a rendering context has been created.
 */
void SrpDeleteRC(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    SrpDeleteFrustum(sg_pRC->pFrustum);
    IgFreeMemory(sg_pRC->buffer);
    IgFreeMemory(sg_pRC->clearBuffer);
    IgFreeMemory(sg_pRC);
}

/*------------------------------------------------------------------------------
 * int SrpRCGetWidth()
 * int SrpRCGetHeight()
 * int SrpRCGetBit(void)
 * int SrpRCGetPitch(void)
 * int SrpRCGetSize(void)
 * const unsigned char* SrpRCGetBuffer(void)
 * float SrpRCGetFovy(void)
 * float SrpRCGetAspect(void)
 * MATRIX43F* SrpRCGetModelView(void)
 *
 * void SrpRCSetWidth(int width)
 * void SrpRCSetHeight(int height)
 * void SrpRCSetFrustum(float fovy, float aspect, float near, float far)
 *
 * RC gets and sets.
 */
int SrpRCGetWidth(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->width;
}

int SrpRCGetHeight(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->height;
}

int SrpRCGetBit(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->bit;
}

int SrpRCGetPitch(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->pitch;
}

int SrpRCGetSize(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->size;
}

const unsigned char* SrpRCGetBuffer(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->buffer;
}

float SrpRCGetFovy(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->fFovy;
}

float SrpRCGetAspect(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return sg_pRC->fAspect;
}

MATRIX43F* SrpRCGetModelView(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    return (sg_pRC->fModelViewStack + sg_pRC->stackPosM);
}

void SrpRCSetWidth(int width)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    sg_pRC->width = width;
}

void SrpRCSetHeight(int height)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    sg_pRC->height = height;
}

void SrpRCSetFrustum(float fovy, float aspect, float near, float far)
{
    ASSERTMSG(fovy > 0.0f && fovy < 180.0f &&
              aspect > 0.0f && near < -1.0f && far < near,
              "SrpRCSetFrustum: invalid arguments.");
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    sg_pRC->fFovy = fovy;
    sg_pRC->fAspect = aspect;
    SrpSetFrustum(sg_pRC->pFrustum, fovy, aspect, near, far);
}

void SrpRCPrintMatrix(void)
{
    char *name;

    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    switch (sg_pRC->transformAttrib.matrixMode)
    {
    case SRP_MODELVIEW:
        name = "modelview";
        break;
    case SRP_PROJECTION:
        name = "projection";
        break;
    case SRP_TEXTURE:
        name = "texture";
        break;
    case SRP_VIEWPORT:
        name = "viewport";
        break;
    default:
        ASSERTMSG(FALSE, "SrpRCPrintMatrix: invalid matrix mode.");
        break;
    }

    printf("currentStackPos = %d\n", *sg_pRC->pCurrentStackPos);
    SrpMatrixPrint43f(*sg_pRC->pfCurrentMatrix, name);
}

void SrpRCPrintStack(int depth)
{
    int i;
    MATRIX43F *stack;

    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    switch (sg_pRC->transformAttrib.matrixMode)
    {
    case SRP_MODELVIEW:
        stack = sg_pRC->fModelViewStack;
        break;
    case SRP_PROJECTION:
        stack = sg_pRC->fProjectionStack;
        break;
    case SRP_TEXTURE:
        stack = sg_pRC->fTextureStack;
        break;
    case SRP_VIEWPORT:
        stack = sg_pRC->fViewportStack;
        break;
    default:
        ASSERTMSG(FALSE, "SrpRCPrintStack: invalid matrix mode.");
        break;
    }

    for (i = 0; i < depth; i++)
    {
        SrpMatrixPrint43f(stack[i], "matrix");
    }
}

/*------------------------------------------------------------------------------
 * void SrpRCSetClearColor(int red, int green, int blue, int alpha)
 *
 * This function explicitly set the current clearing color in rendering context
 * with the given rgba.
 */
void SrpRCSetClearColor(int red, int green, int blue, int alpha)
{
    int i;

    ASSERTMSG(sg_pRC != NULL && red >= 0 && red <= 255 && 
        green >= 0 && green <= 255 && blue >= 0 && blue <= 255 && 
        alpha >= 0 && alpha <= 255, "SrpRCSetClearColor: invalid arguments.");

    switch (sg_pRC->bit)
    {
    case 32:
        sg_pRC->clearRed   = red;
        sg_pRC->clearGreen = green;
        sg_pRC->clearBlue  = blue;
        sg_pRC->clearAlpha = alpha;

        for (i = 0; i < sg_pRC->size; i += 4)
        {
            sg_pRC->clearBuffer[i + 0] = sg_pRC->clearBlue;
            sg_pRC->clearBuffer[i + 1] = sg_pRC->clearGreen;
            sg_pRC->clearBuffer[i + 2] = sg_pRC->clearRed;
            sg_pRC->clearBuffer[i + 3] = sg_pRC->clearAlpha;
        }
        break;
    default:
        ASSERTMSG(FALSE, 
                  "SrpRCSetClearColor: current color bit is not supported.");
        break;
    }
}

/*------------------------------------------------------------------------------
 * void SrpRCSetDrawColor(int red, int green, int blue, int alpha)
 *
 * This function explicitly set the current drawing color in rendering context
 * with the given rgba.
 */
void SrpRCSetDrawColor(int red, int green, int blue, int alpha)
{
    ASSERTMSG(sg_pRC != NULL && red >= 0 && red <= 255 && 
        green >= 0 && green <= 255 && blue >= 0 && blue <= 255 && 
        alpha >= 0 && alpha <= 255, "SrpRCSetDrawColor: invalid arguments.");

    switch (sg_pRC->bit)
    {
    case 32:
        sg_pRC->drawRed   = red;
        sg_pRC->drawGreen = green;
        sg_pRC->drawBlue  = blue;
        sg_pRC->drawAlpha = alpha;
        break;
    default:
        ASSERTMSG(FALSE, 
                  "SrpRCSetDrawColor: current color bit is not supported.");
        break;
    }
}

/*------------------------------------------------------------------------------
 * void SrpRCSetPixel(size_t offset)
 *
 * This function sets the color at (sg_pRC->buffer + offset), using
 * different approach according to current color bit.
 */
void SrpRCSetPixel(size_t offset)
{
    ASSERTMSG(sg_pRC != NULL && offset >= 0 && offset < sg_pRC->size, 
        "SrpRCSetPixel: invalid arguments.");
    
    switch (sg_pRC->bit)
    {
    case 32:
        sg_pRC->buffer[offset + 0] = sg_pRC->drawBlue;
        sg_pRC->buffer[offset + 1] = sg_pRC->drawGreen;
        sg_pRC->buffer[offset + 2] = sg_pRC->drawRed;
        sg_pRC->buffer[offset + 3] = sg_pRC->drawAlpha;
        break;
    default:
        ASSERTMSG(FALSE, "SrpRCSetPixel: current color bit is not supported.");
        break;
    }
}

/*------------------------------------------------------------------------------
 * void SrpRCClear(void)
 *
 * This function clears The Buffer With current clearing color.
 */
void SrpRCClear(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    memcpy(sg_pRC->buffer, sg_pRC->clearBuffer, sg_pRC->size);
}

/*------------------------------------------------------------------------------
 * void SrpRCCombineMatrix43f(const MATRIX43F m)
 *
 * Multiply current matrix with the incoming matrix.
 */
void SrpRCCombineMatrix43f(const MATRIX43F m)
{
    MATRIX43F temp;

    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    SrpMatrixMultiply43f(temp, m, *sg_pRC->pfCurrentMatrix);
    SrpMatrixCopy43f(*sg_pRC->pfCurrentMatrix, temp);
}

/*------------------------------------------------------------------------------
 * void SrpRCSetMatrixMode(int mode)
 *
 * Specify which matrix is the current matrix, updating pointer to
 * current matrix, pointer to current stack position, max depth of
 * current stack.
 */
void SrpRCSetMatrixMode(int mode)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    switch (mode)
    {
    case SRP_MODELVIEW:
        sg_pRC->pfCurrentMatrix = &sg_pRC->fModelViewStack[sg_pRC->stackPosM];
        sg_pRC->pCurrentStackPos = &sg_pRC->stackPosM;
        sg_pRC->currentStackMaxDepth = SRP_MAX_MODELVIEW_STACK_DEPTH;
        break;
    case SRP_PROJECTION:
        sg_pRC->pfCurrentMatrix = &sg_pRC->fProjectionStack[sg_pRC->stackPosP];
        sg_pRC->pCurrentStackPos = &sg_pRC->stackPosP;
        sg_pRC->currentStackMaxDepth = SRP_MAX_PROJECTION_STACK_DEPTH;
        break;
    case SRP_TEXTURE:
        sg_pRC->pfCurrentMatrix = &sg_pRC->fTextureStack[sg_pRC->stackPosT];
        sg_pRC->pCurrentStackPos = &sg_pRC->stackPosT;
        sg_pRC->currentStackMaxDepth = SRP_MAX_TEXTURE_STACK_DEPTH;
        break;
    case SRP_VIEWPORT:
        sg_pRC->pfCurrentMatrix = &sg_pRC->fViewportStack[sg_pRC->stackPosV];
        sg_pRC->pCurrentStackPos = &sg_pRC->stackPosV;
        sg_pRC->currentStackMaxDepth = SRP_MAX_VIEWPORT_STACK_DEPTH;
        break;
    default:
        ASSERTMSG(FALSE, "SrpRCSetMatrixMode: invalid matrix mode.");
        break;
    }

    sg_pRC->transformAttrib.matrixMode = mode;
}

/*------------------------------------------------------------------------------
 * void SrpRCPushMatrix(void)
 *
 * Push the current matrix stack down by one, duplicating 
 * the current matrix.
 */
void SrpRCPushMatrix(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    ASSERTMSG(*sg_pRC->pCurrentStackPos < sg_pRC->currentStackMaxDepth - 1, 
              "SrpRCPushMatrix: current matrix stack is full.");

    if (*sg_pRC->pCurrentStackPos < sg_pRC->currentStackMaxDepth - 1)
    {
        /* Duplicate the current matrix */
        SrpMatrixCopy43f(*(sg_pRC->pfCurrentMatrix + 1), 
                         *sg_pRC->pfCurrentMatrix);

        /* Push the current matrix stack down by one */
        (*sg_pRC->pCurrentStackPos)++;
        (sg_pRC->pfCurrentMatrix)++;
    }
}

/*------------------------------------------------------------------------------
 * void SrpRCPopMatrix(void)
 *
 * Pops the current matrix stack, replacing the current matrix 
 * with the one below it on the stack.
 */
void SrpRCPopMatrix(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    ASSERTMSG(*sg_pRC->pCurrentStackPos > 0,
            "SrpRCPopMatrix: current matrix stack is empty.");

    if (*sg_pRC->pCurrentStackPos > 0)
    {
        (*sg_pRC->pCurrentStackPos)--;
        (sg_pRC->pfCurrentMatrix)--;
    }
}

/*------------------------------------------------------------------------------
 * void SrpRCLoadIdentity(void)
 *
 * Load the current matrix with identity matrix.
 */
void SrpRCLoadIdentity(void)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    SrpMatrixLoadIdentity43f(*sg_pRC->pfCurrentMatrix);
}

/*------------------------------------------------------------------------------
 * int SrpRCIsVisible(const VECTOR3F pos, float radius)
 *
 * Check if an object is visible with current frustum in RC.
 * The object is define by its postion and radius.
 * 
 * Return:
 *     TRUE if the object is partly or completely visible.
 *     FALSE if the object is completely invisible.
 */
int SrpRCIsVisible(const VECTOR3F pos, float radius)
{
    ASSERTMSG(sg_pRC != NULL && sg_pRC->pFrustum != NULL, 
              "Rendering context has not been initialized.");
    ASSERTMSG(radius > 0.0f, "SrpRCIsVisible: invalid argument.");

    return SrpIsVisibleInFrustum(sg_pRC->pFrustum, pos, radius);
}

/*------------------------------------------------------------------------------
 * void SrpRCEnable(int cap)
 *
 * Enable a specific capability.
 */
void SrpRCEnable(int cap)
{
    SrpRCSetCapability(cap, TRUE);
}

/*------------------------------------------------------------------------------
 * void SrpRCDisable(int cap)
 *
 * Disable a specific capability.
 */
void SrpRCDisable(int cap)
{
    SrpRCSetCapability(cap, FALSE);
}

/*------------------------------------------------------------------------------
 * int SrpRCIsEnabled(int cap)
 *
 * Check if a required capability is enabled.
 */
int SrpRCIsEnabled(int cap)
{
    ASSERTMSG(sg_pRC != NULL, "Rendering context has not been initialized.");

    switch (cap)
    {
    case SRP_CULL_OBJECT:
        return sg_pRC->objectAttrib.cullFlag;

    case SRP_CULL_FACE:
        return sg_pRC->polygonAttrib.cullFlag;

    default:
        ASSERTMSG(FALSE, "SrpRCIsEnabled: unknown capability.");
        return FALSE;
    }
}
