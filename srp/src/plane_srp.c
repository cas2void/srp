/*******************************************************************************
 * File   : plane_srp.c
 * Content: Plane manipulation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-10-28 11:13
 ******************************************************************************/

#include "plane_srp.h"
#include "vector_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

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
 * void SrpPlaneLoad(PLANE plane, float a, float b, float c, float d)
 *
 * Load a plane from coefficients explicitly.
 * The equation of p is ax + by + cz + d = 0.
 */
void SrpPlaneLoad(PLANE plane, float a, float b, float c, float d)
{
    plane[0] = a;
    plane[1] = b;
    plane[2] = c;
    plane[3] = d;
}

/*------------------------------------------------------------------------------
 * void SrpPlaneGetEquation(PLANE plane, const VECTOR3F p1, 
 *                          const VECTOR3F p2, const VECTOR3F p3)
 *
 * Get a plane equation from three points.
 * The front face is defined by counter-clockwise order.
 */
void SrpPlaneGetEquation(PLANE plane, const VECTOR3F p1, 
                         const VECTOR3F p2, const VECTOR3F p3)
{
    VECTOR3F n;

    /* Get the normal */
    SrpVectorGetPlaneNormal3f(n, p1, p2, p3);
    SrpVectorNormalize3f(plane, n);

    /* Back substitude to get d */
    plane[3] = -(plane[0] * p1[0] + plane[1] * p1[1] + plane[2] * p1[2]);
}

/*------------------------------------------------------------------------------
 * float SrpPlaneGetDistance(const PLANE plane, const VECTOR3F p)
 *
 * Get the distance from a point to the plane.
 */
float SrpPlaneGetDistance(const PLANE plane, const VECTOR3F p)
{
    return (plane[0] * p[0] + plane[1] * p[1] + plane[2] * p[2] + plane[3]);
}
