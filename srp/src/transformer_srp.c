/*******************************************************************************
 * File   : transformer_srp.c
 * Content: Transformation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-05-05 17:21
 ******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "assert_ig.h"
#include "datadef_ig.h"
#include "transformer_srp.h"
#include "vector_srp.h"
#include "matrix_srp.h"
#include "math_srp.h"
#include "datadef_srp.h"
#include "rcmanager_srp.h"

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
 * void SrpTransformerSetTranslationf(const VECTOR3F v)
 * void SrpTransformerSetTranslationd(const VECTOR3D v)
 *
 * These functions combine the current matrix in RC 
 * with a 'pure' translation matrix for 'object' transformation 
 * from local to parent space.
 *
 * Parent space translates vector v to object space.
 */
void SrpTransformerSetTranslationf(const VECTOR3F v)
{
    MATRIX43F translation;

    SrpMatrixMakeTranslation43f(translation, v);
    SrpRCCombineMatrix43f(translation);
}

void SrpTransformerSetTranslationd(const VECTOR3D v)
{
    MATRIX43D translation;
    MATRIX43F fTrans;
    int i;

    SrpMatrixMakeTranslation43d(translation, v);

    for (i = 0; i < sizeof(MATRIX43F) / sizeof(float); i++)
    {
        fTrans[i] = (float)translation[i];
    }

    SrpRCCombineMatrix43f(fTrans);
}

/*------------------------------------------------------------------------------
 * void SrpTransformerSetRotationf(const VECTOR3F v, float t)
 * void SrpTransformerSetRotationd(const VECTOR3D v, double t)
 *
 * These functions combine the current matrix in RC 
 * with a 'pure' rotation matrix for 'object' transformation 
 * from local to parent space.
 *
 * Parent space rotates angle t about an arbitrary axis to object space.
 * This axis is defined by origin and vector v in parent space.
 *
 * t is the amount of rotation in degree.
 */
void SrpTransformerSetRotationf(const VECTOR3F v, float t)
{
    MATRIX43F rotation;
    float radian;
    
    radian = SrpMathDegToRadf(t);
    SrpMatrixMakeRotation43f(rotation, v, radian);
    SrpRCCombineMatrix43f(rotation);
}

void SrpTransformerSetRotationd(const VECTOR3D v,double t)
{
    MATRIX43D rotation;
    MATRIX43F fRot;
    int i;
    double radian;

    radian = SrpMathDegToRadd(t);
    SrpMatrixMakeRotation43d(rotation, v, radian);

    for (i = 0; i < sizeof(MATRIX43F) / sizeof(float); i++)
    {
        fRot[i] = (float)rotation[i];
    }

    SrpRCCombineMatrix43f(fRot);
}

/*------------------------------------------------------------------------------
 * void SrpTransformerSetScalef(const VECTOR3F v)
 * void SrpTransformerSetScaled(const VECTOR3D v)
 *
 * These functions combine the current matrix in RC 
 * with a scale matrix for 'object' transformation from local 
 * to parent space.
 *
 * v[0], v[1] and v[2] define the scale factors along cardinal 
 * axis x, y and z, respectively.
 *
 * Parent space scales each factors along each axis to object space.
 */
void SrpTransformerSetScalef(const VECTOR3F v)
{
    MATRIX43F scale;

    SrpMatrixMakeScale43f(scale, v);
    SrpRCCombineMatrix43f(scale);
}

void SrpTransformerSetScaled(const VECTOR3D v)
{
    MATRIX43D scale;
    MATRIX43F fScale;
    int i;
    
    SrpMatrixMakeScale43d(scale, v);

    for (i = 0; i < sizeof(MATRIX43F) / sizeof(float); i++)
    {
        fScale[i] = (float)scale[i];
    }

    SrpRCCombineMatrix43f(fScale);
}

