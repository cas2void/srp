/*******************************************************************************
 * File   : model_srp.c
 * Content: Model manipulation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-10-03 17:57
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "malloc_ig.h"
#include "datadef_srp.h"
#include "math_srp.h"
#include "model_srp.h"
#include "vector_srp.h"
#include "matrix_srp.h"
#include "rcmanager_srp.h"
#include "renderee_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

#define TRIANGLE_STATE_ACTIVE     0x00000000
#define TRIANGLE_STATE_CLIPPED    0x00000001
#define TRIANGLE_STATE_BACKFACE   0x00000002

#define OBJECT_STATE_ACTIVE       0x00000000
#define OBJECT_STATE_CULLED       0x00000001

/*
 * A triangle based on an external vertex list
 */
struct TRIANGLE_T
{
    int state;
    int attr;
    
    VECTOR3F *pList;
    int index[3]; /* indies into the vertex list */
};

typedef struct TRIANGLE_T TRIANGLE;

/*
 * A model(triangle mesh) based on a vertex list and a list of triangles
 */
struct MODEL_T
{
    char name[32];

    float radius;

    int numVertices;
    VECTOR3F *pOldList;
    VECTOR3F *pNewList;

    int numTriangles;
    TRIANGLE *pTriList;
};

/*
 * An object based on an external model data
 */
struct OBJECT_T
{
    int id;
    char name[32];
    int state;

    float radius;

    VECTOR3F pos;         /* Position in world space */
    VECTOR3F dir;         /* Orientation in world space, measured in degree */
    VECTOR3F sca;         /* Scale factor */

    VECTOR3F translation; /* Traslation vector */
    MATRIX43F rotation;   /* Rotation matrix derived from (dir + deltaDir) */
    VECTOR3F scale;       /* scale vector */

    MODEL *pModel;
};

/*----------------------------------------------------------------------------*/
/*                                Private Data                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                       Private Function Declarations                        */
/*----------------------------------------------------------------------------*/

static int SrpGetLine(char *buffer, int maxLength, FILE *fp);
static void SrpCalculateModelRadius(MODEL *pModel);
static void SrpResetObjectState(OBJECT *pObj);
static void SrpCullObject(OBJECT *pObj);
static void SrpTransObjectLocToCam(OBJECT *obj);
static void SrpCullBackFace(OBJECT *pObj);
static void SrpInsertObjectToRenderList(const OBJECT *obj, RENDER_LIST *pRl);

/*----------------------------------------------------------------------------*/
/*                             Private Functions                              */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * int SrpGetLine(char *buffer, int maxLength, FILE *fp)
 *
 * Get a line from a file.
 *
 * Return:
 *     TRUE : something is filled into buffer.
 *     FALSE: the file is empty.
 */
static int SrpGetLine(char *buffer, int maxLength, FILE *fp)
{
    int i, length;
    char tempBuf[maxLength];

    ASSERTMSG(buffer != NULL && fp != NULL, 
              "SrpGetLine: invalid arguments.");

    while (TRUE)
    {
        if (!fgets(tempBuf, maxLength, fp))
        {
            return FALSE;
        }

        length = strlen(tempBuf);
        /* Get rid of space */
        for (i = 0; i < length; i++)
        {
            if (!isspace(tempBuf[i]))
            {
                break;
            }
        }

        /* Discard blank lines and comments */
        if (i >= length  || tempBuf[i] == '#')
        {
            continue;
        }

        /* Now everything is fine. */
        strcpy(buffer, tempBuf);
        return TRUE;
    }
}

/*------------------------------------------------------------------------------
 * void SrpCalculateModelRadius(MODEL *pModel)
 *
 * Calculate a model's radius.
 */
static void SrpCalculateModelRadius(MODEL *pModel)
{
    int i;
    float maxRadiusSquared, curRadiusSquared;

    ASSERTMSG(pModel != NULL, "SrpCalculateModelRadius: invalid argument.");
    ASSERTMSG(pModel->pNewList != NULL, 
              "SrpCalculateModelRadius: invalid new vertex list.");

    maxRadiusSquared = 0.0f;
    for (i = 0; i < pModel->numVertices; i++)
    {
        curRadiusSquared = SrpVectorLengthSquared3f(pModel->pNewList[i]);
        if (maxRadiusSquared < curRadiusSquared)
        {
            maxRadiusSquared = curRadiusSquared;
        }
    }

    pModel->radius = sqrt(maxRadiusSquared);
}

