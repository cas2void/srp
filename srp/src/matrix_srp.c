/*******************************************************************************
 * File   : matrix_srp.c
 * Content: Matrix function. For now, just 4X3 matrix.
 *
 * Coder  : Ean Jee
 * Time   : 2011-04-20 16:27
 ******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <math.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "math_srp.h"
#include "matrix_srp.h"

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
 * void SrpMatrixLoadIdentity43f(MATRIX43F m)
 * void SrpMatrixLoadIdentity43d(MATRIX43D m)
 *
 * These functions load identity matrix into the argument. 
 */
void SrpMatrixLoadIdentity43f(MATRIX43F m)
{
    static MATRIX43F s_identity = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f
    };

    memcpy(m, s_identity, sizeof(MATRIX43F));
}

void SrpMatrixLoadIdentity43d(MATRIX43D m)
{
    static MATRIX43D s_identity = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 0.0
    };

    memcpy(m, s_identity, sizeof(MATRIX43D));
}

/*------------------------------------------------------------------------------
 * void SrpMatrixCopy43f(MATRIX43F dst, const MATRIX43F src)
 * void SrpMatrixCopy43d(MATRIX43D dst, const MATRIX43D src)
 *
 * These functions copy matrix src into dst.
 */
void SrpMatrixCopy43f(MATRIX43F dst, const MATRIX43F src)
{
    memcpy(dst, src, sizeof(MATRIX43F));
}

void SrpMatrixCopy43d(MATRIX43D dst, const MATRIX43D src)
{
    memcpy(dst, src, sizeof(MATRIX43D));
}

/*------------------------------------------------------------------------------
 * void SrpMatrixMultiply43f(MATRIX43F out, const MATRIX43F a,const MATRIX43F b)
 * void SrpMatrixMultiply43d(MATRIX43D out, const MATRIX43D a,const MATRIX43D b)
 *
 * These functions do 4X3 matrix multiplication. out = a * b.
 * Each of those 4X3 matrices has a invisible 4th column [0, 0, 0, 1]T.
 *
 * | 0  1  2 '0' |     | 0  1  2 '0' |
 * | 3  4  5 '0' |  X  | 3  4  5 '0' |
 * | 6  7  8 '0' |     | 6  7  8 '0' |
 * | 9 10 11 '1' |     | 9 10 11 '1' |
 */
