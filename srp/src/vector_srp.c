/*******************************************************************************
 * File   : vector_srp.c
 * Content: Vector functions.
 *
 * Coder  : Ean Jee
 * Time   : 2011-04-14 08:36
 ******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "vector_srp.h"
#include "math_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

const VECTOR3F ZERO_VECTOR = {0.0f, 0.0f, 0.0f};

const VECTOR3F CARDINAL_X  = {1.0f, 0.0f, 0.0f};
const VECTOR3F CARDINAL_Y  = {0.0f, 1.0f, 0.0f};
const VECTOR3F CARDINAL_Z  = {0.0f, 0.0f, 1.0f};

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
 * void SrpVectorLoad2f(VECTOR2F v, float x, float y)
 * void SrpVectorLoad2d(VECTOR2D v, double x, double y)
 * void SrpVectorLoad3f(VECTOR3F v, float x, float y, float z)
 * void SrpVectorLoad3d(VECTOR3D v, double x, double y, double z)
 * void SrpVectorLoad4f(VECTOR4F v, float x, float y, float z, float w)
 * void SrpVectorLoad4d(VECTOR4D v, double x, double y, double z, double w)
 *
 * These functions load vector v with x, y, z, w.
 */
void SrpVectorLoad2f(VECTOR2F v, float x, float y)
{
    v[0] = x;
    v[1] = y;
}

void SrpVectorLoad2d(VECTOR2D v, double x, double y)
{
    v[0] = x;
    v[1] = y;
}

void SrpVectorLoad3f(VECTOR3F v, float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void SrpVectorLoad3d(VECTOR3D v, double x, double y, double z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void SrpVectorLoad4f(VECTOR4F v, float x, float y, float z, float w)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

void SrpVectorLoad4d(VECTOR4D v, double x, double y, double z, double w)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

/*------------------------------------------------------------------------------
 * void SrpVectorCopy2f(VECTOR2F dst, const VECTOR2F src)
 * void SrpVectorCopy2d(VECTOR2D dst, const VECTOR2D src)
 * void SrpVectorCopy3f(VECTOR3F dst, const VECTOR3F src)
 * void SrpVectorCopy3d(VECTOR3D dst, const VECTOR3D src)
 * void SrpVectorCopy4f(VECTOR4F dst, const VECTOR4F src)
 * void SrpVectorCopy4d(VECTOR4D dst, const VECTOR4D src)
 *
 * These functions copy vector src into vector dst.
 */
void SrpVectorCopy2f(VECTOR2F dst, const VECTOR2F src)
{
    memcpy(dst, src, sizeof(VECTOR2F));
}

void SrpVectorCopy2d(VECTOR2D dst, const VECTOR2D src)
{
    memcpy(dst, src, sizeof(VECTOR2D));
}

void SrpVectorCopy3f(VECTOR3F dst, const VECTOR3F src)
{
    memcpy(dst, src, sizeof(VECTOR3F));
}

void SrpVectorCopy3d(VECTOR3D dst, const VECTOR3D src)
{
    memcpy(dst, src, sizeof(VECTOR3D));
}

void SrpVectorCopy4f(VECTOR4F dst, const VECTOR4F src)
{
    memcpy(dst, src, sizeof(VECTOR4F));
}

void SrpVectorCopy4d(VECTOR4D dst, const VECTOR4D src)
{
    memcpy(dst, src, sizeof(VECTOR4D));
}

/*------------------------------------------------------------------------------
 * void SrpVectorAdd2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v)
 * void SrpVectorAdd2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v)
 * void SrpVectorAdd3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
 * void SrpVectorAdd3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
 * void SrpVectorAdd4f(VECTOR4F out, const VECTOR4F u, const VECTOR4F v)
 * void SrpVectorAdd4d(VECTOR4D out, const VECTOR4D u, const VECTOR4D v)

 * These functions add vectors, out = u + v.
 */
void SrpVectorAdd2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v)
{
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
}

void SrpVectorAdd2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v)
{
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
}

void SrpVectorAdd3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
{
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
    out[2] = u[2] + v[2];
}

