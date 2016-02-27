/*******************************************************************************
 * File   : vector_srp.h
 * Content: Vector functions.
 *
 * Coder  : Ean Jee
 * Time   : 2011-04-11 16:26
 ******************************************************************************/

#ifndef _VECTOR_SRP_H
#define _VECTOR_SRP_H

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

typedef float VECTOR2F[2];
typedef double VECTOR2D[2];

typedef float VECTOR3F[3];
typedef double VECTOR3D[3];

typedef float VECTOR4F[4];
typedef double VECTOR4D[4];

extern const VECTOR3F ZERO_VECTOR, CARDINAL_X, CARDINAL_Y, CARDINAL_Z;

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/*
 * Load vectors.
 */
extern void SrpVectorLoad2f(VECTOR2F v, float x, float y);
extern void SrpVectorLoad2d(VECTOR2D v, double x, double y);
extern void SrpVectorLoad3f(VECTOR3F v, float x, float y, float z);
extern void SrpVectorLoad3d(VECTOR3D v, double x, double y, double z);
extern void SrpVectorLoad4f(VECTOR4F v, float x, float y, float z, float w);
extern void SrpVectorLoad4d(VECTOR4D v, double x, double y, double z, double w);

/*
 * Copy vectors.
 */
extern void SrpVectorCopy2f(VECTOR2F dst, const VECTOR2F src);
extern void SrpVectorCopy2d(VECTOR2D dst, const VECTOR2D src);
extern void SrpVectorCopy3f(VECTOR3F dst, const VECTOR3F src);
extern void SrpVectorCopy3d(VECTOR3D dst, const VECTOR3D src);
extern void SrpVectorCopy4f(VECTOR4F dst, const VECTOR4F src);
extern void SrpVectorCopy4d(VECTOR4D dst, const VECTOR4D src);

/*
 * Add vectors, out = u + v.
 */
extern void SrpVectorAdd2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v);
extern void SrpVectorAdd2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v);
extern void SrpVectorAdd3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v);
extern void SrpVectorAdd3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v);
extern void SrpVectorAdd4f(VECTOR4F out, const VECTOR4F u, const VECTOR4F v);
extern void SrpVectorAdd4d(VECTOR4D out, const VECTOR4D u, const VECTOR4D v);

/*
 * Subtract vectors, out = u - v.
 */
extern void SrpVectorSubtract2f(VECTOR2F out, const VECTOR2F u, 
                                const VECTOR2F v);
extern void SrpVectorSubtract2d(VECTOR2D out, const VECTOR2D u, 
                                const VECTOR2D v);            
extern void SrpVectorSubtract3f(VECTOR3F out, const VECTOR3F u, 
                                const VECTOR3F v);            
extern void SrpVectorSubtract3d(VECTOR3D out, const VECTOR3D u, 
                                const VECTOR3D v);            
extern void SrpVectorSubtract4f(VECTOR4F out, const VECTOR4F u, 
                                const VECTOR4F v);            
extern void SrpVectorSubtract4d(VECTOR4D out, const VECTOR4D u, 
                                const VECTOR4D v);

/*
 * Scale vectors uniformly, out = scale * v.
 */
extern void SrpVectorScale2f(VECTOR2F out, const VECTOR2F v, float scale);
extern void SrpVectorScale2d(VECTOR2D out, const VECTOR2D v, double scale);
extern void SrpVectorScale3f(VECTOR3F out, const VECTOR3F v, float scale);
extern void SrpVectorScale3d(VECTOR3D out, const VECTOR3D v, double scale);
extern void SrpVectorScale4f(VECTOR4F out, const VECTOR4F v, float scale);
extern void SrpVectorScale4d(VECTOR4D out, const VECTOR4D v, double scale);

/*
 * Scale vectors independently.
 */
extern void SrpVectorScaleIndie2f(VECTOR2F out, const VECTOR2F u, 
                                  const VECTOR2F v);
extern void SrpVectorScaleIndie2d(VECTOR2D out, const VECTOR2D u, 
                                  const VECTOR2D v);
extern void SrpVectorScaleIndie3f(VECTOR3F out, const VECTOR3F u, 
                                  const VECTOR3F v);
extern void SrpVectorScaleIndie3d(VECTOR3D out, const VECTOR3D u, 
                                  const VECTOR3D v);

/*
 * Get vector's length squared. 4D vector is not supported.
 */