/*------------------------------------------------------------------------------
 * void SrpTransformerSetLookAt(const VECTOR3F eye, const VECTOR3F center, 
 *                              const VECTOR3F up)
 *
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
void SrpTransformerSetLookAt(const VECTOR3F eye, const VECTOR3F center, 
                             const VECTOR3F up)
{
    /* u: right vector, v: up vector, n: lookat vector(inversed) */
    VECTOR3F u, v, n;
    VECTOR3F uNormal, vNormal, nNormal;
    MATRIX43F w2c;

    SrpVectorCopy3f(v, up);

    /* We are using right-handed rule, the positive z is pointing 
     * to the viewer, so n is computed as eye - center.
     */
    SrpVectorSubtract3f(n, eye, center);

    /* Right vector is the cross product of v and n */
    SrpVectorCrossProduct3f(u, v, n);

    /* If v and n are parallel to each other, leave the model view
     * matrix untouched.
     */
    if (fabsf(SrpVectorLength3f(u)) < EPSILON_E6F)
    {
        ASSERTMSG(FALSE, "SrpTransformerSetLookAt: lookat vector \
and up vector are parallel to each other.");
        return;
    }

    /* Recalculate v as v may not be othogonal with n previously */
    SrpVectorCrossProduct3f(v, n, u);

    /* Normalize u, v and n */
    SrpVectorNormalize3f(uNormal, u);
    SrpVectorNormalize3f(vNormal, v);
    SrpVectorNormalize3f(nNormal, n);

    /* Now uNormal, vNormal and nNormal are the basis of camera
     * space presented in world space. But we need to find the
     * world-to-camera transformation matrix for object, so we need
     * to get the world space basis presented in camera space.
     *
     * From now on in this comment, use u, v and n as acronym 
     * for uNormal, vNormal and nNormal, respectively.
     *
     * The world space is transformed to camera space by TR, 
     * T = | 0   0 |   R = | u 0 |
     *     | eye 1 |,      | v 0 |
     *                     | n 0 |
     *                     | 0 1 |,
     * so the world-to-camera transformation matrix for object is 
     *    Inv(T)*Inv(R)
     * 
     *    |   1      0      0    0 |   | u.x v.x n.x 0 |
     * =  |   0      1      0    0 |   | u.y v.y n.y 0 |
     *    |   0      0      1    0 | * | u.z v.z n.z 0 |
     *    | -eye.x -eye.y -eye.z 1 |   |  0   0   0  1 |
     * 
     *    |   u.x    v.x    n.x  0 |
     * =  |   u.y    v.y    n.y  0 |
     *    |   u.z    v.z    n.z  0 |  ( eye*u is the dot product of eye and u)
     *    | -eye*u -eye*v -eye*n 1 | 
     */
    w2c[0] = uNormal[0];
    w2c[1] = vNormal[0];
    w2c[2] = nNormal[0];

    w2c[3] = uNormal[1];
    w2c[4] = vNormal[1];
    w2c[5] = nNormal[1];

    w2c[6] = uNormal[2];
    w2c[7] = vNormal[2];
    w2c[8] = nNormal[2];

    w2c[9]  = -1 * SrpVectorDotProduct3f(eye, uNormal);
    w2c[10] = -1 * SrpVectorDotProduct3f(eye, vNormal);
    w2c[11] = -1 * SrpVectorDotProduct3f(eye, nNormal);

    /* Combine the world-to-camera matrix with the model view
     * matrix in RC.
     */
    SrpRCSetMatrixMode(SRP_MODELVIEW);
    SrpRCCombineMatrix43f(w2c);
}

/*------------------------------------------------------------------------------
 * void SrpTransformerSetProjection(float fovy, float aspect,
 *                                  float near, float far)
 *
 * Setup a camera-to-projection matrix, using normalized projection
 * space. The matrix is
 * | 1/tan    0   0  0 |
 * |   0   ar/tan 0  0 |
 * |   0      0   1  1 |
 * |   0      0   0  0 |
 *
 * Combine the current projection matrix in RC with that matrix.
 */
void SrpTransformerSetProjection(float fovy, float aspect,
                                 float near, float far)
{
    float oneOverTanTheta;
    MATRIX43F proj;

    ASSERTMSG(fovy > 0.0f && fovy < 180.0f &&
              aspect > 0.0f && near < -1.0f && far < near,
              "SrpTransformerSetProjection: invalid arguments.");

    oneOverTanTheta = 1.0f / tanf(SrpMathDegToRadf(fovy / 2.0f));

    proj[0] = oneOverTanTheta;
    proj[1] = 0.0f;
    proj[2] = 0.0f;
    
    proj[3] = 0.0f;
    proj[4] = aspect * oneOverTanTheta;
    proj[5] = 0.0f;

    proj[6] = 0.0f;
    proj[7] = 0.0f;
    proj[8] = 1.0f;

    proj[9] = proj[10] = proj[11] = 0.0f;

    /* Combine the projection matrix with the current projection
     * matrix in RC.
     */
    SrpRCSetMatrixMode(SRP_PROJECTION);
    SrpRCCombineMatrix43f(proj);

    SrpRCSetFrustum(fovy, aspect, near, far);
}

/*------------------------------------------------------------------------------
 * void SrpTransformerSetViewport(float width, float height)
 *
 * Setup a projection-to-screen matrix, replace the current
 * viewport transformation matrix in RC with that matrix.
 * The matrix is
 * | A  0 0 0 |
 * | 0 -B 0 0 |
 * | A  B 1 0 |
 * | 0  0 0 1 |
 *
 * A = (width - 1.0f) / 2.0f, B = (height - 1.0f) / 2.0f
 */
void SrpTransformerSetViewport(float width, float height)
{
    MATRIX43F viewport;
    float alpha;
    float beta;

    ASSERTMSG(width > 0 && height > 0, 
              "SrpTransformerSetViewport: invalid arguments.");

    alpha = (width - 1.0f) / 2.0f;
    beta  = (height - 1.0f) / 2.0f;

    viewport[0]  = alpha;
    viewport[1]  = 0.0f;
    viewport[2]  = 0.0f;

    viewport[3]  = 0.0f;
    viewport[4]  = -beta;
    viewport[5]  = 0.0f;

    viewport[6]  = alpha;
    viewport[7]  = beta;
    viewport[8]  = 1.0f;

    viewport[9]  = 0.0f;
    viewport[10] = 0.0f;
    viewport[11] = 0.0f;

    SrpRCSetMatrixMode(SRP_VIEWPORT);
    SrpRCCombineMatrix43f(viewport);

    SrpRCSetWidth(width);
    SrpRCSetHeight(height);
}
