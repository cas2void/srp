/*******************************************************************************
 * File   : raster_srp.c
 * Content: Rasterization functions.
 *
 * Coder  : Ean Jee
 * Time   : 2011-02-15 14:42
 ******************************************************************************/

#include <stdio.h>
#include "datadef_srp.h"
#include "assert_ig.h"
#include "raster_srp.h"
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

static int SrpCheckPointBuffer(const POINT2I *pBuffer, int count);
static int SrpPolygonIsHorizontalLine(const POINT2I *pBuffer, int count);
static int SrpPolygonIsVerticalLine(const POINT2I *pBuffer, int count);
static void SrpDrawHorizontalLine(int xStart, int xEnd, int y);
static void SrpDrawTriangleBottomFlat(int xTop, int yTop, 
                                      int xLeft, int yLeft, 
                                      int xRight, int yRight);
static void SrpDrawTriangleTopFlat(int xLeft, int yLeft, 
                                   int xRight, int yRight,
                                   int xBottom, int yBottom);

/*----------------------------------------------------------------------------*/
/*                             Private Functions                              */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * static int SrpCheckPointBuffer(const POINT2I *pBuffer, int count)
 *
 * This function checks if the fisrt 'count' point(s) stored in 'pBuffer'
 * is inside the clip region.
 */
