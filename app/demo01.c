#include <stdio.h>
#include <windows.h>
#include "datadef_srp.h"
#include "winport_srp.h"
#include "rcmanager_srp.h"
#include "transformer_srp.h"
#include "vector_srp.h"
#include "matrix_srp.h"
#include "renderee_srp.h"
#include "raster_srp.h"

static RENDER_LIST *sg_pRl;
static float sg_angle;
static TRIANGLE_INDIE *sg_pTri;

void Demo01Init(void)
{
    VECTOR3F a, b, c;

    if (!SrpCreateRenderList(&sg_pRl))
    {
        printf("Demo Init error.\n");
        return;
    }

    SrpVectorLoad3f(a, 1.0f,-1.0f, 0.0f);
    SrpVectorLoad3f(b, 0.0f, 1.0f, 0.0f);
    SrpVectorLoad3f(c,-1.0f,-1.0f, 0.0f);

    if (!SrpCreateTriIndie(&sg_pTri, a, b, c))
    {
        printf("Demo Init error.\n");
        return;
    }

    SrpRCSetMatrixMode(SRP_PROJECTION);
    SrpRCLoadIdentity();
    SrpTransformerSetProjection(90.0f, (float)SrpRCGetWidth() / (float)SrpRCGetHeight(), 
                                -2.0f, -100.0f);

    SrpRCSetMatrixMode(SRP_VIEWPORT);
    SrpRCLoadIdentity();
    SrpTransformerSetViewport(640, 480);

    SrpRCSetMatrixMode(SRP_MODELVIEW);

    SrpRCSetClearColor(0, 0, 0, 0);

    sg_angle = 0.0f;
}

void Demo01Main(float dt)
{
    VECTOR3F vTrans, vRot, vScale;

    LARGE_INTEGER lastTime, currentTime;
    LARGE_INTEGER ticksPerSecond;
    int hiResTimer;

    /* Get timer */
    if (!QueryPerformanceFrequency(&ticksPerSecond))
    {
        printf("High-res timer is not supported.\n");

        ticksPerSecond.QuadPart = 1000;
        lastTime.QuadPart = GetTickCount();

        hiResTimer = FALSE;
    }
    else
    {
        QueryPerformanceCounter(&lastTime);
        hiResTimer = TRUE;
    }

    SrpRCClear();

    if (hiResTimer)
    {
        QueryPerformanceCounter(&currentTime);
    }
    else
    {
        currentTime.QuadPart = GetTickCount();
    }

    printf("SrpRCClear(): %lf, tick = %lf\n", (double)(currentTime.QuadPart - lastTime.QuadPart),
           (double)ticksPerSecond.QuadPart);


    SrpRCSetDrawColor(0, 255, 0, 0);

    SrpVectorLoad3f(vTrans, 0.0f, 0.0f, -10.0f);
    SrpVectorLoad3f(vRot, 0.0f, 1.0f, 0.0f);
    SrpVectorLoad3f(vScale, 3.0f, 3.0f, 3.0f);

    SrpResetRenderList(sg_pRl);
    SrpInsertTriangleToRenderList(sg_pTri, sg_pRl);

    SrpRCLoadIdentity();

    sg_angle += 50.0f * dt;
    SrpTransformerSetTranslationf(vTrans);
    SrpTransformerSetRotationf(vRot, sg_angle);
    SrpTransformerSetScalef(vScale);

    SrpTransRenderListLocToCam(sg_pRl);
    SrpTransRenderListCamToProj(sg_pRl);
    SrpTransRenderListProjToScr(sg_pRl); 

    SrpDrawRenderList(sg_pRl);
}

void Demo01Quit(void)
{
    SrpDeleteTriIndie(sg_pTri);
    SrpDeleteRenderList(sg_pRl);
}

void SrpDemoCallback(void)
{
    SrpWinportSetDemoEntry(Demo01Init, Demo01Main, Demo01Quit);
}
