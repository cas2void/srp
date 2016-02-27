/*******************************************************************************
 * File   : model_srp.h
 * Content: Model manipulation function
 *
 * Coder  : Ean Jee
 * Time   : 2011-10-03 17:46
 ******************************************************************************/

#ifndef _MODEL_SRP_H
#define _MODEL_SRP_H

#include "vector_srp.h"
#include "renderee_srp.h"

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

/*
 * A model(triangle mesh) based on a vertex list and a list of triangles
 */
struct MODEL_T;
typedef struct MODEL_T MODEL;

/*
 * An object based on a vertex list and a list of triangles
 */
struct OBJECT_T;
typedef struct OBJECT_T OBJECT;

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

/* 
 * Create object.
 */
extern int SrpCreateObject(OBJECT **ppObj, MODEL *pModel, 
                           float positionX, float positionY, float positionZ, 
                           float directionX, float directionY, float directionZ,
                           float scaleX, float scaleY, float scaleZ);

/*
 * Delete object.
 */
extern void SrpDeleteObject(OBJECT *pObj);

/* 
 * Translate Object. 
 * x, y and z are translation on Cardinal X, Y and Z respectively.
 */
extern void SrpTranslateObject(OBJECT *pObj, float x, float y, float z);

/* 
 * Rotate Object.
 * x, y and z are rotation on Cardinal X, Y and Z respectively.
 */
extern void SrpRotateObject(OBJECT *pObj, float x, float y, float z);

/* 
 * Scale Object.
 * x, y and z are scale on Cardinal X, Y and Z respectively.
 */
extern void SrpScaleObject(OBJECT *pObj, float x, float y, float z);

/* 
 * Draw Object.
 */
extern void SrpDrawObject(OBJECT *pObj, RENDER_LIST *pRl);

/*
 * Print the object.
 */
extern void SrpPrintObject(const OBJECT *pObj);

/* 
 * Load model from PLG file.
 */
extern int SrpModelLoadPLG(MODEL **ppModel, const char *fileName);

/* 
 * Release model.
 */
extern void SrpModelRelease(MODEL *pModel);

/* 
 * Get model's radius;
 */
float SrpModelGetRadius(const MODEL *pModel);

#endif /* _MODEL_SRP_H */
