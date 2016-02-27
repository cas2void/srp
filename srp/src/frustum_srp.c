/*******************************************************************************
 * File   : frustum_srp.c
 * Content: Frustum manipulation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-10-28 16:45
 ******************************************************************************/

#include <stdio.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "malloc_ig.h"
#include "frustum_srp.h"
#include "plane_srp.h"
#include "math_srp.h"
#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

struct FRUSTUM_T
{
    PLANE near, far;
    PLANE top, down;
    PLANE left, right;
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
 * int SrpCreateFrustum(FRUSTUM **ppFrustum, float fovy, float aspect, 
 *                      float near, float far)
 *
 * Create frustum
 */
int SrpCreateFrustum(FRUSTUM **ppFrustum, float fovy, float aspect, 
                     float near, float far)
{
    ASSERTMSG(ppFrustum != NULL && fovy > 0.0f && fovy < 180.0f &&
              aspect > 0.0f && near < -1.0f && far < near,
              "SrpCreateFrustum: invalid arguments.");

    if (!IgNewMemory((void **)ppFrustum, sizeof(FRUSTUM)))
    {
        printf("Error: create frustum failed.\n");
        return FALSE;
    }

    SrpSetFrustum(*ppFrustum, fovy, aspect, near, far);

    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpDeleteFrustum(FRUSTUM *pFrustum)
 *
 * Delete frustum
 */
void SrpDeleteFrustum(FRUSTUM *pFrustum)
{
    ASSERTMSG(pFrustum != NULL, "SrpDeleteFrustum: invalid arguments.");

    IgFreeMemory(pFrustum);
}

/*------------------------------------------------------------------------------
 * void SrpSetFrustum(FRUSTUM *pFrustum, float fovy, float aspect, 
 *                    float near, float far)
 *
 * Set frustum
 */
void SrpSetFrustum(FRUSTUM *pFrustum, float fovy, float aspect, 
                   float near, float far)
{
    /* Vertices of frustum */
    VECTOR3F nearUL, nearUR, nearLL, nearLR;
    VECTOR3F farUL, farUR, farLL, farLR;

    float tanTheta;
    float halfHeightNear, halfWidthNear, halfHeightFar, halfWidthFar;

    ASSERTMSG(pFrustum != NULL && fovy > 0.0f && fovy < 180.0f &&
              aspect > 0.0f && near < -1.0f && far < near,
              "SrpSetFrustum: invalid arguments.");

    /* Calculate the 8 vertices of the frustum */
    tanTheta = tanf(SrpMathDegToRadf(fovy / 2.0f));

    halfWidthNear  = -near * tanTheta;
    halfHeightNear = halfWidthNear / aspect;
    halfWidthFar   = -far * tanTheta;
    halfHeightFar  = halfWidthFar / aspect;

    nearUL[0] = -halfWidthNear;
    nearUL[1] = halfHeightNear;
    nearUL[2] = near;

    nearUR[0] = halfWidthNear;
    nearUR[1] = halfHeightNear;
    nearUR[2] = near;

    nearLL[0] = -halfWidthNear;
    nearLL[1] = -halfHeightNear;
    nearLL[2] = near;

    nearLR[0] = halfWidthNear;
    nearLR[1] = -halfHeightNear;
    nearLR[2] = near;

    farUL[0] = -halfWidthFar;
    farUL[1] = halfHeightFar;
    farUL[2] = far;

    farUR[0] = halfWidthFar;
    farUR[1] = halfHeightFar;
    farUR[2] = far;

    farLL[0] = -halfWidthFar;
    farLL[1] = -halfHeightFar;
    farLL[2] = far;

    farLR[0] = halfWidthFar;
    farLR[1] = -halfHeightFar;
    farLR[2] = far;

    /* Get the equation for each face of the frustum,
     * normal points inside of the frustum.
     */
    SrpPlaneGetEquation(pFrustum->near,  nearUL, nearUR, nearLL);
    SrpPlaneGetEquation(pFrustum->far,   farLR,  farUR,  farLL);
    SrpPlaneGetEquation(pFrustum->top,   farUL,  farUR,  nearUL);
    SrpPlaneGetEquation(pFrustum->down,  nearLR, farLR,  nearLL);
    SrpPlaneGetEquation(pFrustum->left,  farUL,  nearUL, farLL);
    SrpPlaneGetEquation(pFrustum->right, nearLR, nearUR, farLR);
}

/*------------------------------------------------------------------------------
 * int SrpIsVisibleInFrustum(const FRUSTUM *pFrustum, const VECTOR3F pos, 
 *                           float radius)
 *
 * Check if an object is visible with a frustum.
 * The object is define by its postion and radius.
 * 
 * Return:
 *     TRUE if the object is partly or completely visible.
 *     FALSE if the object is completely invisible.
 */
int SrpIsVisibleInFrustum(const FRUSTUM *pFrustum, const VECTOR3F pos, 
                          float radius)
{
    ASSERTMSG(pFrustum != NULL && radius > 0.0f,
              "SrpIsVisibleInFrustum: invalid arguments.");

    if (SrpPlaneGetDistance(pFrustum->near,  pos) < radius || 
        SrpPlaneGetDistance(pFrustum->far,   pos) < radius || 
        SrpPlaneGetDistance(pFrustum->top,   pos) < radius || 
        SrpPlaneGetDistance(pFrustum->down,  pos) < radius || 
        SrpPlaneGetDistance(pFrustum->left,  pos) < radius || 
        SrpPlaneGetDistance(pFrustum->right, pos) < radius)
    {
        return FALSE;
    }

/*
    if (SrpPlaneGetDistance(pFrustum->near,  pos) < radius)
    {
        printf("cull on near.\n");
        return FALSE;
    }

    if (SrpPlaneGetDistance(pFrustum->far,  pos) < radius)
    {
        printf("cull on far.\n");
        return FALSE;
    }

    if (SrpPlaneGetDistance(pFrustum->top,  pos) < radius)
    {
        printf("cull on top.\n");
        return FALSE;
    }

    if (SrpPlaneGetDistance(pFrustum->down,  pos) < radius)
    {
        printf("cull on down.\n");
        return FALSE;
    }

    if (SrpPlaneGetDistance(pFrustum->left,  pos) < radius)
    {
        printf("cull on left.\n");
        return FALSE;
    }

    if (SrpPlaneGetDistance(pFrustum->right,  pos) < radius)
    {
        printf("cull on right.\n");
        return FALSE;
    }
*/
    return TRUE;
}