/*------------------------------------------------------------------------------
 * void SrpResetObject(OBJECT *pObj)
 *
 * Reset object.
 */
static void SrpResetObjectState(OBJECT *pObj)
{
    int i;
    MODEL *pModel;

    ASSERTMSG(pObj != NULL, "SrpResetObjectState: invalid argument.");

    pModel = pObj->pModel;
    ASSERTMSG(pModel->pOldList != NULL, 
              "SrpResetObjectState: invalid old vertex list.");
    ASSERTMSG(pModel->pNewList != NULL, 
              "SrpResetObjectState: invalid new vertex list.");
    ASSERTMSG(pModel->pTriList != NULL, 
              "SrpResetObjectState: invalid triangle list.");

    CLEAN_BIT(pObj->state);
    for (i = 0; i < pModel->numTriangles; i++)
    {
        CLEAN_BIT(pModel->pTriList[i].state);
    }
}

/*------------------------------------------------------------------------------
 * void SrpCullObject(OBJECT *pObj)
 *
 * Object culling.
 */
static void SrpCullObject(OBJECT *pObj)
{
    VECTOR3F posT;
    MATRIX43F *pModelView;

    ASSERTMSG(pObj != NULL, "SrpCullObject: invalid argument.");

    /* Transform object's postion into camera space */
    pModelView = SrpRCGetModelView();
    SrpMatrixTransformVector3f(posT, pObj->pos, *pModelView);

    if (!SrpRCIsVisible(posT, pObj->radius))
    {
        SET_BIT(pObj->state, OBJECT_STATE_CULLED);
    }
}

/*------------------------------------------------------------------------------
 * void SrpTransObjectLocToCam(OBJECT *pObj)
 *
 * Transfrom object from local space to camera space.
 */
static void SrpTransObjectLocToCam(OBJECT *pObj)
{
    int i;
    MODEL *pModel;
    MATRIX43F *pModelView;
    VECTOR3F scaledVec, rotatedVec, translatedVec, transformedVec;

    ASSERTMSG(pObj != NULL, 
              "SrpTransObjectLocToCam: invalid arguments.");

    pModel = pObj->pModel;
    ASSERTMSG(pModel->pOldList != NULL, 
              "SrpTransObjectLocToCam: invalid old vertex list.");
    ASSERTMSG(pModel->pNewList != NULL, 
              "SrpTransObjectLocToCam: invalid new vertex list.");

    /* Discard this object if it's been culled */
    if (pObj->state & OBJECT_STATE_CULLED)
    {
        return;
    }

    pModelView = SrpRCGetModelView();
    for (i = 0; i < pModel->numVertices; i++)
    {
        /* Scale it */
        SrpVectorScaleIndie3f(scaledVec, pModel->pOldList[i], pObj->scale);

        /* Rotate it */
        SrpMatrixTransformVector3f(rotatedVec, scaledVec, pObj->rotation);

        /* Translate it */
        SrpVectorAdd3f(translatedVec, rotatedVec, pObj->translation);

        /* Transform it with current modelview matrix */
        SrpMatrixTransformVector3f(transformedVec, translatedVec, *pModelView);

        /* Store it */
        SrpVectorCopy3f(pModel->pNewList[i], transformedVec);
    }
}

/*------------------------------------------------------------------------------
 * void SrpCullBackFace(OBJECT *pObj)
 *
 * Back face removing.
 */
