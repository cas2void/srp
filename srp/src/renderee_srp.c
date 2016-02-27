/*******************************************************************************
 * File   : renderee_srp.c
 * Content: Polygon, object and render list manipulation functions
 *
 * Coder  : Ean Jee
 * Time   : 2011-09-23 17:03
 ******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "assert_ig.h"
#include "datadef_ig.h"
#include "malloc_ig.h"
#include "datadef_srp.h"
#include "renderee_srp.h"
#include "rcmanager_srp.h"
#include "math_srp.h"
#include "raster_srp.h"
#include "vector_srp.h"
#include "matrix_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

#define RENDER_LIST_MAX_TRIANGLES 32768

/*
 * A self-contained triangle used for render list
 */
struct TRIANGLE_INDIE_T
{
    int state;
    int attr;

    VECTOR3F vList[3];

    struct TRIANGLE_INDIE_T *next;
    struct TRIANGLE_INDIE_T *prev;
};

/*
 * A render list based on self-contained triangles
 */
struct RENDER_LIST_T
{
    int state;

    int numTriangles;
    struct TRIANGLE_INDIE_T *triPtr[RENDER_LIST_MAX_TRIANGLES];
    struct TRIANGLE_INDIE_T triData[RENDER_LIST_MAX_TRIANGLES];
};

/*----------------------------------------------------------------------------*/
/*                                Private Data                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                       Private Function Declarations                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             Private Functions                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                              Public Functions                              */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * int SrpCreateTriIndie(TRIANGLE_INDIE **ppTriIndie, const VECTOR3F a, 
 *                       const VECTOR3F b, const  VECTOR3F c)
 *
 * Create a self-contained triangle with three vectors
 */
