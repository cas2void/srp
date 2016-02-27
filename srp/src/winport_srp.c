/*******************************************************************************
 * File   : winport_srp.c
 * Content: Porting function for Windows platform
 *
 * Coder  : Ean Jee
 * Time   : 2011-02-14 17:08
 ******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "malloc_ig.h"
#include "winport_srp.h"
#include "rcmanager_srp.h"

/*----------------------------------------------------------------------------*/
/*                               Data Structure                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                Private Data                                */
/*----------------------------------------------------------------------------*/

static HBITMAP sg_hBitmap;
static HDC sg_hdcReal, sg_hdcMem;
static LARGE_INTEGER sg_lastTime;
static LARGE_INTEGER sg_ticksPerSecond;
static int sg_hiResTimer;
static double sg_fps;

static void (*sg_pDemoInit)(void);
static void (*sg_pDemoMain)(float);
static void (*sg_pDemoQuit)(void);

/*----------------------------------------------------------------------------*/
/*                       Private Function Declarations                        */
/*----------------------------------------------------------------------------*/

static void SrpFlipBuffers(void);

/*----------------------------------------------------------------------------*/
/*                             Private Functions                              */
/*----------------------------------------------------------------------------*/

static void SrpFlipBuffers(void)
{
    int ret;
    char fpsString[32];
    int stringSize;

    ret = SetBitmapBits(sg_hBitmap, SrpRCGetSize(), SrpRCGetBuffer());
    ASSERT(ret != 0);

    /* Show the fps */
    stringSize = sprintf(fpsString, "%8.2lfFPS", sg_fps);
    TextOut(sg_hdcMem, 0, 0, fpsString, stringSize);
    
    BitBlt(sg_hdcReal, 0, 0, SrpRCGetWidth(), SrpRCGetHeight(), 
           sg_hdcMem, 0, 0, SRCCOPY);
}

/*----------------------------------------------------------------------------*/
/*                              Public Functions                              */
/*----------------------------------------------------------------------------*/

extern void SrpDemoCallback(void);

int SrpInit(HWND hwnd, int width, int height, int bit)
{
    HFONT hFont;

    if (!SrpCreateRC(width, height, bit))
    {
        printf("SrpInit failed.\n");
        return FALSE;
    }

    if ((sg_hdcReal = GetDC(hwnd)) == NULL)
    {
        printf("SrpInit failed.\n");
        return FALSE;
    }

    if ((sg_hdcMem = CreateCompatibleDC(sg_hdcReal)) == NULL)
    {
        printf("SrpInit failed.\n");
        return FALSE;
    }        

    if ((sg_hBitmap = CreateCompatibleBitmap(sg_hdcReal, width, height)) 
        == NULL)
    {
        printf("SrpInit failed.\n");
        return FALSE;
    }

    if (SelectObject(sg_hdcMem, sg_hBitmap) == NULL)
    {
        printf("SrpInit failed.\n");
        return FALSE;
    }

    /* Get timer */
    if (!QueryPerformanceFrequency(&sg_ticksPerSecond))
    {
        printf("High-res timer is not supported.\n");

        sg_ticksPerSecond.QuadPart = 1000;
        sg_lastTime.QuadPart = GetTickCount();

        sg_hiResTimer = FALSE;
    }
    else
    {
        QueryPerformanceCounter(&sg_lastTime);
        sg_hiResTimer = TRUE;
    }

    /* Set font */
    hFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
                       ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, 
                       ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, 
                       "Lucida Console");
    SelectObject(sg_hdcMem, hFont);
    DeleteObject(hFont);
    SetTextColor(sg_hdcMem, RGB(255,255,255));
    SetBkColor(sg_hdcMem, RGB(0,0,0));
    SetBkMode(sg_hdcMem, OPAQUE);

    sg_pDemoInit = NULL;
    sg_pDemoMain = NULL;
    sg_pDemoQuit = NULL;

    SrpDemoCallback();

    if (!sg_pDemoInit || !sg_pDemoMain || !sg_pDemoQuit)
    {
        printf("SrpInit: Demo program should implement SrpDemoCallback() \
with a call of SrpWinportSetDemoEntry().\n");
        return FALSE;
    }

    (*sg_pDemoInit)();

    return TRUE;
}

void SrpMain(void)
{
    LARGE_INTEGER currentTime;
    float dt;

    if (sg_hiResTimer)
    {
        QueryPerformanceCounter(&currentTime);
    }
    else
    {
        currentTime.QuadPart = GetTickCount();
    }

    /* Calculate the fps */
    sg_fps = (double)sg_ticksPerSecond.QuadPart / 
        (double)(currentTime.QuadPart - sg_lastTime.QuadPart);

    sg_lastTime.QuadPart = currentTime.QuadPart;

    /* Now go rendering */
    dt = 1.0f / (float)sg_fps;
    (*sg_pDemoMain)(dt);

    /* Swap buffers */
    SrpFlipBuffers();
}

void SrpQuit(HWND hwnd)
{
    (*sg_pDemoQuit)();
    SrpDeleteRC();
    DeleteObject(sg_hBitmap);
    DeleteDC(sg_hdcMem);
    ReleaseDC(hwnd, sg_hdcReal);
}

void SrpWinportSetDemoEntry(void (*init)(void), void (*main)(float), 
                            void (*quit)(void))
{
    sg_pDemoInit   = init;
    sg_pDemoMain   = main;
    sg_pDemoQuit   = quit;
}
