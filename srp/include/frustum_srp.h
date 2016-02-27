/*******************************************************************************
 * File   : frustum_srp.h
 * Content: Frustum manipulation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-10-28 16:36
 ******************************************************************************/

#ifndef _FRUSTUM_SRP_H
#define _FRUSTUM_SRP_H

#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

struct FRUSTUM_T;
typedef struct FRUSTUM_T FRUSTUM;

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/* 
 * Create frustum
 */
extern int SrpCreateFrustum(FRUSTUM **ppFrustum, float fovy, float aspect, 
                            float near, float far);

/* 
 * Delete frustum
 */
extern void SrpDeleteFrustum(FRUSTUM *pFrustum);

/* 
 * Set frustum
 */
extern void SrpSetFrustum(FRUSTUM *pFrustum, float fovy, float aspect, 
                          float near, float far);

/* 
 * Check if an object is visible with a frustum.
 * The object is define by its postion and radius.
 */
extern int SrpIsVisibleInFrustum(const FRUSTUM *pFrustum, const VECTOR3F pos, 
                                 float radius);

#endif /* _FRUSTUM_SRP_H */
