/*******************************************************************************
 * File   : math_srp.h
 * Content: Math functions
 *
 * Coder  : Ean Jee
 * Time   : 2011-04-18 17:26
 ******************************************************************************/

#ifndef _MATH_SRP_H
#define _MATH_SRP_H

#include <math.h>

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

/* defines for small numbers */
#define EPSILON_E3F (float)(1.0E-3)
#define EPSILON_E4F (float)(1.0E-4)
#define EPSILON_E5F (float)(1.0E-5)
#define EPSILON_E6F (float)(1.0E-6)

#define EPSILON_E3D (1.0E-3)
#define EPSILON_E4D (1.0E-4)
#define EPSILON_E5D (1.0E-5)
#define EPSILON_E6D (1.0E-6)

/* Radians and degrees */
#define SRP_PIF          3.141592654f
#define SRP_2PIF         6.283185307f
#define SRP_PI_DIV_2F    1.570796327f
#define SRP_PI_DIV_4F    0.785398163f
#define SRP_PI_DIV_180F  0.017453292f
#define SRP_180_DIV_PIF 57.295779513f

#define SRP_PID          3.141592654
#define SRP_2PID         6.283185307
#define SRP_PI_DIV_2D    1.570796327
#define SRP_PI_DIV_4D    0.785398163
#define SRP_PI_DIV_180D  0.017453292
#define SRP_180_DIV_PID 57.295779513

#define SrpMathDegToRadf(deg) ((deg) * SRP_PI_DIV_180F)
#define SrpMathDegToRadd(deg) ((deg) * SRP_PI_DIV_180D)

#define SrpMathRadToDegf(rad) ((rad) * SRP_180_DIV_PIF)
#define SrpMathRadToDegd(rad) ((rad) * SRP_180_DIV_PID)

#define SrpMathMax(a, b) (a > b ? a : b)
#define SrpMathMin(a, b) (a < b ? a : b)

#define SrpMathFloatIsZero(a)  (fabsf(a) < EPSILON_E5F)
#define SrpMathDoubleIsZero(a) (fabs(a) < EPSILON_E5D)

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/


#endif /* _MATH_SRP_H */
