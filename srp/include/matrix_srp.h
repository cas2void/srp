/*******************************************************************************
 * File   : matrix_srp.h
 * Content: Matrix functions. For now, just 4X3 matrix.
 *
 * Coder  : Ean Jee
 * Time   : 2011-04-20 11:51
 ******************************************************************************/

#ifndef _MATRIX_SRP_H
#define _MATRIX_SRP_H

#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

/*
 * 4X3 matrix, row major.
 *
 * | 0  1  2 |
 * | 3  4  5 |
 * | 6  7  8 |
 * | 9 10 11 |
 */
typedef float MATRIX43F[12];
typedef double MATRIX43D[12];

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/*
 * Load identity.
 */
extern void SrpMatrixLoadIdentity43f(MATRIX43F m);
extern void SrpMatrixLoadIdentity43d(MATRIX43D m);

/*
 * Copy Matrix.
 */
extern void SrpMatrixCopy43f(MATRIX43F dst, const MATRIX43F src);
extern void SrpMatrixCopy43d(MATRIX43D dst, const MATRIX43D src);

/*
 * Multiply matrix, out = a * b.
 */
extern void SrpMatrixMultiply43f(MATRIX43F out, const MATRIX43F a, 
                                 const MATRIX43F b);
extern void SrpMatrixMultiply43d(MATRIX43D out, const MATRIX43D a, 
                                 const MATRIX43D b);

/*
 * Invert matrix.
 */
extern int SrpMatrixInvert43f(MATRIX43F out, const MATRIX43F m);
extern int SrpMatrixInvert43d(MATRIX43D out, const MATRIX43D m);

/*
 * Make a 'pure' translation matrix for 'object' transformation 
 * from local to parent space.
 *
 * Parent space translates vector v to object space.
 */
extern void SrpMatrixMakeTranslation43f(MATRIX43F out, const VECTOR3F v);
extern void SrpMatrixMakeTranslation43d(MATRIX43D out, const VECTOR3D v);

/*
 * Make a 'pure' rotation matrix for 'object' transformation 
 * from local to parent space.
 *
 * Parent space rotates angle t about an arbitrary axis to object space.
 * This axis is defined by origin and vector v in parent space.
 *
 * t is the amount of rotation in radians.
 */
extern void SrpMatrixMakeRotation43f(MATRIX43F out, const VECTOR3F v, float t);
extern void SrpMatrixMakeRotation43d(MATRIX43D out, const VECTOR3D v,double t);

/*
 * Make a scale matrix for 'object' transformation 
 * from local to parent space.
 *
 * v[0], v[1] and v[2] define the scale factors along cardinal 
 * axis x, y and z, respectively.
 *
 * Parent space scales each factors along each axis to object space.
 */
extern void SrpMatrixMakeScale43f(MATRIX43F out, const VECTOR3F v);
extern void SrpMatrixMakeScale43d(MATRIX43D out, const VECTOR3D v);

/*
 * Transform vector, out = v * m.
 */
extern void SrpMatrixTransformVector3f(VECTOR3F out, const VECTOR3F v, 
                                       const MATRIX43F m);
extern void SrpMatrixTransformVector3d(VECTOR3D out, const VECTOR3D v, 
                                       const MATRIX43D m);
extern void SrpMatrixTransformVector4f(VECTOR4F out, const VECTOR4F v, 
                                       const MATRIX43F m);
extern void SrpMatrixTransformVector4d(VECTOR4D out, const VECTOR4D v, 
                                       const MATRIX43D m);

/*
 * Print matrix.
 */
extern void SrpMatrixPrint43f(const MATRIX43F m, const char *name);
extern void SrpMatrixPrint43d(const MATRIX43D m, const char *name);

#endif /* _MATRIX_SRP_H */
