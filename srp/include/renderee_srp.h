/*******************************************************************************
 * File   : renderee_srp.h
 * Content: Polygon and render list manipulation functions
 *
 * Coder  : Ean Jee
 * Time   : 2011-09-22 16:17
 ******************************************************************************/

#ifndef _RENDEREE_SRP_H
#define _RENDEREE_SRP_H

#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

/*
 * A self-contained triangle used for render list
 */
struct TRIANGLE_INDIE_T;
typedef struct TRIANGLE_INDIE_T TRIANGLE_INDIE;

/*
 * A render list based on self-contained triangles
 */
struct RENDER_LIST_T;
typedef struct RENDER_LIST_T RENDER_LIST;

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/* 
 * Create a self-contained triangle with three vectors
 */
extern int SrpCreateTriIndie(TRIANGLE_INDIE **ppTriIndie, const VECTOR3F a, 
                             const VECTOR3F b, const VECTOR3F c);

/* 
 * Set self-contained triangle's vertex
 */
extern void SrpSetTriIndieVertex(TRIANGLE_INDIE *pTriIndie, const VECTOR3F a, 
                                 const VECTOR3F b, const VECTOR3F c);

/* 
 * Set self-contained triangle's attribute
 */
extern void SrpSetTriIndieAttr(TRIANGLE_INDIE *pTriIndie, int attr);

/*
 * Delete self-contained triangle
 */
extern void SrpDeleteTriIndie(TRIANGLE_INDIE *pTriIndie);

/* 
 * Create render list
 */
extern int SrpCreateRenderList(RENDER_LIST **ppRl);

/* 
 * Delete render list
 */
extern void SrpDeleteRenderList(RENDER_LIST *pRl);

/*
 * Reset render list
 */
extern void SrpResetRenderList(RENDER_LIST *pRl);

/*
 * Insert individual self-contained triangle into render list
 */
extern void SrpInsertTriangleToRenderList(const TRIANGLE_INDIE *tri, 
    RENDER_LIST *pRl);

/*
 * Print the render list
 */
extern void SrpPrintRenderList(const RENDER_LIST *pRl);

/*
 * Transfrom render list from local space to camera space.
 */
extern void SrpTransRenderListLocToCam(RENDER_LIST *pRl);

/*
 * Transfrom render list from camera space to projection space.
 */
extern void SrpTransRenderListCamToProj(RENDER_LIST *pRl);

/*
 * Transfrom render list from projection space to screen space.
 */
extern void SrpTransRenderListProjToScr(RENDER_LIST *pRl);

/* 
 * Draw the render list.
 */
extern void SrpDrawRenderList(const RENDER_LIST *pRl);

#endif /* _RENDEREE_SRP_H */
