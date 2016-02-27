/*******************************************************************************
 * File   : raster_srp.h
 * Content: Rasterization function
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-28 17:16
 ******************************************************************************/

#ifndef _RASTER_SRP_H
#define _RASTER_SRP_H

/*----------------------------------------------------------------------------*/
/*                         Macros and Type Definitions                        */
/*----------------------------------------------------------------------------*/

typedef struct tagPOINT2I
{
    int x;
    int y;
} POINT2I;

/*----------------------------------------------------------------------------*/
/*                           Function Declarations                            */
/*----------------------------------------------------------------------------*/

extern void SrpDrawPixel(int x, int y);
extern void SrpDrawLine(int x0, int y0, int x1, int y1);

/* 
 * Note: 
 * 1. SrpDrawPolygonWire is 2~5 ticks faster than SrpDrawTriangleWire.
 * 2. SrpDrawPolygonFill is 26~30 ticks faster than SrpDrawTriangleFill
 *    in debug version, and 10~15 ticks faster in non-debug version.
 *
 * SrpDrawPolygonWire and SrpDrawPolygonFill are recommended.
 */
extern void SrpDrawTriangleWire(int x0, int y0, int x1, int y1, int x2, int y2);
extern void SrpDrawTriangleFill(int x0, int y0, int x1, int y1, int x2, int y2);
extern void SrpDrawPolygonWire(const POINT2I *pBuffer, int count);
extern void SrpDrawPolygonFill(const POINT2I *pBuffer, int count);

#endif /* _RASTER_SRP_H */
