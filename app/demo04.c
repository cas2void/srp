#include <stdio.h>
#include <math.h>
#include "winport_srp.h"
#include "datadef_srp.h"
#include "rcmanager_srp.h"
#include "transformer_srp.h"
#include "vector_srp.h"
#include "renderee_srp.h"
#include "model_srp.h"
#include "math_srp.h"

static RENDER_LIST *sg_pRl;
static MODEL *sg_pModel;
static OBJECT *sg_pObj;

void Demo04Init(void)
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

    SrpRCEnable(SRP_CULL_OBJECT);
    SrpRCEnable(SRP_CULL_FACE);
}

void Demo04Main(float dt)
{
    static float angleV = 0.0f;
    static float angleH = 0.0f;
    static float cubeAngle = 0.0f;

    VECTOR3F eye, center, up, look, right, yPositive;

    if (KEYDOWN(VK_UP))
    {
        angleV += 30.0f * dt;
    }

    if (KEYDOWN(VK_DOWN))
    {
        angleV -= 30.0f * dt;
    }

    if (KEYDOWN(VK_LEFT))
    {
        angleH -= 30.0f * dt;
    }

    if (KEYDOWN(VK_RIGHT))
    {
        angleH += 30.0f * dt;
    }

    cubeAngle += 50.0f * dt;
    
    SrpVectorLoad3f(eye, 0.0f, 20.0 * sin(SrpMathDegToRadf(angleV)), 
                    20.0f * cos(SrpMathDegToRadf(angleV)));
    SrpVectorLoad3f(center, 0.0f, 0.0f, 0.0f);
    SrpVectorSubtract3f(look, center, eye);
    SrpVectorLoad3f(right, 1.0f, 0.0f, 0.0f);
    SrpVectorCrossProduct3f(up, right, look);

    SrpVectorLoad3f(yPositive, 0.0f, 1.0f, 0.0f);

    SrpRCClear();
    SrpRCSetDrawColor(0, 255, 0, 0);
    SrpResetRenderList(sg_pRl);
    SrpRCLoadIdentity();

    SrpTransformerSetRotationf(yPositive, angleH);
    SrpTransformerSetLookAt(eye, center, up);

    SrpRotateObject(sg_pObj, 0.0f, cubeAngle, 0.0f);
    SrpDrawObject(sg_pObj, sg_pRl);
    SrpTransRenderListCamToProj(sg_pRl);
    SrpTransRenderListProjToScr(sg_pRl); 

    SrpDrawRenderList(sg_pRl);
}

void Demo04Quit(void)
{
    SrpDeleteObject(sg_pObj);
    SrpModelRelease(sg_pModel);
    SrpDeleteRenderList(sg_pRl);
}

void SrpDemoCallback(void)
{
    SrpWinportSetDemoEntry(Demo04Init, Demo04Main, Demo04Quit);
}