static void SrpCullBackFace(OBJECT *pObj)
{
    int i, verIndex0, verIndex1, verIndex2;
    MODEL *pModel;
    TRIANGLE *pTri;
    VECTOR3F normal, view;
    VECTOR3F *pVerList;

    ASSERTMSG(pObj != NULL, "SrpCullBackFace: invalid argument.");

    pModel = pObj->pModel;
    ASSERTMSG(pModel->pOldList != NULL, 
              "SrpCullBackFace: invalid old vertex list.");
    ASSERTMSG(pModel->pNewList != NULL, 
              "SrpCullBackFace: invalid new vertex list.");
    ASSERTMSG(pModel->pTriList != NULL, 
              "SrpCullBackFace: invalid triangle list.");

    /* Discard this object if it's been culled */
    if (pObj->state & OBJECT_STATE_CULLED)
    {
        return;
    }

    pVerList = pModel->pNewList;
    for (i = 0; i < pModel->numTriangles; i++)
    {
        pTri = &pModel->pTriList[i];

        /* Don't repeat yourself. */
        if (pTri->state & TRIANGLE_STATE_CLIPPED || 
            pTri->state & TRIANGLE_STATE_BACKFACE)
        {
            continue;
        }

        verIndex0 = pTri->index[0];
        verIndex1 = pTri->index[1];
        verIndex2 = pTri->index[2];

        SrpVectorGetPlaneNormal3f(normal, pVerList[verIndex0], 
                                  pVerList[verIndex1], pVerList[verIndex2]);

        /* Set the triangle's state backface if the dot product of
         * its normal and view vector is negative.
         */
        SrpVectorSubtract3f(view, ZERO_VECTOR, pVerList[verIndex0]);
        if (SrpVectorDotProduct3f(normal, view) <= 0.0f)
        {
            SET_BIT(pTri->state, TRIANGLE_STATE_BACKFACE);
        }
    }
}

/*------------------------------------------------------------------------------
 * void SrpInsertObjectToRenderList(const OBJECT *pObj, RENDER_LIST *pRl)
 *
 * Break object into triangles and insert them as self-contained
 * triangles into render list.
 */
static void SrpInsertObjectToRenderList(const OBJECT *pObj, RENDER_LIST *pRl)
{
    int i, verIndex0, verIndex1, verIndex2;
    MODEL *pModel;
    TRIANGLE *pTri;
    TRIANGLE_INDIE *pTriIndie;
    VECTOR3F *pVerList;

    ASSERTMSG(pObj != NULL && pRl != NULL, 
              "SrpInsertObjectToRenderList: invalid argument.");

    pModel = pObj->pModel;
    ASSERTMSG(pModel->pOldList != NULL, 
              "SrpInsertObjectToRenderList: invalid old vertex list.");
    ASSERTMSG(pModel->pNewList != NULL, 
              "SrpInsertObjectToRenderList: invalid new vertex list.");
    ASSERTMSG(pModel->pTriList != NULL, 
              "SrpInsertObjectToRenderList: invalid triangle list.");
    
    /* Discard this object if it's been culled */
    if (pObj->state & OBJECT_STATE_CULLED)
    {
        return;
    }

    if (!SrpCreateTriIndie(&pTriIndie, ZERO_VECTOR, ZERO_VECTOR, ZERO_VECTOR))
    {
        printf("Error: SrpInsertObjectToRenderList create TRIANGLE_INDIE \
failed.\n");
        return;
    }

    pVerList = pModel->pNewList;
    for (i = 0; i < pModel->numTriangles; i++)
    {
        pTri = &pModel->pTriList[i];
        /* Discard this triangle if it's clipped or backfaced */
        if (pTri->state & TRIANGLE_STATE_CLIPPED || 
            pTri->state & TRIANGLE_STATE_BACKFACE)
        {
            continue;
        }

        /* Initialize a self-contained triangle from
         * the object's building triangle.
         */
        SrpSetTriIndieAttr(pTriIndie, pTri->attr);

        /* pTri->pList and pModel->pNewList should point to
         * the same address even if pTri->pList has not
         * been initialized.
         */
        verIndex0 = pTri->index[0];
        verIndex1 = pTri->index[1];
        verIndex2 = pTri->index[2];
        SrpSetTriIndieVertex(pTriIndie, pVerList[verIndex0], 
                            pVerList[verIndex1], pVerList[verIndex2]);

        /* Insert the self-contained triangle into render list */
        SrpInsertTriangleToRenderList(pTriIndie, pRl);
    }

    SrpDeleteTriIndie(pTriIndie);
}

/*----------------------------------------------------------------------------*/
/*                              Public Functions                              */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * int SrpCreateObject(OBJECT **ppObj, MODEL *pModel, 
 *                     float positionX, float positionY, float positionZ, 
 *                     float directionX, float directionY, float directionZ, 
 *                     float scaleX, float scaleY, float scaleZ)
 *
 * Create object.
 */
