#include <stdio.h>
#include <math.h>
#include "winport_srp.h"
#include "datadef_srp.h"
#include "rcmanager_srp.h"
#include "transformer_srp.h"
#include "vector_srp.h"
#include "matrix_srp.h"
#include "renderee_srp.h"
#include "raster_srp.h"
#include "model_srp.h"
#include "math_srp.h"

static RENDER_LIST *sg_pRl;
static MODEL *sg_pModel;
static OBJECT *sg_pObj;

void Demo02Init(void)
{
    if (!SrpCreateRenderList(&sg_pRl))
    {
        printf("Demo Init error.\n");
        return;
    }

    if (!SrpModelLoadPLG(&sg_pModel, "./model/cube1.plg"))
    {
        printf("Demo Init error.\n");
        return;
    }

    if (!SrpCreateObject(&sg_pObj, sg_pModel, 
                         0.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 0.0f,
                         1.0f, 1.0f, 1.0f))
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
}

void Demo02Main(float dt)
{
    static float angle = 0.0f;
    static float cubeAngle = 0.0f;

    VECTOR3F eye, center, up, look, right;

    if (KEYDOWN(VK_UP))
    {
        angle += 30.0f * dt;
    }

    if (KEYDOWN(VK_DOWN))
    {
        angle -= 30.0f * dt;
    }

    cubeAngle += 50.0f * dt;
    
    SrpVectorLoad3f(eye, 0.0f, 20.0 * sin(SrpMathDegToRadf(angle)), 
                    20.0f * cos(SrpMathDegToRadf(angle)));
    SrpVectorLoad3f(center, 0.0f, 0.0f, 0.0f);
    SrpVectorSubtract3f(look, center, eye);
    SrpVectorLoad3f(right, 1.0f, 0.0f, 0.0f);
    SrpVectorCrossProduct3f(up, right, look);

    SrpRCClear();
    SrpRCSetDrawColor(0, 255, 0, 0);
    SrpResetRenderList(sg_pRl);
    SrpRCLoadIdentity();

    SrpTransformerSetLookAt(eye, center, up);

    SrpRotateObject(sg_pObj, 0.0f, cubeAngle, 0.0f);
    SrpDrawObject(sg_pObj, sg_pRl);
    SrpTransRenderListCamToProj(sg_pRl);
    SrpTransRenderListProjToScr(sg_pRl); 

    SrpDrawRenderList(sg_pRl);
}

void Demo02Quit(void)
{
    SrpDeleteObject(sg_pObj);
    SrpModelRelease(sg_pModel);
    SrpDeleteRenderList(sg_pRl);
}

void SrpDemoCallback(void)
{
    SrpWinportSetDemoEntry(Demo02Init, Demo02Main, Demo02Quit);
}