void SrpVectorAdd3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
{
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
    out[2] = u[2] + v[2];
}

void SrpVectorAdd4f(VECTOR4F out, const VECTOR4F u, const VECTOR4F v)
{
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
    out[2] = u[2] + v[2];
    out[3] = u[3] + v[3];
}

void SrpVectorAdd4d(VECTOR4D out, const VECTOR4D u, const VECTOR4D v)
{
    out[0] = u[0] + v[0];
    out[1] = u[1] + v[1];
    out[2] = u[2] + v[2];
    out[3] = u[3] + v[3];
}

/*------------------------------------------------------------------------------
 * void SrpVectorSubtract2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v)
 * void SrpVectorSubtract2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v)
 * void SrpVectorSubtract3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
 * void SrpVectorSubtract3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
 * void SrpVectorSubtract4f(VECTOR4F out, const VECTOR4F u, const VECTOR4F v)
 * void SrpVectorSubtract4d(VECTOR4D out, const VECTOR4D u, const VECTOR4D v)

 * These functions subtract vector v from vector u, out = u - v.
 */
void SrpVectorSubtract2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v)
{
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
}

void SrpVectorSubtract2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v)
{
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
}

void SrpVectorSubtract3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
{
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
    out[2] = u[2] - v[2];
}

void SrpVectorSubtract3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
{
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
    out[2] = u[2] - v[2];
}

void SrpVectorSubtract4f(VECTOR4F out, const VECTOR4F u, const VECTOR4F v)
{
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
    out[2] = u[2] - v[2];
    out[3] = u[3] - v[3];
}

void SrpVectorSubtract4d(VECTOR4D out, const VECTOR4D u, const VECTOR4D v)
{
    out[0] = u[0] - v[0];
    out[1] = u[1] - v[1];
    out[2] = u[2] - v[2];
    out[3] = u[3] - v[3];
}

/*------------------------------------------------------------------------------
 * void SrpVectorScale2f(VECTOR2F out, const VECTOR2F v, float scale)
 * void SrpVectorScale2d(VECTOR2D out, const VECTOR2D v, double scale)
 * void SrpVectorScale3f(VECTOR3F out, const VECTOR3F v, float scale)
 * void SrpVectorScale3d(VECTOR3D out, const VECTOR3D v, double scale)
 * void SrpVectorScale4f(VECTOR4F out, const VECTOR4F v, float scale)
 * void SrpVectorScale4d(VECTOR4D out, const VECTOR4D v, double scale)

 * These functions scale vector v with value scale. out = v * scale.
 */
void SrpVectorScale2f(VECTOR2F out, const VECTOR2F v, float scale)
{
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
}

void SrpVectorScale2d(VECTOR2D out, const VECTOR2D v, double scale)
{
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
}

void SrpVectorScale3f(VECTOR3F out, const VECTOR3F v, float scale)
{
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
}

void SrpVectorScale3d(VECTOR3D out, const VECTOR3D v, double scale)
{
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
}

void SrpVectorScale4f(VECTOR4F out, const VECTOR4F v, float scale)
{
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
    out[3] = v[3] * scale;
}

void SrpVectorScale4d(VECTOR4D out, const VECTOR4D v, double scale)
{
    out[0] = v[0] * scale;
    out[1] = v[1] * scale;
    out[2] = v[2] * scale;
    out[3] = v[3] * scale;
}

/*------------------------------------------------------------------------------
 * void SrpVectorScaleIndie2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v)
 * void SrpVectorScaleIndie2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v)
 * void SrpVectorScaleIndie3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
 * void SrpVectorScaleIndie3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
 *
 * Scale vectors with each member scaled independently.
 */
void SrpVectorScaleIndie2f(VECTOR2F out, const VECTOR2F u, const VECTOR2F v)
{
    out[0] = u[0] * v[0];
    out[1] = u[1] * v[1];
}

void SrpVectorScaleIndie2d(VECTOR2D out, const VECTOR2D u, const VECTOR2D v)
{
    out[0] = u[0] * v[0];
    out[1] = u[1] * v[1];
}