int SrpCreateObject(OBJECT **ppObj, MODEL *pModel, 
                    float positionX, float positionY, float positionZ, 
                    float directionX, float directionY, float directionZ, 
                    float scaleX, float scaleY, float scaleZ)
{
    OBJECT *pObj;
    float maxScale;
    MATRIX43F rotMat, tempMat;

    ASSERTMSG(ppObj != NULL && pModel != NULL, 
              "SrpCreateObject: invalid arguments.");

    if (!IgNewMemory((void **)ppObj, sizeof(OBJECT)))
    {
        printf("Error: create object failed.\n");
        return FALSE;
    }

    pObj = *ppObj;
    pObj->pModel = pModel;
    strcpy(pObj->name, pModel->name);
    pObj->state = OBJECT_STATE_ACTIVE;

    /* Take the largest scale factor and use it to scale the radius */
    maxScale = SrpMathMax(scaleX, scaleY);
    maxScale = SrpMathMax(maxScale, scaleZ);
    pObj->radius = pModel->radius * maxScale;

    SrpVectorLoad3f(pObj->pos, positionX, positionY, positionZ);
    SrpVectorLoad3f(pObj->dir, directionX, directionY, directionZ);
    SrpVectorLoad3f(pObj->sca, scaleX, scaleY, scaleZ);

    SrpVectorCopy3f(pObj->translation, pObj->pos);
    SrpVectorCopy3f(pObj->scale, pObj->sca);

    /* Make the rotation matrix, with YXZ sequence */
    SrpMatrixLoadIdentity43f(pObj->rotation);
    if (!SrpMathFloatIsZero(directionY))
    {
        SrpMatrixMakeRotation43f(rotMat, CARDINAL_Y, 
                                 SrpMathDegToRadf(directionY));
        SrpMatrixMultiply43f(tempMat, rotMat, pObj->rotation);
        SrpMatrixCopy43f(pObj->rotation, tempMat);
    }
    if (!SrpMathFloatIsZero(directionX))
    {
        SrpMatrixMakeRotation43f(rotMat, CARDINAL_X, 
                                 SrpMathDegToRadf(directionX));
        SrpMatrixMultiply43f(tempMat, rotMat, pObj->rotation);
        SrpMatrixCopy43f(pObj->rotation, tempMat);
    }
    if (!SrpMathFloatIsZero(directionZ))
    {
        SrpMatrixMakeRotation43f(rotMat, CARDINAL_Z, 
                                 SrpMathDegToRadf(directionZ));
        SrpMatrixMultiply43f(tempMat, rotMat, pObj->rotation);
        SrpMatrixCopy43f(pObj->rotation, tempMat);
    }

    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpDeleteObject(OBJECT *pObj)
 *
 * Delete object.
 */
void SrpDeleteObject(OBJECT *pObj)
{
    ASSERTMSG(pObj != NULL, "SrpDeleteObject: invalid arguments.");

    IgFreeMemory(pObj);
}

/*------------------------------------------------------------------------------
 * void SrpTranslateObject(OBJECT *pObj, float x, float y, float z)
 *
 * Translate Object.
 * x, y and z are translation on Cardinal X, Y and Z respectively.
 */
void SrpTranslateObject(OBJECT *pObj, float x, float y, float z)
{
    VECTOR3F translation;

    ASSERTMSG(pObj != NULL, "SrpTranslateObject: invalid arguments.");

    SrpVectorLoad3f(translation, x, y, z);
    SrpVectorAdd3f(pObj->translation, pObj->pos, translation);
}

/*------------------------------------------------------------------------------
 * void SrpRotateObject(OBJECT *pObj, float x, float y, float z)
 *
 * Rotate Object.
 * x, y and z are rotation on Cardinal X, Y and Z respectively.
 */
void SrpRotateObject(OBJECT *pObj, float x, float y, float z)
{
    MATRIX43F rotMat, tempMat;
    VECTOR3F deltaDir;    /* vector(x, y, z) */
    VECTOR3F rotation;    /* pObj->dir + deltaDir */

    ASSERTMSG(pObj != NULL, "SrpRotateObject: invalid arguments.");

    SrpVectorLoad3f(deltaDir, x, y, z);

    /* Make the rotation matrix, with YXZ sequence */
    if (!SrpVectorIsZero3f(deltaDir))
    {
        SrpVectorAdd3f(rotation, pObj->dir, deltaDir);

        SrpMatrixLoadIdentity43f(pObj->rotation);
        if (!SrpMathFloatIsZero(deltaDir[1]))
        {
            SrpMatrixMakeRotation43f(rotMat, CARDINAL_Y, 
                                     SrpMathDegToRadf(rotation[1]));
            SrpMatrixMultiply43f(tempMat, rotMat, pObj->rotation);
            SrpMatrixCopy43f(pObj->rotation, tempMat);
        }
        if (!SrpMathFloatIsZero(deltaDir[0]))
        {
            SrpMatrixMakeRotation43f(rotMat, CARDINAL_X, 
                                     SrpMathDegToRadf(rotation[0]));
            SrpMatrixMultiply43f(tempMat, rotMat, pObj->rotation);
            SrpMatrixCopy43f(pObj->rotation, tempMat);
        }
        if (!SrpMathFloatIsZero(deltaDir[2]))
        {
            SrpMatrixMakeRotation43f(rotMat, CARDINAL_Z, 
                                     SrpMathDegToRadf(rotation[2]));
            SrpMatrixMultiply43f(tempMat, rotMat, pObj->rotation);
            SrpMatrixCopy43f(pObj->rotation, tempMat);
        }
    }
}

/*------------------------------------------------------------------------------
 * void SrpScaleObject(OBJECT *pObj, float x, float y, float z)
 *
 * Scale Object.
 * x, y and z are scale on Cardinal X, Y and Z respectively.
 */
void SrpScaleObject(OBJECT *pObj, float x, float y, float z)
{
    VECTOR3F scale;

    ASSERTMSG(pObj != NULL, "SrpScaleObject: invalid arguments.");

    SrpVectorLoad3f(scale, x, y, z);
    SrpVectorScaleIndie3f(pObj->scale, pObj->sca, scale);
}

/*------------------------------------------------------------------------------
 * void SrpDrawObject(OBJECT *pObj)
 *
 * Draw Object.
 */
void SrpDrawObject(OBJECT *pObj, RENDER_LIST *pRl)
{
    ASSERTMSG(pObj != NULL && pRl != NULL, "SrpDrawObject: invalid arguments.");

    SrpResetObjectState(pObj);

    if (SrpRCIsEnabled(SRP_CULL_OBJECT))
    {
        SrpCullObject(pObj);
    }

    SrpTransObjectLocToCam(pObj);

    if (SrpRCIsEnabled(SRP_CULL_FACE))
    {
        SrpCullBackFace(pObj);
    }

    SrpInsertObjectToRenderList(pObj, pRl);
}

/*------------------------------------------------------------------------------
 * void SrpPrintObject(const OBJECT *pObj)
 *
 * Print the object.
 */
void SrpPrintObject(const OBJECT *pObj)
{
    int i, j, index;
    TRIANGLE *pTri;
    MODEL *pModel;

    ASSERTMSG(pObj != NULL, "SrpPrintObject: invalid argument.");

    pModel = pObj->pModel;
    ASSERTMSG(pModel->pTriList != NULL, 
              "SrpPrintObject: invalid triangle list.");

    printf("Object %s, state: %d, radius = %f\n", pObj->name, pObj->state, 
           pObj->radius);
    SrpVectorPrint3f(pObj->pos, "Position");
    SrpVectorPrint3f(pObj->dir, "Direction");
    SrpVectorPrint3f(pObj->sca, "Scale");
    SrpMatrixPrint43f(pObj->rotation, "Rotation");

    printf("\n\tNumber of Triangles: %d\n", pModel->numTriangles);
    for (i = 0; i < pModel->numTriangles; i++)
    {
        pTri = &pModel->pTriList[i];
        printf("\n\tTriangle %d, state = %d:\n", i, pTri->state);
        for (j = 0; j < 3; j++)
        {
            index = pTri->index[j];
            SrpVectorPrint3f(pModel->pNewList[index], "vertex");
        }
    }
}

/*------------------------------------------------------------------------------
 * int SrpModelLoadPLG(MODEL **ppModel, const char *fileName)
 *
 * Load PLG file.
 */
int SrpModelLoadPLG(MODEL **ppModel, const char *fileName)
{
    FILE *fp;
    char buffer[256]; /* Working buffer */
    char triDesp[16]; /* Discarded */
    int polyNumVerts; /* Discarded, should be always 3 */
    int i, sizeVertex, sizeTriangle;
    TRIANGLE *pTri;
    MODEL *pModel;

    ASSERTMSG(ppModel != NULL && fileName != NULL, 
              "SrpModelLoadPLG: invalid arguments.");

    if (!(fp = fopen(fileName, "r")))
    {
        printf("Error: can't load file \"%s\"\n", fileName);
        return FALSE;
    }

    if (!IgNewMemory((void **)ppModel, sizeof(MODEL)))
    {
        printf("Error: create model failed.\n");
        return FALSE;
    }
    pModel = *ppModel;

    /* Read in the model info */
    if (!SrpGetLine(buffer, 255, fp))
    {
        printf("Error: invalid model descriptor in file \"%s\".\n", fileName);
        return FALSE;
    }
    
    sscanf(buffer, "%s %d %d", pModel->name, &pModel->numVertices, 
           &pModel->numTriangles);

    /* Allocate vertices and triangles space */
    sizeVertex = pModel->numVertices * sizeof(VECTOR3F);
    if (!IgNewMemory((void **)&pModel->pOldList, sizeVertex))
    {
        IgFreeMemory(pModel);
        printf("Error: vertex list malloc failed in loading file.\n");
        return FALSE;
    }

    if (!IgNewMemory((void **)&(pModel->pNewList), sizeVertex))
    {
        IgFreeMemory(pModel->pOldList);
        IgFreeMemory(pModel);
        printf("Error: vertex list malloc failed in loading file.\n");
        return FALSE;
    }

    sizeTriangle = pModel->numTriangles * sizeof(TRIANGLE);
    if (!IgNewMemory((void **)&pModel->pTriList, sizeTriangle))
    {
        IgFreeMemory(pModel->pOldList);
        IgFreeMemory(pModel->pNewList);
        IgFreeMemory(pModel);
        printf("Error: triangle list malloc failed in loading file.\n");
        return FALSE;
    }

    /* Read in the vertex list */
    for (i = 0; i < pModel->numVertices; i++)
    {
        if (!SrpGetLine(buffer, 255, fp))
        {
            printf("Error: invalid vertex list in file \"%s\".\n", fileName);
            return FALSE;
        }

        sscanf(buffer, "%f %f %f", &pModel->pOldList[i][0], 
               &pModel->pOldList[i][1], &pModel->pOldList[i][2]);
    }

    /* Replicate the vertex data */
    memcpy(pModel->pNewList, pModel->pOldList, sizeVertex);

    /* Calculate the radius */
    SrpCalculateModelRadius(pModel);

    /* Read in the triangle list */
    for (i = 0; i < pModel->numTriangles; i++)
    {
        if (!SrpGetLine(buffer, 255, fp))
        {
            printf("Error: invalid triangle list in file \"%s\".\n", fileName);
            return FALSE;
        }

        pTri = &pModel->pTriList[i];
        sscanf(buffer, "%s %d %d %d %d", triDesp, &polyNumVerts, 
               &pTri->index[0], &pTri->index[1], &pTri->index[2]);
        pTri->state = TRIANGLE_STATE_ACTIVE;
    }

    fclose(fp);
    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpModelRelease(MODEL *pModel)
 *
 * Release model.
 */
void SrpModelRelease(MODEL *pModel)
{
    ASSERTMSG(pModel != NULL, "SrpModelRelease: invalid arguments.");

    IgFreeMemory(pModel->pOldList);
    IgFreeMemory(pModel->pNewList);
    IgFreeMemory(pModel->pTriList);
    IgFreeMemory(pModel);
}

/*------------------------------------------------------------------------------
 * float SrpGetModelRadius(const MODEL *pModel)
 *
 * Get model's radius;
 */
float SrpModelGetRadius(const MODEL *pModel)
{
    return (pModel->radius);
}