void SrpMatrixMultiply43f(MATRIX43F out, const MATRIX43F a, const MATRIX43F b)
{
    /* The linear transformation portion is the same as 3X3 Matrix. */
    out[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
    out[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
    out[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];

    out[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
    out[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
    out[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];

    out[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
    out[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
    out[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];

    /* Now the translation portion. */
    out[9]  = a[9] * b[0] + a[10] * b[3] + a[11] * b[6] + b[9];
    out[10] = a[9] * b[1] + a[10] * b[4] + a[11] * b[7] + b[10];
    out[11] = a[9] * b[2] + a[10] * b[5] + a[11] * b[8] + b[11];
}

void SrpMatrixMultiply43d(MATRIX43D out, const MATRIX43D a, const MATRIX43D b)
{
    /* The linear transformation portion is the same as 3X3 Matrix. */
    out[0] = a[0] * b[0] + a[1] * b[3] + a[2] * b[6];
    out[1] = a[0] * b[1] + a[1] * b[4] + a[2] * b[7];
    out[2] = a[0] * b[2] + a[1] * b[5] + a[2] * b[8];

    out[3] = a[3] * b[0] + a[4] * b[3] + a[5] * b[6];
    out[4] = a[3] * b[1] + a[4] * b[4] + a[5] * b[7];
    out[5] = a[3] * b[2] + a[4] * b[5] + a[5] * b[8];

    out[6] = a[6] * b[0] + a[7] * b[3] + a[8] * b[6];
    out[7] = a[6] * b[1] + a[7] * b[4] + a[8] * b[7];
    out[8] = a[6] * b[2] + a[7] * b[5] + a[8] * b[8];

    /* Now the translation portion. */
    out[9]  = a[9] * b[0] + a[10] * b[3] + a[11] * b[6] + b[9];
    out[10] = a[9] * b[1] + a[10] * b[4] + a[11] * b[7] + b[10];
    out[11] = a[9] * b[2] + a[10] * b[5] + a[11] * b[8] + b[11];
}

/*------------------------------------------------------------------------------
 * int SrpMatrixInvert43f(MATRIX43F out, const MATRIX43F m)
 * int SrpMatrixInvert43d(MATRIX43D out, const MATRIX43D m)
 *
 * These functions compute the inverse of 4X3 general matrix m.
 * The 4X3(actually 4X4) tranformation matrix F can be seen as the 
 * combination of rotation matrix R and tranlation T, as F = R * T, that is 
 *
 * | R 0 |  =  | R 0 |  X  | I 0 |
 * | v 1 |     | 0 1 |     | v 1 |
 *
 * Then we can calculate the inverse of transformation matrix F as follow.
 * 
 * Inv(F) = Inv(R * T) = Inv(T) * Inv(R), that is
 *
 * Inv| I 0 |  X  Inv| R 0 |  =  | I 0 |  X  | Inv(R) 0 |  =  |     Inv(R) 0 |
 *    | v 1 |        | 0 1 |     |-v 1 |     |     0  1 |     |-v * Inv(R) 1 |
 *
 * Return Value:
 *     FALSE: the inverse of m does not exist.
 *     TRUE : the inverse of m has been stored in matrix out.
 */
int SrpMatrixInvert43f(MATRIX43F out, const MATRIX43F m)
{
    float det, oneDivDet;

    /* The linear transformation portion is the same as 3X3 Matrix. */
    det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
          m[1] * (m[3] * m[8] - m[5] * m[6]) +
          m[2] * (m[3] * m[7] - m[4] * m[6]);

    /* The inverse does not exist. */
    if (fabsf(det) < EPSILON_E6F)
    {
        return FALSE;
    }

    oneDivDet = 1.0f / det;

    /* Divide the adjoint by the determinant. */
    out[0] =  (m[4] * m[8] - m[5] * m[7]) * oneDivDet;
    out[1] = -(m[1] * m[8] - m[2] * m[7]) * oneDivDet;
    out[2] =  (m[1] * m[5] - m[2] * m[4]) * oneDivDet;

    out[3] = -(m[3] * m[8] - m[5] * m[6]) * oneDivDet;
    out[4] =  (m[0] * m[8] - m[2] * m[6]) * oneDivDet;
    out[5] = -(m[0] * m[5] - m[2] * m[3]) * oneDivDet;

    out[6] =  (m[3] * m[7] - m[4] * m[6]) * oneDivDet;
    out[7] = -(m[0] * m[7] - m[1] * m[6]) * oneDivDet;
    out[8] =  (m[0] * m[4] - m[1] * m[3]) * oneDivDet;

    /* Now the translation portion --> -v * Inv(R) */
    out[9]  = -(m[9] * out[0] + m[10] * out[3] + m[11] * out[6]);
    out[10] = -(m[9] * out[1] + m[10] * out[4] + m[11] * out[7]);
    out[11] = -(m[9] * out[2] + m[10] * out[5] + m[11] * out[8]);

    return TRUE;
}

int SrpMatrixInvert43d(MATRIX43D out, const MATRIX43D m)
{
    double det, oneDivDet;

    /* The linear transformation portion is the same as 3X3 Matrix. */
    det = m[0] * (m[4] * m[8] - m[5] * m[7]) -
          m[1] * (m[3] * m[8] - m[5] * m[6]) +
          m[2] * (m[3] * m[7] - m[4] * m[6]);

    /* The inverse does not exist. */
    if (fabs(det) < EPSILON_E6D)
    {
        return FALSE;
    }

    oneDivDet = 1.0 / det;

    /* Divide the adjoint by the determinant. */
    out[0] =  (m[4] * m[8] - m[5] * m[7]) * oneDivDet;
    out[1] = -(m[1] * m[8] - m[2] * m[7]) * oneDivDet;
    out[2] =  (m[1] * m[5] - m[2] * m[4]) * oneDivDet;

    out[3] = -(m[3] * m[8] - m[5] * m[6]) * oneDivDet;
    out[4] =  (m[0] * m[8] - m[2] * m[6]) * oneDivDet;
    out[5] = -(m[0] * m[5] - m[2] * m[3]) * oneDivDet;

    out[6] =  (m[3] * m[7] - m[4] * m[6]) * oneDivDet;
    out[7] = -(m[0] * m[7] - m[1] * m[6]) * oneDivDet;
    out[8] =  (m[0] * m[4] - m[1] * m[3]) * oneDivDet;

    /* Now the translation portion --> -v * Inv(R) */
    out[9]  = -(m[9] * out[0] + m[10] * out[3] + m[11] * out[6]);
    out[10] = -(m[9] * out[1] + m[10] * out[4] + m[11] * out[7]);
    out[11] = -(m[9] * out[2] + m[10] * out[5] + m[11] * out[8]);

    return TRUE;
}

/*------------------------------------------------------------------------------
 * void SrpMatrixMakeTranslation43f(MATRIX43F out, const VECTOR3F v)
 * void SrpMatrixMakeTranslation43d(MATRIX43D out, const VECTOR3D v)
 *
 * These functions make a 'pure' translation matrix for 'object' 
 * transformation from local to parent space.
 *
 * Parent space translates vector v to object space.
 */
void SrpMatrixMakeTranslation43f(MATRIX43F out, const VECTOR3F v)
{
    /* Set the linear transformation portion to identity. */
    out[0] = 1.0f; out[1] = 0.0f; out[2] = 0.0f;
    out[3] = 0.0f; out[4] = 1.0f; out[5] = 0.0f;
    out[6] = 0.0f; out[7] = 0.0f; out[8] = 1.0f;

    /* Set the translation portion. */
    out[9] = v[0]; out[10] = v[1]; out[11] = v[2];
}

void SrpMatrixMakeTranslation43d(MATRIX43D out, const VECTOR3D v)
{
    /* Set the linear transformation portion to identity. */
    out[0] = 1.0; out[1] = 0.0; out[2] = 0.0;
    out[3] = 0.0; out[4] = 1.0; out[5] = 0.0;
    out[6] = 0.0; out[7] = 0.0; out[8] = 1.0;

    /* Set the translation portion. */
    out[9] = v[0]; out[10] = v[1]; out[11] = v[2];
}

/*------------------------------------------------------------------------------
 * void SrpMatrixMakeRotation43f(MATRIX43F out, const VECTOR3F v, float t)
 * void SrpMatrixMakeRotation43d(MATRIX43D out, const VECTOR3D v,double t)
 *
 * These functions make a 'pure' rotation matrix for 'object' 
 * transformation from local to parent space.
 *
 * Parent space rotates angle t about an arbitrary axis to object space.
 * This axis is defined by origin and vector v in parent space.
 *
 * t is the amount of rotation in radians.
 */
void SrpMatrixMakeRotation43f(MATRIX43F out, const VECTOR3F v, float t)
{
    /* Normalized v */
    VECTOR3F vNormal;

    /* v's length */
    float length, oneDivLength;

    /* sin(t), cos(t), 1 - cos(t) */
    float s, c, n;

    /* Multiplication of vNormal's members */
    float xx, yy, zz, xy, xz, yz;

    /* Multiplication of vNormal's members with sin(t) */
    float xs, ys, zs;

    /* If v is zero vector, load identity matrix in matrix out and return. */
    length = SrpVectorLength3f(v);
    if (length < EPSILON_E6F)
    {
        ASSERTMSG(FALSE, 
                  "SrpMatrixMakeRotation43f: rotation axis is zero vector.");
        SrpMatrixLoadIdentity43f(out);
        return;
    }
    
    oneDivLength = 1.0f / length;
    vNormal[0] = v[0] * oneDivLength;
    vNormal[1] = v[1] * oneDivLength;
    vNormal[2] = v[2] * oneDivLength;

    /* Calculate those intermidiary variables to save time. */
    s = sinf(t);
    c = cosf(t);
    n = 1.0f - c;

    xx = vNormal[0] * vNormal[0];
    yy = vNormal[1] * vNormal[1];
    zz = vNormal[2] * vNormal[2];

    xy = vNormal[0] * vNormal[1];
    xz = vNormal[0] * vNormal[2];
    yz = vNormal[1] * vNormal[2];

    xs = vNormal[0] * s;
    ys = vNormal[1] * s;
    zs = vNormal[2] * s;

    /* Now fill in the linear transformation portion. */
    out[0] = xx * n +  c;  out[1] = xy * n + zs;  out[2] = xz * n - ys;
    out[3] = xy * n - zs;  out[4] = yy * n +  c;  out[5] = yz * n + xs;
    out[6] = xz * n + ys;  out[7] = yz * n - xs;  out[8] = zz * n +  c;

    /* Zero the translation portion. */
    out[9] = 0.0f;        out[10] = 0.0f;        out[11] = 0.0f;
}

void SrpMatrixMakeRotation43d(MATRIX43D out, const VECTOR3D v,double t)
{
    /* Normalized v */
    VECTOR3D vNormal;

    /* v's length */
    double length, oneDivLength;

    /* sin(t), cos(t), 1 - cos(t) */
    double s, c, n;

    /* Multiplication of vNormal's members */
    double xx, yy, zz, xy, xz, yz;

    /* Multiplication of vNormal's members with sin(t) */
    double xs, ys, zs;

    /* If v is zero vector, load identity matrix in matrix out and return. */
    length = SrpVectorLength3d(v);
    if (length < EPSILON_E6D)
    {
        ASSERTMSG(FALSE, 
                  "SrpMatrixMakeRotation43d: rotation axis is zero vector.");
        SrpMatrixLoadIdentity43d(out);
        return;
    }
    
    oneDivLength = 1.0 / length;
    vNormal[0] = v[0] * oneDivLength;
    vNormal[1] = v[1] * oneDivLength;
    vNormal[2] = v[2] * oneDivLength;

    /* Calculate those intermidiary variables to save time. */
    s = sin(t);
    c = cos(t);
    n = 1.0 - c;

    xx = vNormal[0] * vNormal[0];
    yy = vNormal[1] * vNormal[1];
    zz = vNormal[2] * vNormal[2];

    xy = vNormal[0] * vNormal[1];
    xz = vNormal[0] * vNormal[2];
    yz = vNormal[1] * vNormal[2];

    xs = vNormal[0] * s;
    ys = vNormal[1] * s;
    zs = vNormal[2] * s;

    /* Now fill in the linear transformation portion. */
    out[0] = xx * n +  c;  out[1] = xy * n + zs;  out[2] = xz * n - ys;
    out[3] = xy * n - zs;  out[4] = yy * n +  c;  out[5] = yz * n + xs;
    out[6] = xz * n + ys;  out[7] = yz * n - xs;  out[8] = zz * n +  c;

    /* Zero the translation portion. */
    out[9] = 0.0;         out[10] = 0.0;         out[11] = 0.0;
}

/*------------------------------------------------------------------------------
 * void SrpMatrixMakeScale43f(MATRIX43F out, const VECTOR3F v)
 * void SrpMatrixMakeScale43d(MATRIX43D out, const VECTOR3D v)

 * These functions make a scale transformation matrix for 'object' 
 * transformation from local to parent space.
 *
 * v[0], v[1] and v[2] define the scale factors along cardinal 
 * axis x, y and z, respectively.
 *
 * Parent space scales each factors along each axis to object space.
 */
void SrpMatrixMakeScale43f(MATRIX43F out, const VECTOR3F v)
{
    out[0] = v[0]; out[1]  = 0.0f; out[2]  = 0.0f;
    out[3] = 0.0f; out[4]  = v[1]; out[5]  = 0.0f;
    out[6] = 0.0f; out[7]  = 0.0f; out[8]  = v[2];
    out[9] = 0.0f; out[10] = 0.0f; out[11] = 0.0f;
}

void SrpMatrixMakeScale43d(MATRIX43D out, const VECTOR3D v)
{
    out[0] = v[0]; out[1]  = 0.0;  out[2]  = 0.0;
    out[3] = 0.0;  out[4]  = v[1]; out[5]  = 0.0;
    out[6] = 0.0;  out[7]  = 0.0;  out[8]  = v[2];
    out[9] = 0.0;  out[10] = 0.0;  out[11] = 0.0;
}

/*------------------------------------------------------------------------------
 * void SrpMatrixTransformVector3f(VECTOR3F out, const VECTOR3F v, 
                                   const MATRIX43F m)
 * void SrpMatrixTransformVector3d(VECTOR3D out, const VECTOR3D v, 
                                   const MATRIX43D m)
 * void SrpMatrixTransformVector4f(VECTOR4F out, const VECTOR4F v, 
                                   const MATRIX43F m)
 * void SrpMatrixTransformVector4d(VECTOR4D out, const VECTOR4D v, 
                                   const MATRIX43D m)
 *
 * These functions transform vector with 4X3 matrix m,
 * and store the result in vector out.
 */
void SrpMatrixTransformVector3f(VECTOR3F out, const VECTOR3F v, 
                                const MATRIX43F m)
{
    out[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6] + m[9];
    out[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7] + m[10];
    out[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8] + m[11];
}

void SrpMatrixTransformVector3d(VECTOR3D out, const VECTOR3D v, 
                                const MATRIX43D m)
{
    out[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6] + m[9];
    out[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7] + m[10];
    out[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8] + m[11];
}

void SrpMatrixTransformVector4f(VECTOR4F out, const VECTOR4F v, 
                                const MATRIX43F m)
{
    out[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6] + v[3] * m[9];
    out[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7] + v[3] * m[10];
    out[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8] + v[3] * m[11];
    out[3] = v[3];
}

void SrpMatrixTransformVector4d(VECTOR4D out, const VECTOR4D v, 
                                const MATRIX43D m)
{
    out[0] = v[0] * m[0] + v[1] * m[3] + v[2] * m[6] + v[3] * m[9];
    out[1] = v[0] * m[1] + v[1] * m[4] + v[2] * m[7] + v[3] * m[10];
    out[2] = v[0] * m[2] + v[1] * m[5] + v[2] * m[8] + v[3] * m[11];
    out[3] = v[3];
}

/*------------------------------------------------------------------------------
 * void SrpMatrixPrint43f(const MATRIX43F m, const char *name)
 * void SrpMatrixPrint43d(const MATRIX43D m, const char *name)
 *
 * These functions print the matrix m.
 * The element at row i and column j is m[i * 3 + j].
 */
void SrpMatrixPrint43f(const MATRIX43F m, const char *name)
{
    int i, j;

    printf("Print Matrix:\n\t%s =\n", name);

    for (i = 0; i < 4; i++)
    {
        printf("\t| ");
        for (j = 0; j < 3; j++)
        {
            printf("%12f ", m[i * 3 + j]);
        }
        printf("|\n");
    }
}

void SrpMatrixPrint43d(const MATRIX43D m, const char *name)
{
    int i, j;

    printf("Print Matrix:\n\t%s =\n", name);

    for (i = 0; i < 4; i++)
    {
        printf("\t| ");
        for (j = 0; j < 3; j++)
        {
            printf("%12lf ", m[i * 3 + j]);
        }
        printf("|\n");
    }
}