extern float SrpVectorLengthSquared2f(const VECTOR2F v);
extern double SrpVectorLengthSquared2d(const VECTOR2D v);
extern float SrpVectorLengthSquared3f(const VECTOR3F v);
extern double SrpVectorLengthSquared3d(const VECTOR3D v);

/*
 * Get vector's length. 4D vector is not supported.
 */
extern float SrpVectorLength2f(const VECTOR2F v);
extern double SrpVectorLength2d(const VECTOR2D v);
extern float SrpVectorLength3f(const VECTOR3F v);
extern double SrpVectorLength3d(const VECTOR3D v);

/*
 * Get squared distance between two points. 4D vector is not supported.
 */
extern float SrpVectorDistanceSquared2f(const VECTOR2F u, const VECTOR2F v);
extern double SrpVectorDistanceSquared2d(const VECTOR2D u, const VECTOR2D v);
extern float SrpVectorDistanceSquared3f(const VECTOR3F u, const VECTOR3F v);
extern double SrpVectorDistanceSquared3d(const VECTOR3D u, const VECTOR3D v);

/*
 * Get distance between two points. 4D vector is not supported.
 */
extern float SrpVectorDistance2f(const VECTOR2F u, const VECTOR2F v);
extern double SrpVectorDistance2d(const VECTOR2D u, const VECTOR2D v);
extern float SrpVectorDistance3f(const VECTOR3F u, const VECTOR3F v);
extern double SrpVectorDistance3d(const VECTOR3D u, const VECTOR3D v);

/*
 * Normalize vector. 4D vector is not supported.
 * Store normalized vector in out.
 */
extern void SrpVectorNormalize2f(VECTOR2F out, const VECTOR2F v);
extern void SrpVectorNormalize2d(VECTOR2D out, const VECTOR2D v);
extern void SrpVectorNormalize3f(VECTOR3F out, const VECTOR3F v);
extern void SrpVectorNormalize3d(VECTOR3D out, const VECTOR3D v);

/*
 * Dot product, 4D vector is not supported.
 */
extern float SrpVectorDotProduct2f(const VECTOR2F u, const VECTOR2F v);
extern double SrpVectorDotProduct2d(const VECTOR2D u, const VECTOR2D v);
extern float SrpVectorDotProduct3f(const VECTOR3F u, const VECTOR3F v);
extern double SrpVectorDotProduct3d(const VECTOR3D u, const VECTOR3D v);

/*
 * Cross product, only for 3D vectors.
 */
extern void SrpVectorCrossProduct3f(VECTOR3F out, const VECTOR3F u, 
                                    const VECTOR3F v);
extern void SrpVectorCrossProduct3d(VECTOR3D out, const VECTOR3D u, 
                                    const VECTOR3D v);

/*
 * Get angle between two vectors, 4D vector is not supported.
 */
extern float SrpVectorAngle2f(const VECTOR2F u, const VECTOR2F v);
extern double SrpVectorAngle2d(const VECTOR2D u, const VECTOR2D v);
extern float SrpVectorAngle3f(const VECTOR3F u, const VECTOR3F v);
extern double SrpVectorAngle3d(const VECTOR3D u, const VECTOR3D v);

/* 
 * Get plane normal from three points on the plane.
 * The output normal vector is not normalized.
 */
extern void SrpVectorGetPlaneNormal3f(VECTOR3F out, const VECTOR3F p1, 
                                      const VECTOR3F p2, const VECTOR3F p3);
extern void SrpVectorGetPlaneNormal3d(VECTOR3D out, const VECTOR3D p1, 
                                      const VECTOR3D p2, const VECTOR3D p3);

/* 
 * Check if a vector is zero
 */
extern int SrpVectorIsZero2f(const VECTOR2F v);
extern int SrpVectorIsZero2d(const VECTOR2D v);
extern int SrpVectorIsZero3f(const VECTOR3F v);
extern int SrpVectorIsZero3d(const VECTOR3D v);

/*
 * Print vectors, used for debugging.
 */
extern void SrpVectorPrint2f(const VECTOR2F v, const char *name);
extern void SrpVectorPrint2d(const VECTOR2D v, const char *name);
extern void SrpVectorPrint3f(const VECTOR3F v, const char *name);
extern void SrpVectorPrint3d(const VECTOR3D v, const char *name);
extern void SrpVectorPrint4f(const VECTOR4F v, const char *name);
extern void SrpVectorPrint4d(const VECTOR4D v, const char *name);

#endif /* _VECTOR_SRP_H */