static int SrpCheckPointBuffer(const POINT2I *pBuffer, int count)
{
    int i;
    int widthMinusOne, heightMinusOne;

    ASSERTMSG(pBuffer != NULL && count > 0, 
        "SrpCheckPointBuffer: invalid arguments.");
    
    widthMinusOne = SrpRCGetWidth() - 1;
    heightMinusOne = SrpRCGetHeight() - 1;

    for (i = 0; i < count; i++)
    {
        if (pBuffer[i].x < 0 ||
            pBuffer[i].x > widthMinusOne ||
            pBuffer[i].y < 0 ||
            pBuffer[i].y > heightMinusOne)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*------------------------------------------------------------------------------
 * static int SrpPolygonIsHorizontalLine(const POINT2I *pBuffer, int count)
 * static int SrpPolygonIsVerticalLine(const POINT2I *pBuffer, int count)
 *
 * Check if the fisrt 'count' point(s) stored in 'pBuffer'
 * is a horizontal or vertical line.
 */
static int SrpPolygonIsHorizontalLine(const POINT2I *pBuffer, int count)
{
    int i;

    ASSERTMSG(pBuffer != NULL && count > 0, 
        "SrpPolygonIsHorizontalLine: invalid arguments.");

    for (i = 0; i < count - 1; i++)
    {
        if (pBuffer[i].y != pBuffer[i + 1].y)
        {
            return FALSE;
        }
    }

    return TRUE;
}

static int SrpPolygonIsVerticalLine(const POINT2I *pBuffer, int count)
{
    int i;

    ASSERTMSG(pBuffer != NULL && count > 0, 
        "SrpPolygonIsVerticalLine: invalid arguments.");

    for (i = 0; i < count - 1; i++)
    {
        if (pBuffer[i].x != pBuffer[i + 1].x)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*------------------------------------------------------------------------------
 * static void SrpDrawHorizontalLine(int xStart, int xEnd, int y)
 *
 * This function draws a horizontal line from (xStart, y) to (xEnd, y),
 * both ends included.
 */
static void SrpDrawHorizontalLine(int xStart, int xEnd, int y)
{
    int xLeft, xRight;
    int i;

#ifndef NDEBUG
    POINT2I pointList[2] = {{xStart, y}, {xEnd, y}};
    int retTemp;

    retTemp = SrpCheckPointBuffer(pointList, 2);
    ASSERTMSG(retTemp, "SrpDrawHorizontalLine: invalid raster position.");
#endif
    
    if (xStart > xEnd)
    {
        xLeft = xEnd;
        xRight = xStart;
    }
    else
    {
        xLeft = xStart;
        xRight = xEnd;
    }

    for (i = xLeft; i <= xRight; i++)
    {
        SrpDrawPixel(i, y);
    }
}

/*------------------------------------------------------------------------------
 * static void SrpDrawTriangleBottomFlat(int xTop, int yTop, 
 *                                       int xLeft, int yLeft, 
 *                                       int xRight, int yRight)
 *
 * This function draws a filled bottom flat trianlge specified with 
 * (xTop, yTop), (xLeft, yLeft), (xRight, yRight).
 *
 * Note: xLeft does not need to be lesser than xRight, as 
 * SrpDrawHorizontalLine will deal with that.
 */
static void SrpDrawTriangleBottomFlat(int xTop, int yTop, 
                                      int xLeft, int yLeft, 
                                      int xRight, int yRight)
{
    /* The start and end of a horizontal line. */
    float xStart, xEnd;

    /* Current y position of a horizontal line. */
    int yPos;

    /* Not realy slope, is actually dx / dy, used to increase x. */
    float slopeLeft, slopeRight;

    POINT2I pointList[3] = {{xTop, yTop}, {xLeft, yLeft}, {xRight, yRight}};

#ifndef NDEBUG
    int retTemp;
    retTemp = SrpCheckPointBuffer(pointList, 3);
    ASSERTMSG(retTemp, "SrpDrawTriangleBottomFlat: invalid raster position.");
#endif

    ASSERTMSG(yLeft == yRight, 
              "SrpDrawTriangleBottomFlat: not a bottom flat triangle.");

    if (SrpPolygonIsHorizontalLine(pointList, 3))
    { 
        printf("Debug: the triangle is a horizontal line in \
SrpDrawTriangleBottomFlat().\n");
        return;
    }

    if (SrpPolygonIsVerticalLine(pointList, 3))
    {
        printf("Debug: the triangle is a vertical line in \
SrpDrawTriangleBottomFlat().\n");
        return;
    }

    slopeLeft = (float)(xLeft - xTop) / (float)(yLeft - yTop);
    slopeRight = (float)(xRight - xTop) / (float)(yRight - yTop);

    xStart = xTop;
    xEnd = xTop;

    for (yPos = yTop; yPos <= yLeft; yPos++)
    {
        SrpDrawHorizontalLine(ROUND2INT(xStart), ROUND2INT(xEnd), yPos);

        xStart = xStart + slopeLeft;
        xEnd = xEnd + slopeRight;
    }
}

/*------------------------------------------------------------------------------
 * static void SrpDrawTriangleTopFlat(int xLeft, int yLeft, 
 *                                    int xRight, int yRight,
 *                                    int xBottom, int yBottom)
 *
 * This function draws a filled top flat trianlge specified with 
 * (xLeft, yLeft), (xRight, yRight), (xBottom, yBottom).
 *
 * Note: xLeft does not need to be lesser than xRight, as 
 * SrpDrawHorizontalLine will deal with that.
 */
static void SrpDrawTriangleTopFlat(int xLeft, int yLeft, 
                                   int xRight, int yRight,
                                   int xBottom, int yBottom)
{
    /* The start and end of a horizontal line. */
    float xStart, xEnd;

    /* Current y position of a horizontal line. */
    int yPos;

    /* Not realy slope, is actually dx / dy, used to increase x. */
    float slopeLeft, slopeRight;

    POINT2I pointList[3] = {{xLeft, yLeft}, {xRight, yRight}, 
                            {xBottom, yBottom}};

#ifndef NDEBUG
    int retTemp;
    retTemp = SrpCheckPointBuffer(pointList, 3);
    ASSERTMSG(retTemp, "SrpDrawTriangleTopFlat: invalid raster position.");
#endif

    ASSERTMSG(yLeft == yRight, 
              "SrpDrawTriangleTopFlat: not a top flat triangle.");

    if (SrpPolygonIsHorizontalLine(pointList, 3))
    { 
        printf("Debug: the triangle is a horizontal line in \
SrpDrawTriangleTopFlat().\n");
        return;
    }

    if (SrpPolygonIsVerticalLine(pointList, 3))
    {
        printf("Debug: the triangle is a vertical line in \
SrpDrawTriangleTopFlat().\n");
        return;
    }

    slopeLeft = (float)(xBottom - xLeft) / (float)(yBottom - yLeft);
    slopeRight = (float)(xBottom - xRight) / (float)(yBottom - yRight);

    xStart = xLeft;
    xEnd = xRight;

    for (yPos = yLeft; yPos <= yBottom; yPos++)
    {
        SrpDrawHorizontalLine(ROUND2INT(xStart), ROUND2INT(xEnd), yPos);

        xStart = xStart + slopeLeft;
        xEnd = xEnd + slopeRight;
    }
}

/*----------------------------------------------------------------------------*/
/*                              Public Functions                              */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * void SrpDrawPixel(int x, int y)
 *
 * This function draws a pixel on (x, y) with current rendering context.
 */
void SrpDrawPixel(int x, int y)
{
    size_t offset;
    
#ifndef NDEBUG
    POINT2I point = {x, y};
    int retTemp;

    retTemp = SrpCheckPointBuffer(&point, 1);
    ASSERTMSG(retTemp, "SrpDrawPixel: invalid raster position.");
#endif

    offset = y * SrpRCGetPitch() + x * SrpRCGetBit() / 8;

    SrpRCSetPixel(offset);
}

/*------------------------------------------------------------------------------
 * void SrpDrawLine(int x0, int y0, int x1, int y1)
 *
 * This function draw a line from (x0, y0) to (x1, y1), both ends included.
 */
void SrpDrawLine(int x0, int y0, /* starting position */
                 int x1, int y1) /* ending position */
{
    int pitch, bit;
    int i, dx, dy;
    int xOffset, yOffset;
    size_t totalOffset;
    int xInc, yInc;
    float slope; /* not really slope, min(|dx|, |dy|) / max(|dx|, |dy|) */
    
#ifndef NDEBUG
    POINT2I pointList[2] = {{x0, y0}, {x1, y1}};
    int retTemp;

    retTemp = SrpCheckPointBuffer(pointList, 2);
    ASSERTMSG(retTemp, "SrpDrawLine: invalid raster position.");
#endif
    
    pitch = SrpRCGetPitch();
    bit = SrpRCGetBit();

    totalOffset = y0 * pitch + x0 * bit / 8;
    
    dx = x1 - x0;
    dy = y1 - y0;
    
    xOffset = 0;
    yOffset = 0;

    xInc = bit / 8;
    yInc = pitch;
    
    /* need absolute value */
    if (dx < 0)
    {
        dx = -dx;
        xInc = -xInc;
    }
    
    if (dy < 0)
    {
        dy = -dy;
        yInc = -yInc;
    }
    
    if (dx > dy)
    {
        slope = (float)dy / (float)dx;
        for (i = 0; i <= dx; i++)
        {
/*
            printf("xOffset = %d, yOffset = %d, ", xOffset, yOffset);
            printf("dist = %f, ", (float)xOffset * slope - yOffset);
            printf("flag = %d\n", (float)xOffset * slope - yOffset - 0.5f > 0 ? 1 : 0);
*/
            SrpRCSetPixel(totalOffset);
            
            if ((float)xOffset * slope - yOffset > 0.5f)
            {
                totalOffset += yInc;
                yOffset++;
            }
            
            totalOffset += xInc;
            xOffset++;
        }
    }
    else
    if (dy > dx)
    {
        slope = (float)dx / (float)dy;
        for (i = 0; i <= dy; i++)
        {
/*
            printf("xOffset = %d, yOffset = %d, ", xOffset, yOffset);
            printf("dist = %f\n", (float)yOffset * slope - xOffset);
            printf("flag = %d\n", (float)yOffset * slope - xOffset - 0.5f > 0 ? 1 : 0);
*/
            SrpRCSetPixel(totalOffset);
            
            if ((float)yOffset * slope - xOffset > 0.5f)
            {
                totalOffset += xInc;
                xOffset++;
            }
            
            totalOffset += yInc;
            yOffset++;
        }
    }
    else
    {
        for (i = 0; i <= dx; i++)
        {
/*
            printf("xOffset = %d, yOffset = %d\n", xOffset, yOffset);
*/
            SrpRCSetPixel(totalOffset);
            
            totalOffset += xInc;
            xOffset++;
            
            totalOffset += yInc;
            yOffset++;
        }
    }
}

/*------------------------------------------------------------------------------
 * void SrpDrawTriangleWire(int x0, int y0, int x1, int y1, int x2, int y2)
 *
 * This function draws a wired trianlge specified with 
 * (x0, y0), (x1, y1), (x2, y2).
 */
void SrpDrawTriangleWire(int x0, int y0, int x1, int y1, int x2, int y2)
{
    POINT2I pointList[3] = {{x0, y0}, {x1, y1}, {x2, y2}};

#ifndef NDEBUG
    int retTemp;
    retTemp = SrpCheckPointBuffer(pointList, 3);
    ASSERTMSG(retTemp, "SrpDrawTriangleWire: invalid raster position.");
#endif

    SrpDrawPolygonWire(pointList, 3);
}

/*------------------------------------------------------------------------------
 * void SrpDrawTriangleFill(int x0, int y0, int x1, int y1, int x2, int y2)
 *
 * This function draws a filled trianlge specified with 
 * (x0, y0), (x1, y1), (x2, y2).
 */
void SrpDrawTriangleFill(int x0, int y0, int x1, int y1, int x2, int y2)
{
    int temp;
    float xNewf;
    int xNewi;
    POINT2I pointList[3] = {{x0, y0}, {x1, y1}, {x2, y2}};

#ifndef NDEBUG
    int retTemp;
    retTemp = SrpCheckPointBuffer(pointList, 3);
    ASSERTMSG(retTemp, "SrpDrawTriangleFill: invalid raster position.");
#endif

    if (SrpPolygonIsHorizontalLine(pointList, 3))
    { 
        printf("Debug: the triangle is a horizontal line in \
SrpDrawTriangleFill().\n");
        return;
    }

    if (SrpPolygonIsVerticalLine(pointList, 3))
    {
        printf("Debug: the triangle is a vertical line in \
SrpDrawTriangleFill().\n");
        return;
    }

    /* Sort p0, p1, p2 in ascending y order. */
    if (y0 > y1)
    {
        temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    if (y0 > y2)
    {
        temp = x0;
        x0 = x2;
        x2 = temp;

        temp = y0;
        y0 = y2;
        y2 = temp;
    }

    if (y1 > y2)
    {
        temp = x1;
        x1 = x2;
        x2 = temp;

        temp = y1;
        y1 = y2;
        y2 = temp;
    }

    if (y0 == y1)
    {
        SrpDrawTriangleTopFlat(x0, y0, x1, y1, x2, y2);
    }
    else if (y1 == y2)
    {
        SrpDrawTriangleBottomFlat(x0, y0, x1, y1, x2, y2);
    }
    else
    {
        xNewf = (float)(x2 - x0) / (float)(y2 - y0) * (float)(y1 - y0) +
            (float)x0;
        xNewi = ROUND2INT(xNewf);

        SrpDrawTriangleBottomFlat(x0, y0, x1, y1, xNewi, y1);
        SrpDrawTriangleTopFlat(xNewi, y1, x1, y1, x2, y2);
    }
}

/*------------------------------------------------------------------------------
 * void SrpDrawPolygonWire(const POINT2I *pBuffer, int count)
 *
 * This function draws a wired polygon with the fisrt 'count' points
 * stored in 'pBuffer'.
 */
void SrpDrawPolygonWire(const POINT2I *pBuffer, int count)
{
    int i;

    ASSERTMSG(pBuffer != NULL && count > 0, 
        "SrpDrawPolygonWire: invalid arguments.");
    
    ASSERTMSG(count > 2, 
        "SrpDrawPolygonWire: points count should be at least 3.");

    ASSERTMSG(SrpCheckPointBuffer(pBuffer, count), 
        "SrpDrawPolygonWire: invalid raster position.");

    for (i = 0; i < count - 1; i++)
    {
        /* Draw line from the ith to (i+1)th point */
        SrpDrawLine(pBuffer[i].x, pBuffer[i].y, 
                    pBuffer[i + 1].x, pBuffer[i + 1].y);
    }

    /* Draw line from the last point to the first point, close the polygon */
    SrpDrawLine(pBuffer[count - 1].x, pBuffer[count - 1].y,
                pBuffer[0].x, pBuffer[0].y);
}

/*------------------------------------------------------------------------------
 * void SrpDrawPolygonFill(const POINT2I *pBuffer, int count)
 *
 * This function draws a filled polygon with the fisrt 'count' points
 * stored in 'pBuffer'.
 */
void SrpDrawPolygonFill(const POINT2I *pBuffer, int count)
{
    /* Indice denote starts and ends of edges. 'left' or 'right'
     * does not mean the left or right hand side edge of the polygon,
     * but the index locates left or right of the top point in 'pBuffer'.
     */
    int indexLeftStart, indexLeftEnd, indexRightStart, indexRightEnd;

    /* The start and end of a horizontal line. */
    float xStart, xEnd;

    /* Current y distance to the top point. */
    int yOffset;

    /* The index of the top point and bottom point. */
    int indexTop, indexBottom;

    /* Not realy slope, is actually dx / dy, used to increase x. */
    float slopeLeft, slopeRight;

    int dx, dy;
    int i;

    ASSERTMSG(pBuffer != NULL && count > 0, 
        "SrpDrawPolygonFill: invalid arguments.");
    
    ASSERTMSG(count > 2, 
        "SrpDrawPolygonFill: points count should be at least 3.");

    ASSERTMSG(SrpCheckPointBuffer(pBuffer, count), 
        "SrpDrawPolygonFill: invalid raster position.");

    if (SrpPolygonIsHorizontalLine(pBuffer, count))
    { 
        printf("Debug: the polygon is a horizontal line in \
SrpDrawPolygonFill().\n");
        return;
    }

    if (SrpPolygonIsVerticalLine(pBuffer, count))
    {
        printf("Debug: the polygon is a vertical line in \
SrpDrawPolygonFill().\n");
        return;
    }

    /* Find the top point, whose y is the smallest,
     * and the bottom point, whose y is the biggest.
     */
    indexTop = indexBottom = 0;
    for (i = 1; i < count; i++)
    {
        if (pBuffer[i].y < pBuffer[indexTop].y)
        {
            indexTop = i;
        }

        if (pBuffer[i].y > pBuffer[indexBottom].y)
        {
            indexBottom = i;
        }
    }

    /* Initialize left and right edges with the top point's index */
    indexLeftStart = indexLeftEnd = indexTop;
    indexRightStart = indexRightEnd = indexTop;

    /* Start drawing */

    for (yOffset = pBuffer[indexTop].y; 
         yOffset <= pBuffer[indexBottom].y; yOffset++)
    {
        if (yOffset >= pBuffer[indexLeftEnd].y)
        {
            /* Left side edge is finished. Update indice, xStart,
             * and calculate the new slope.
             */

            /* Ignore those points whose y is equal with their
             * previous points, to avoid being divided by zero
             * when calculating the slope.
             */
            while (TRUE)
            {
                indexLeftStart = indexLeftEnd;
                indexLeftEnd = (indexLeftStart - 1 + count) % count;

                if (pBuffer[indexLeftStart].y != pBuffer[indexLeftEnd].y)
                {
                    break;
                }
            }

            dx = pBuffer[indexLeftEnd].x - pBuffer[indexLeftStart].x;
            dy = pBuffer[indexLeftEnd].y - pBuffer[indexLeftStart].y;
            slopeLeft = (float)dx / (float)dy;
            xStart = pBuffer[indexLeftStart].x;
        }

        if (yOffset >= pBuffer[indexRightEnd].y)
        {
            /* Right side edge is finished. Update indice, xEnd,
             * and calculate the new slope.
             */

            /* Ignore those points for the same reason. */
            while (TRUE)
            {
                indexRightStart = indexRightEnd;
                indexRightEnd = (indexRightStart + 1) % count;

                if (pBuffer[indexRightStart].y != pBuffer[indexRightEnd].y)
                {
                    break;
                }
            }

            dx = pBuffer[indexRightEnd].x - pBuffer[indexRightStart].x;
            dy = pBuffer[indexRightEnd].y - pBuffer[indexRightStart].y;
            slopeRight = (float)dx / (float)dy;
            xEnd = pBuffer[indexRightStart].x;
        }
/*
        printf("%d, ", yOffset);
        printf("ls=%d, le=%d, rs=%d, re=%d, ", indexLeftStart, indexLeftEnd,
               indexRightStart, indexRightEnd);
        printf("xStart=%f, xEnd=%f\n", xStart, xEnd);
*/
        SrpDrawHorizontalLine(ROUND2INT(xStart), ROUND2INT(xEnd), yOffset);

        xStart = xStart + slopeLeft;
        xEnd = xEnd + slopeRight;
    }
}