void SrpVectorScaleIndie3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
{
    out[0] = u[0] * v[0];
    out[1] = u[1] * v[1];
    out[2] = u[2] * v[2];
}

void SrpVectorScaleIndie3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
{
    out[0] = u[0] * v[0];
    out[1] = u[1] * v[1];
    out[2] = u[2] * v[2];
}

/*------------------------------------------------------------------------------
 * float SrpVectorLengthSquared2f(const VECTOR2F v)
 * double SrpVectorLengthSquared2d(const VECTOR2D v)
 * float SrpVectorLengthSquared3f(const VECTOR3F v)
 * double SrpVectorLengthSquared3d(const VECTOR3D v)

 * These functions return vector's length squared.
 * 4D vector is not supported.
 */
float SrpVectorLengthSquared2f(const VECTOR2F v)
{
    return (v[0] * v[0] + v[1] * v[1]);
}

double SrpVectorLengthSquared2d(const VECTOR2D v)
{
    return (v[0] * v[0] + v[1] * v[1]);
}

float SrpVectorLengthSquared3f(const VECTOR3F v)
{
    return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

double SrpVectorLengthSquared3d(const VECTOR3D v)
{
    return (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/*------------------------------------------------------------------------------
 * float SrpVectorLength2f(const VECTOR2F v)
 * double SrpVectorLength2d(const VECTOR2D v)
 * float SrpVectorLength3f(const VECTOR3F v)
 * double SrpVectorLength3d(const VECTOR3D v)

 * These functions return vector's length.
 * 4D vector is not supported.
 */
float SrpVectorLength2f(const VECTOR2F v)
{
    return sqrtf(SrpVectorLengthSquared2f(v));
}

double SrpVectorLength2d(const VECTOR2D v)
{
    return sqrt(SrpVectorLengthSquared2d(v));
}

float SrpVectorLength3f(const VECTOR3F v)
{
    return sqrtf(SrpVectorLengthSquared3f(v));
}

double SrpVectorLength3d(const VECTOR3D v)
{
    return sqrt(SrpVectorLengthSquared3d(v));
}

/*------------------------------------------------------------------------------
 * float SrpVectorDistanceSquared2f(const VECTOR2F u, const VECTOR2F v)
 * double SrpVectorDistanceSquared2d(const VECTOR2D u, const VECTOR2D v)
 * float SrpVectorDistanceSquared3f(const VECTOR3F u, const VECTOR3F v)
 * double SrpVectorDistanceSquared3d(const VECTOR3D u, const VECTOR3D v)

 * These functions return the squared distance between two points.
 * 4D vector is not supported.
 */
float SrpVectorDistanceSquared2f(const VECTOR2F u, const VECTOR2F v)
{
    VECTOR2F dif;

    SrpVectorSubtract2f(dif, u, v);

    return SrpVectorLengthSquared2f(dif);
}

double SrpVectorDistanceSquared2d(const VECTOR2D u, const VECTOR2D v)
{
    VECTOR2D dif;

    SrpVectorSubtract2d(dif, u, v);

    return SrpVectorLengthSquared2d(dif);
}

float SrpVectorDistanceSquared3f(const VECTOR3F u, const VECTOR3F v)
{
    VECTOR3F dif;

    SrpVectorSubtract3f(dif, u, v);

    return SrpVectorLengthSquared3f(dif);
}

double SrpVectorDistanceSquared3d(const VECTOR3D u, const VECTOR3D v)
{
    VECTOR3D dif;

    SrpVectorSubtract3d(dif, u, v);

    return SrpVectorLengthSquared3d(dif);
}

/*------------------------------------------------------------------------------
 * float SrpVectorDistance2f(const VECTOR2F u, const VECTOR2F v)
 * double SrpVectorDistance2d(const VECTOR2D u, const VECTOR2D v)
 * float SrpVectorDistance3f(const VECTOR3F u, const VECTOR3F v)
 * double SrpVectorDistance3d(const VECTOR3D u, const VECTOR3D v)

 * These functions return the distance between two points.
 * 4D vector is not supported.
 */
float SrpVectorDistance2f(const VECTOR2F u, const VECTOR2F v)
{
    return sqrtf(SrpVectorDistanceSquared2f(u, v));
}

double SrpVectorDistance2d(const VECTOR2D u, const VECTOR2D v)
{
    return sqrt(SrpVectorDistanceSquared2d(u, v));
}

float SrpVectorDistance3f(const VECTOR3F u, const VECTOR3F v)
{
    return sqrtf(SrpVectorDistanceSquared3f(u, v));
}

double SrpVectorDistance3d(const VECTOR3D u, const VECTOR3D v)
{
    return sqrt(SrpVectorDistanceSquared3d(u, v));
}

/*------------------------------------------------------------------------------
 * void SrpVectorNormalize2f(VECTOR2F out, const VECTOR2F v)
 * void SrpVectorNormalize2d(VECTOR2D out, const VECTOR2D v)
 * void SrpVectorNormalize3f(VECTOR3F out, const VECTOR3F v)
 * void SrpVectorNormalize3d(VECTOR3D out, const VECTOR3D v)

 * These functions normalize vector v, store it in vector out.
 * 4D vector is not supported.
 */
void SrpVectorNormalize2f(VECTOR2F out, const VECTOR2F v)
{
    float length = SrpVectorLength2f(v);
    
    if (fabsf(length) < EPSILON_E6F)
    {
        ASSERTMSG(FALSE, 
                  "SrpVectorNormalize2f: input vector is zero vector");
        SrpVectorCopy2f(out, v);
    }
    else
    {
        SrpVectorScale2f(out, v, 1.0f / length);
    }
}

void SrpVectorNormalize2d(VECTOR2D out, const VECTOR2D v)
{
    double length = SrpVectorLength2d(v);
    
    if (fabs(length) < EPSILON_E6D)
    {
        ASSERTMSG(FALSE, 
                  "SrpVectorNormalize2d: input vector is zero vector");
        SrpVectorCopy2d(out, v);
    }
    else
    {
        SrpVectorScale2d(out, v, 1.0 / length);
    }
}

void SrpVectorNormalize3f(VECTOR3F out, const VECTOR3F v)
{
    float length = SrpVectorLength3f(v);
    
    if (fabsf(length) < EPSILON_E6F)
    {
        ASSERTMSG(FALSE, 
                  "SrpVectorNormalize3f: input vector is zero vector");
        SrpVectorCopy3f(out, v);
    }
    else
    {
        SrpVectorScale3f(out, v, 1.0f / length);
    }
}

void SrpVectorNormalize3d(VECTOR3D out, const VECTOR3D v)
{
    double length = SrpVectorLength3d(v);
    
    if (fabs(length) < EPSILON_E6D)
    {
        ASSERTMSG(FALSE, 
                  "SrpVectorNormalize3d: input vector is zero vector");
        SrpVectorCopy3d(out, v);
    }
    else
    {
        SrpVectorScale3d(out, v, 1.0 / length);
    }
}

/*------------------------------------------------------------------------------
 * float SrpVectorDotProduct2f(const VECTOR2F u, const VECTOR2F v)
 * double SrpVectorDotProduct2d(const VECTOR2D u, const VECTOR2D v)
 * float SrpVectorDotProduct3f(const VECTOR3F u, const VECTOR3F v)
 * double SrpVectorDotProduct3d(const VECTOR3D u, const VECTOR3D v)

 * These functions return the dot product of vectors u and v.
 * 4D vector is not supported.
 */
float SrpVectorDotProduct2f(const VECTOR2F u, const VECTOR2F v)
{
    return (u[0] * v[0] + u[1] * v[1]);
}

double SrpVectorDotProduct2d(const VECTOR2D u, const VECTOR2D v)
{
    return (u[0] * v[0] + u[1] * v[1]);
}

float SrpVectorDotProduct3f(const VECTOR3F u, const VECTOR3F v)
{
    return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}

double SrpVectorDotProduct3d(const VECTOR3D u, const VECTOR3D v)
{
    return (u[0] * v[0] + u[1] * v[1] + u[2] * v[2]);
}

/*------------------------------------------------------------------------------
 * void SrpVectorCrossProduct3f(VECTOR3F out, const VECTOR3F u, 
 *                              const VECTOR3F v)
 * void SrpVectorCrossProduct3d(VECTOR3F out, const VECTOR3D u, 
 *                              const VECTOR3D v)

 * These functions get the cross product of vectors u and v,
 * store it in vector out.
 * Only for 3D vectors.
 */
void SrpVectorCrossProduct3f(VECTOR3F out, const VECTOR3F u, const VECTOR3F v)
{
    out[0] = u[1] * v[2] - u[2] * v[1];
    out[1] = u[2] * v[0] - u[0] * v[2];
    out[2] = u[0] * v[1] - u[1] * v[0];
}

void SrpVectorCrossProduct3d(VECTOR3D out, const VECTOR3D u, const VECTOR3D v)
{
    out[0] = u[1] * v[2] - u[2] * v[1];
    out[1] = u[2] * v[0] - u[0] * v[2];
    out[2] = u[0] * v[1] - u[1] * v[0];
}

/*------------------------------------------------------------------------------
 * float SrpVectorAngle2f(const VECTOR2F u, const VECTOR2F v)
 * double SrpVectorAngle2d(const VECTOR2D u, const VECTOR2D v)
 * float SrpVectorAngle3f(const VECTOR3F u, const VECTOR3F v)
 * double SrpVectorAngle3d(const VECTOR3D u, const VECTOR3D v)
 
 * These functions get the angle between vectors u and v
 * 4D vector is not supported.
 */
float SrpVectorAngle2f(const VECTOR2F u, const VECTOR2F v)
{
    VECTOR2F uNor,vNor;
    float dotProd;

    SrpVectorNormalize2f(uNor, u);
    SrpVectorNormalize2f(vNor, v);

    dotProd = SrpVectorDotProduct2f(uNor, vNor);
    dotProd = SrpMathMin(dotProd,  1.0f);
    dotProd = SrpMathMax(dotProd, -1.0f);

    return acosf(dotProd);
}

double SrpVectorAngle2d(const VECTOR2D u, const VECTOR2D v)
{
    VECTOR2D uNor,vNor;
    double dotProd;

    SrpVectorNormalize2d(uNor, u);
    SrpVectorNormalize2d(vNor, v);

    dotProd = SrpVectorDotProduct2d(uNor, vNor);
    dotProd = SrpMathMin(dotProd,  1.0);
    dotProd = SrpMathMax(dotProd, -1.0);

    return acos(dotProd);
}

float SrpVectorAngle3f(const VECTOR3F u, const VECTOR3F v)
{
    VECTOR3F uNor,vNor;
    float dotProd;

    SrpVectorNormalize3f(uNor, u);
    SrpVectorNormalize3f(vNor, v);

    dotProd = SrpVectorDotProduct3f(uNor, vNor);
    dotProd = SrpMathMin(dotProd,  1.0f);
    dotProd = SrpMathMax(dotProd, -1.0f);

    return acosf(dotProd);
}

double SrpVectorAngle3d(const VECTOR3D u, const VECTOR3D v)
{
    VECTOR3D uNor,vNor;
    double dotProd;

    SrpVectorNormalize3d(uNor, u);
    SrpVectorNormalize3d(vNor, v);

    dotProd = SrpVectorDotProduct3d(uNor, vNor);
    dotProd = SrpMathMin(dotProd,  1.0);
    dotProd = SrpMathMax(dotProd, -1.0);

    return acos(dotProd);
}

/*------------------------------------------------------------------------------
 * void SrpVectorGetPlaneNormal3f(VECTOR3F out, const VECTOR3F p1, 
 *                              const VECTOR3F p2, const VECTOR3F p3)
 * void SrpVectorGetPlaneNormal3d(VECTOR3D out, const VECTOR3D p1, 
 *                              const VECTOR3D p2, const VECTOR3D p3)
 *
 * Get plane normal from three points on the plane.
 * The output normal vector is not normalized.
 */
void SrpVectorGetPlaneNormal3f(VECTOR3F out, const VECTOR3F p1, 
                               const VECTOR3F p2, const VECTOR3F p3)
{
    VECTOR3F u, v;

    SrpVectorSubtract3f(u, p2, p1);
    SrpVectorSubtract3f(v, p3, p1);

    /* Get the normal */
    SrpVectorCrossProduct3f(out, u, v);
}

void SrpVectorGetPlaneNormal3d(VECTOR3D out, const VECTOR3D p1, 
                               const VECTOR3D p2, const VECTOR3D p3)
{
    VECTOR3D u, v;

    SrpVectorSubtract3d(u, p2, p1);
    SrpVectorSubtract3d(v, p3, p1);

    /* Get the normal */
    SrpVectorCrossProduct3d(out, u, v);
}

/*------------------------------------------------------------------------------
 * int SrpVectorIsZero2f(const VECTOR2F v)
 * int SrpVectorIsZero2d(const VECTOR2D v)
 * int SrpVectorIsZero3f(const VECTOR3F v)
 * int SrpVectorIsZero3d(const VECTOR3D v)
 *
 * Check if a vector is zero
 */
int SrpVectorIsZero2f(const VECTOR2F v)
{
    if (!SrpMathFloatIsZero(v[0]) || !SrpMathFloatIsZero(v[1]))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int SrpVectorIsZero2d(const VECTOR2D v)
{
    if (!SrpMathDoubleIsZero(v[0]) || !SrpMathDoubleIsZero(v[1]))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int SrpVectorIsZero3f(const VECTOR3F v)
{
    if (!SrpMathFloatIsZero(v[0]) || !SrpMathFloatIsZero(v[1]) || 
        !SrpMathFloatIsZero(v[2]))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int SrpVectorIsZero3d(const VECTOR3D v)
{
    if (!SrpMathDoubleIsZero(v[0]) || !SrpMathDoubleIsZero(v[1]) || 
        !SrpMathDoubleIsZero(v[2]))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*------------------------------------------------------------------------------
 * void SrpVectorPrint2f(const VECTOR2F v, const char *name)
 * void SrpVectorPrint2d(const VECTOR2D v, const char *name)
 * void SrpVectorPrint3f(const VECTOR3F v, const char *name)
 * void SrpVectorPrint3d(const VECTOR3D v, const char *name)
 * void SrpVectorPrint4f(const VECTOR4F v, const char *name)
 * void SrpVectorPrint4d(const VECTOR4D v, const char *name)
 
 * These functions print the vector v.
 */
void SrpVectorPrint2f(const VECTOR2F v, const char *name)
{
    int i;
    
    printf("Print Vector:\n\t%s = (", name);
    for (i = 0; i < 2; i++)
    {
        printf("%f,", v[i]);
    }
    printf(")\n");
}

void SrpVectorPrint2d(const VECTOR2D v, const char *name)
{
    int i;
    
    printf("Print Vector:\n\t%s = (", name);
    for (i = 0; i < 2; i++)
    {
        printf("%lf,", v[i]);
    }
    printf(")\n");
}

void SrpVectorPrint3f(const VECTOR3F v, const char *name)
{
    int i;
    
    printf("Print Vector:\n\t%s = (", name);
    for (i = 0; i < 3; i++)
    {
        printf("%f,", v[i]);
    }
    printf(")\n");
}

void SrpVectorPrint3d(const VECTOR3D v, const char *name)
{
    int i;
    
    printf("Print Vector:\n\t%s = (", name);
    for (i = 0; i < 3; i++)
    {
        printf("%lf,", v[i]);
    }
    printf(")\n");
}

void SrpVectorPrint4f(const VECTOR4F v, const char *name)
{
    int i;
    
    printf("Print Vector:\n\t%s = (", name);
    for (i = 0; i < 4; i++)
    {
        printf("%f,", v[i]);
    }
    printf(")\n");
}

void SrpVectorPrint4d(const VECTOR4D v, const char *name)
{
    int i;
    
    printf("Print Vector:\n\t%s = (", name);
    for (i = 0; i < 4; i++)
    {
        printf("%lf,", v[i]);
    }
    printf(")\n");
}
