/*******************************************************************************
 * File   : rcmanager_srp.h
 * Content: Redering context manager
 *
 * Coder  : Ean Jee
 * Time   : 2011-02-14 11:42
 ******************************************************************************/

#ifndef _RCMANAGER_SRP_H
#define _RCMANAGER_SRP_H

#include "vector_srp.h"
#include "matrix_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

extern int SrpCreateRC(int width, int height, int bit);
extern void SrpDeleteRC(void);

/* 
 * RC gets and sets.
 */

extern int SrpRCGetWidth(void);
extern int SrpRCGetHeight(void);
extern int SrpRCGetBit(void);
extern int SrpRCGetPitch(void);
extern int SrpRCGetSize(void);
extern const unsigned char* SrpRCGetBuffer(void);
extern float SrpRCGetFovy(void);
extern float SrpRCGetAspect(void);
extern MATRIX43F* SrpRCGetModelView(void);

extern void SrpRCSetWidth(int width);
extern void SrpRCSetHeight(int height);
extern void SrpRCSetFrustum(float fovy, float aspect, float near, float far);

extern void SrpRCPrintMatrix(void);
extern void SrpRCPrintStack(int depth);

/*
 * Explicitly set the current drawing color in rendering context
 * with the given rgba.
 */
extern void SrpRCSetDrawColor(int red, int green, int blue, int alpha);

/*
 * Explicitly set the current clearing color in rendering context
 * with the given rgba.
 */
extern void SrpRCSetClearColor(int red, int green, int blue, int alpha);

/*
 * Sets the color at (sg_pRC->buffer + offset)
 */
extern void SrpRCSetPixel(size_t offset);

/* 
 * Clear the buffer with current clearing color
 */
extern void SrpRCClear(void);

/* 
 * Multiply current matrix with the incoming matrix.
 */
extern void SrpRCCombineMatrix43f(const MATRIX43F m);

/* 
 * Specifie which matrix is the current matrix.
 */
extern void SrpRCSetMatrixMode(int mode);

/* 
 * Pushes the current matrix stack down by one, duplicating 
 * the current matrix.
 */
extern void SrpRCPushMatrix(void);

/* 
 * Pops the current matrix stack, replacing the current matrix 
 * with the one below it on the stack.
 */
extern void SrpRCPopMatrix(void);

/* 
 * Load the current matrix with identity matrix.
 */
extern void SrpRCLoadIdentity(void);

/*
 * Check if an object is visible with current frustum in RC.
 * The object is define by its postion and radius.
 */
extern int SrpRCIsVisible(const VECTOR3F pos, float radius);

/* 
 * Enable a specific capability.
 */
extern void SrpRCEnable(int cap);

/* 
 * Disable a specific capability.
 */
extern void SrpRCDisable(int cap);

/* 
 * Check if a required capability is enabled.
 */
extern int SrpRCIsEnabled(int cap);

#endif /* _RCMANAGER_SRP_H */
