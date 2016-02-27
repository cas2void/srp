#include <stdio.h>
#include "winport_srp.h"
#include "datadef_srp.h"
#include "rcmanager_srp.h"
#include "transformer_srp.h"
#include "vector_srp.h"
#include "matrix_srp.h"
#include "renderee_srp.h"
#include "raster_srp.h"
#include "model_srp.h"

static RENDER_LIST *sg_pRl;
static MODEL *sg_pModel;
static OBJECT *sg_pTank1, *sg_pTank2;

void Demo03Init(void)
{
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

    if (!SrpCreateObject(&sg_pTank1, sg_pModel, 
                         -150.0f, -70.0f, -400.0f,
                         0.0f, 0.0f, 0.0f,
                         1.0f, 1.0f, 1.0f))
    {
        printf("Demo Init error.\n");
        return;
    }

    if (!SrpCreateObject(&sg_pTank2, sg_pModel, 
                         150.0f, -70.0f, -400.0f,
                         0.0f, 0.0f, 0.0f,
                         1.0f, 1.0f, 1.0f))
    {
        printf("Demo Init error.\n");
        return;
    }

    SrpRCSetMatrixMode(SRP_PROJECTION);
    SrpRCLoadIdentity();
    SrpTransformerSetProjection(90.0f, (float)SrpRCGetWidth() / (float)SrpRCGetHeight(), 
                                -2.0f, -1000.0f);

    SrpRCSetMatrixMode(SRP_VIEWPORT);
    SrpRCLoadIdentity();
    SrpTransformerSetViewport(640, 480);

    SrpRCSetMatrixMode(SRP_MODELVIEW);

    SrpRCSetClearColor(0, 0, 0, 0);
    SrpRCSetDrawColor(0, 255, 0, 0);
}

void Demo03Main(float dt)
{
    static float angle1 = 0.0f;
    static float angle2 = 0.0f;

    SrpRCClear();
    SrpResetRenderList(sg_pRl);
    SrpRCLoadIdentity();

    angle1 -= 50.0f * dt;
    angle2 += 50.0f * dt;

    /* Tank 1 */
    SrpRotateObject(sg_pTank1, 0.0f, angle1, 0.0f);
    SrpDrawObject(sg_pTank1, sg_pRl);

    /* Tank 2 */
    SrpRotateObject(sg_pTank2, 0.0f, angle2, 0.0f);
    SrpDrawObject(sg_pTank2, sg_pRl);

    /* Render list */
    SrpTransRenderListCamToProj(sg_pRl);
    SrpTransRenderListProjToScr(sg_pRl); 

    SrpDrawRenderList(sg_pRl);
}

void Demo03Quit(void)
{
    SrpDeleteObject(sg_pTank1);
    SrpDeleteObject(sg_pTank2);
    SrpModelRelease(sg_pModel);
    SrpDeleteRenderList(sg_pRl);
}

void SrpDemoCallback(void)
{
    SrpWinportSetDemoEntry(Demo03Init, Demo03Main, Demo03Quit);
}
