/*******************************************************************************
 * File   : plane_srp.h
 * Content: Plane manipulation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-10-28 11:00
 ******************************************************************************/

#ifndef _PLANE_SRP_H
#define _PLANE_SRP_H

#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

#define PLANE VECTOR4F

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/* 
 * Load a plane from coefficients explicitly.
 * The equation of p is ax + by + cz + d = 0.
 */
extern void SrpPlaneLoad(PLANE plane, float a, float b, float c, float d);

/* 
 * Get a plane equation from three points.
 * The front face is defined by counter-clockwise order.
 */
extern void SrpPlaneGetEquation(PLANE plane, const VECTOR3F p1, 
                                const VECTOR3F p2, const VECTOR3F p3);

/* 
 * Get the distance from a point to the plane.
 */
extern float SrpPlaneGetDistance(const PLANE plane, const VECTOR3F p);

#endif /* _PLANE_SRP_H */