int SrpCreateTriIndie(TRIANGLE_INDIE **ppTriIndie, const VECTOR3F a, 
                      const VECTOR3F b, const VECTOR3F c)
{
    TRIANGLE_INDIE *pTri;

    ASSERTMSG(ppTriIndie != NULL, "SrpCreateTriIndie: invalid arguments.");

    if (!IgNewMemory((void **)ppTriIndie, sizeof(TRIANGLE_INDIE)))
    {
        printf("Error: create TRIANGLE_INDIE failed.\n");
        return FALSE;
    }
    
    pTri = *ppTriIndie;
    SrpVectorCopy3f(pTri->vList[0], a);
    SrpVectorCopy3f(pTri->vList[1], b);
    SrpVectorCopy3f(pTri->vList[2], c);

    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpSetTriIndieVertex(TRIANGLE_INDIE *pTriIndie, const VECTOR3F a, 
 *                           const VECTOR3F b, const VECTOR3F c)
 *
 * Set self-contained triangle's vertex
 */
void SrpSetTriIndieVertex(TRIANGLE_INDIE *pTriIndie, const VECTOR3F a, 
                          const VECTOR3F b, const VECTOR3F c)
{
    ASSERTMSG(pTriIndie != NULL, "SrpSetTriIndieVertex: invalid arguments.");

    SrpVectorCopy3f(pTriIndie->vList[0], a);
    SrpVectorCopy3f(pTriIndie->vList[1], b);
    SrpVectorCopy3f(pTriIndie->vList[2], c);
}

/*------------------------------------------------------------------------------
 * void SrpSetTriIndieAttr(TRIANGLE_INDIE *pTriIndie, int attr)
 *
 * Set self-contained triangle's attribute
 */
void SrpSetTriIndieAttr(TRIANGLE_INDIE *pTriIndie, int attr)
{
    ASSERTMSG(pTriIndie != NULL, "SrpSetTriIndieAttr: invalid arguments.");

    pTriIndie->attr = attr;
}

/*------------------------------------------------------------------------------
 * void SrpDeleteTriIndie(TRIANGLE_INDIE *pTriIndie)
 *
 * Delete self-contained triangle
 */
void SrpDeleteTriIndie(TRIANGLE_INDIE *pTriIndie)
{
    ASSERTMSG(pTriIndie != NULL, "SrpDeleteTriIndie: invalid arguments.");

    IgFreeMemory(pTriIndie);
}

/*------------------------------------------------------------------------------
 * int SrpCreateRenderList(RENDER_LIST **ppRl)
 *
 * Create render list
 */
int SrpCreateRenderList(RENDER_LIST **ppRl)
{
    ASSERTMSG(ppRl != NULL, "SrpCreateRenderList: invalid argument.");

    if (!IgNewMemory((void **)ppRl, sizeof(RENDER_LIST)))
    {
        printf("Error: create render list failed.\n");
        return FALSE;
    }

    memset(*ppRl, 0, sizeof(RENDER_LIST));
    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpDeleteRenderList(RENDER_LIST *pRl)
 *
 * Delete render list
 */
void SrpDeleteRenderList(RENDER_LIST *pRl)
{
    IgFreeMemory(pRl);
}

/*------------------------------------------------------------------------------
 * void SrpResetRenderList(RENDER_LIST *pRl)
 *
 * Reset render list
 */
void SrpResetRenderList(RENDER_LIST *pRl)
{
    ASSERTMSG(pRl != NULL, "SrpResetRenderList: invalid argument.");

    memset(pRl, 0, sizeof(RENDER_LIST));
}

/*------------------------------------------------------------------------------
 * void SrpInsertTriangleToRenderList(const TRIANGLE_INDIE *pTri, 
 *                                    RENDER_LIST *pRl)
 *
 * Insert individual self-contained triangle into render list
 */
void SrpInsertTriangleToRenderList(const TRIANGLE_INDIE *pTri, RENDER_LIST *pRl)
{
    int curIndex;

    ASSERTMSG(pTri != NULL && pRl != NULL, 
              "SrpInsertTriangleToRenderList: invalid argument.");
    ASSERTMSG(pRl->numTriangles < RENDER_LIST_MAX_TRIANGLES, 
              "SrpInsertTriangleToRenderList: render list is full.");

    if (pRl->numTriangles < RENDER_LIST_MAX_TRIANGLES)
    {
        curIndex = pRl->numTriangles;

        /* Link triangle pointer with triangle data */
        pRl->triPtr[curIndex] = &pRl->triData[curIndex];

        /* Copy triangle data into render list */
        memcpy(pRl->triPtr[curIndex], pTri, sizeof(TRIANGLE_INDIE));

        pRl->numTriangles++;
    }
}

/*------------------------------------------------------------------------------
 * void SrpPrintRenderList(const RENDER_LIST *pRl)
 *
 * Print the render list
 */
void SrpPrintRenderList(const RENDER_LIST *pRl)
{
    int i;

    ASSERTMSG(pRl != NULL, "SrpPrintRenderList: invalid argument.");

    printf("Render List:\n");
    printf("\tNumber of Triangles: %d\n", pRl->numTriangles);

    for (i = 0; i < pRl->numTriangles; i++)
    {
        printf("\n\tTriangle %d:\n", i);
        SrpVectorPrint3f(pRl->triPtr[i]->vList[0], "vetex 1");
        SrpVectorPrint3f(pRl->triPtr[i]->vList[1], "vetex 2");
        SrpVectorPrint3f(pRl->triPtr[i]->vList[2], "vetex 3");
    }
}

/*------------------------------------------------------------------------------
 * void SrpTransRenderListLocToCam(RENDER_LIST *pRl)
 *
 * Transfrom render list from local space to camera space.
 */
void SrpTransRenderListLocToCam(RENDER_LIST *pRl)
{
    VECTOR3F tempVector;
    TRIANGLE_INDIE *pTri;
    int i, j;
    MATRIX43F *modelView;

    ASSERTMSG(pRl != NULL, "SrpTransRenderListLocToCam: invalid arguments.");

    modelView = SrpRCGetModelView();
    for (i = 0; i < pRl->numTriangles; i++)
    {
        pTri = pRl->triPtr[i];
        if (!pTri)
        {
            ASSERTMSG(FALSE, "SrpTransRenderListLocalToCamera: \
invalid TRIANGLE_INDIE pointer.");
            return;
        }

        for (j = 0; j < 3; j++)
        {
            SrpMatrixTransformVector3f(tempVector, pTri->vList[j], *modelView);
            SrpVectorCopy3f(pTri->vList[j], tempVector);
        }
    }
}

/*------------------------------------------------------------------------------
 * void SrpTransRenderListCamToProj(RENDER_LIST *pRl)
 *
 * Transfrom render list from camera space to projection space.
 */
void SrpTransRenderListCamToProj(RENDER_LIST *pRl)
{
    float oneOverTanTheta;
    TRIANGLE_INDIE *pTri;
    int i, j;
    float fovy, aspect;

    ASSERTMSG(pRl != NULL, "SrpTransRenderListCamToProj: invalid arguments.");

    fovy = SrpRCGetFovy();
    aspect = SrpRCGetAspect();

    oneOverTanTheta = 1.0f / tanf(SrpMathDegToRadf(fovy / 2.0f));

    for (i = 0; i < pRl->numTriangles; i++)
    {
        pTri = pRl->triPtr[i];
        if (!pTri)
        {
            ASSERTMSG(FALSE, "SrpTransRenderListCamToProj: \
invalid TRIANGLE_INDIE pointer.");
            return;
        }

        /*
         * Xp = Xc * d / Zc, Yp = Yc * d * aspect / Zc.
         * We use right-handed system, so d = -1 / tan. 
         */
        for (j = 0; j < 3; j++)
        {
            pTri->vList[j][0] = -1.0f * pTri->vList[j][0] * 
                oneOverTanTheta / pTri->vList[j][2];
            pTri->vList[j][1] = -1.0f * pTri->vList[j][1] * 
                oneOverTanTheta * aspect / pTri->vList[j][2];
        }
    }
}

/*------------------------------------------------------------------------------
 * void SrpTransRenderListProjToScr(RENDER_LIST *pRl)
 *
 * Transfrom render list from projection space to screen space.
 */
void SrpTransRenderListProjToScr(RENDER_LIST *pRl)
{
    float alpha;
    float beta;
    TRIANGLE_INDIE *pTri;
    int i, j;

    ASSERTMSG(pRl != NULL, "SrpTransRenderListProjToScr: invalid arguments.");

    alpha = (SrpRCGetWidth() - 1.0f) / 2.0f;
    beta  = (SrpRCGetHeight() - 1.0f) / 2.0f;

    for (i = 0; i < pRl->numTriangles; i++)
    {
        pTri = pRl->triPtr[i];
        if (!pTri)
        {
            ASSERTMSG(FALSE, "SrpTransRenderListProjToScr: \
invalid TRIANGLE_INDIE pointer.");
            return;
        }

        for (j = 0; j < 3; j++)
        {
            pTri->vList[j][0] = pTri->vList[j][0] * alpha + alpha;
            pTri->vList[j][1] = beta - pTri->vList[j][1] * beta;
        }
    }
}

/*------------------------------------------------------------------------------
 * void SrpDrawRenderList(const RENDER_LIST *pRl)
 *
 * Draw the render list
 */
void SrpDrawRenderList(const RENDER_LIST *pRl)
{
    int i;
    TRIANGLE_INDIE *pTri;
    POINT2I pointList[3];

    ASSERTMSG(pRl != NULL, "SrpDrawRenderList: invalid argument.");

    for (i = 0; i < pRl->numTriangles; i++)
    {
        pTri = pRl->triPtr[i];
        pointList[0].x = ROUND2INT(pTri->vList[0][0]);
        pointList[0].y = ROUND2INT(pTri->vList[0][1]);
        pointList[1].x = ROUND2INT(pTri->vList[1][0]);
        pointList[1].y = ROUND2INT(pTri->vList[1][1]);
        pointList[2].x = ROUND2INT(pTri->vList[2][0]);
        pointList[2].y = ROUND2INT(pTri->vList[2][1]);

        SrpDrawPolygonWire(pointList, 3);
    }
}
