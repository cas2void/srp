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

#define TANK_ROW 8
#define TANK_COL 8
#define TANK_NUM (TANK_ROW * TANK_COL)

static RENDER_LIST *sg_pRl;
static MODEL *sg_pModel;
static OBJECT *sg_pTanks[TANK_NUM];

void Demo05Init(void)
{
    int i, j;
    float tankRadius, tankSpace, tankSpanRow, tankSpanCol;
    float tankX, tankZ;

    if (!SrpCreateRenderList(&sg_pRl))
    {
        printf("Demo Init error.\n");
        return;
    }

    if (!SrpModelLoadPLG(&sg_pModel, "./model/tank1.plg"))
    {
        printf("Demo Init error.\n");
        return;
    }

    tankRadius = SrpModelGetRadius(sg_pModel);
    tankSpace = tankRadius * 0.3f;
    tankSpanRow = (tankRadius * TANK_COL + tankSpace * (TANK_COL - 1)) * 0.5f;
    tankSpanCol = (tankRadius * TANK_ROW + tankSpace * (TANK_ROW - 1)) * 0.5f;

    for (i = 0; i < TANK_ROW; i++)
    {
        for (j = 0; j < TANK_COL; j++)
        {
            tankX = (tankRadius + tankSpace) * j + tankRadius * 0.5f - 
                tankSpanRow;
            tankZ = (tankRadius + tankSpace) * i + tankRadius * 0.5f - 
                tankSpanCol;

            if (!SrpCreateObject(&sg_pTanks[i * TANK_ROW + j], sg_pModel, 
                                 tankX, 0.0f, tankZ,
                                 0.0f, 0.0f, 0.0f,
                                 1.0f, 1.0f, 1.0f))
            {
                printf("Demo Init error.\n");
                return;
            }
        }
    }

    SrpRCSetMatrixMode(SRP_PROJECTION);
    SrpRCLoadIdentity();
    SrpTransformerSetProjection(90.0f, (float)SrpRCGetWidth() / (float)SrpRCGetHeight(), 
                                -2.0f, -10000.0f);

    SrpRCSetMatrixMode(SRP_VIEWPORT);
    SrpRCLoadIdentity();
    SrpTransformerSetViewport(640, 480);

    SrpRCSetMatrixMode(SRP_MODELVIEW);

    SrpRCSetClearColor(0, 0, 0, 0);

    SrpRCEnable(SRP_CULL_OBJECT);
}

void Demo05Main(float dt)
{
    static float viewAngle = 0.0f;
    static float objectAngle = 0.0f;

    int i;
    float viewDistance = 800.0f;
    VECTOR3F eye, center, up, look;

    viewAngle += 20.0f * dt;
    objectAngle += 50.0f * dt;

    SrpVectorLoad3f(eye, 
                    viewDistance * cos(SrpMathDegToRadf(viewAngle)),
                    viewDistance * sin(SrpMathDegToRadf(viewAngle)), 
                    viewDistance * sin(SrpMathDegToRadf(viewAngle)) * 2.0f);
    SrpVectorLoad3f(center, 0.0f, 0.0f, 0.0f);
    SrpVectorSubtract3f(look, center, eye);
    SrpVectorLoad3f(up, 0.0f, 1.0f, 0.0f);

    SrpRCClear();
    SrpRCSetDrawColor(0, 255, 0, 0);
    SrpResetRenderList(sg_pRl);
    SrpRCLoadIdentity();

    SrpTransformerSetLookAt(eye, center, up);

    for (i = 0; i < TANK_NUM; i++)
    {
        SrpRotateObject(sg_pTanks[i], 0.0f, objectAngle, 0.0f);
        SrpDrawObject(sg_pTanks[i], sg_pRl);
    }
    SrpTransRenderListCamToProj(sg_pRl);
    SrpTransRenderListProjToScr(sg_pRl); 

    SrpDrawRenderList(sg_pRl);
}

void Demo05Quit(void)
{
    int i;

    for (i = 0; i < TANK_NUM; i++)
    {
        SrpDeleteObject(sg_pTanks[i]);
    }
    SrpModelRelease(sg_pModel);
    SrpDeleteRenderList(sg_pRl);
}

void SrpDemoCallback(void)
{
    SrpWinportSetDemoEntry(Demo05Init, Demo05Main, Demo05Quit);
}
