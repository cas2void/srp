/*******************************************************************************
 * File   : transformer_srp.h
 * Content: Transformation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-05-05 17:15
 ******************************************************************************/

#ifndef _TRANSFORMER_SRP_H
#define _TRANSFORMER_SRP_H

#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/*
 * Combine the current matrix in RC with a 'pure' 
 * translation matrix for 'object' transformation from local 
 * to parent space.
 *
 * Parent space translates vector v to object space.
 */
extern void SrpTransformerSetTranslationf(const VECTOR3F v);
extern void SrpTransformerSetTranslationd(const VECTOR3D v);

/*
 * Combine the current matrix in RC with a 'pure' 
 * rotation matrix for 'object' transformation from local 
 * to parent space.
 *
 * Parent space rotates angle t about an arbitrary axis to object space.
 * This axis is defined by origin and vector v in parent space.
 *
 * t is the amount of rotation in degree.
 */
extern void SrpTransformerSetRotationf(const VECTOR3F v, float t);
extern void SrpTransformerSetRotationd(const VECTOR3D v, double t);

/*
 * Combine the current matrix in RC with a scale 
 * matrix for 'object' transformation from local to parent space.
 *
 * v[0], v[1] and v[2] define the scale factors along cardinal 
 * axis x, y and z, respectively.
 *
 * Parent space scales each factors along each axis to object space.
 */
extern void SrpTransformerSetScalef(const VECTOR3F v);
extern void SrpTransformerSetScaled(const VECTOR3D v);

/*
 * Setup a world-to-camera matrix based on UVN camera model,
 * combine the current modelview matrix in RC with that matrix
 * for 'object' transformation from world to camera space.
 *
 * These vectors are defined in world space.
 *
 * eye is the position of eye point;
 * center is position of the target;
 * up is the direction of the up vector.
 */
extern void SrpTransformerSetLookAt(const VECTOR3F eye, const VECTOR3F center, 
                                    const VECTOR3F up);

/*
 * Setup a camera-to-projection matrix, combine the current
 * projection matrix in RC with that matrix.
 */
extern void SrpTransformerSetProjection(float fovy, float aspect, 
                                        float near, float far);

/*
 * Setup a projection-to-screen matrix, replace the current
 * viewport transformation matrix in RC with that matrix.
 */
extern void SrpTransformerSetViewport(float width, float height);

#endif /* _TRANSFORMER_SRP_H */
