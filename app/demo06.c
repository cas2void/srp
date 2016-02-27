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

#define MARKER_NUM_SIZE    21
#define MARKER_NUM         (MARKER_NUM_SIZE * MARKER_NUM_SIZE)

#define TOWER_NUM          96

#define GRID_SIZE          500.0f
#define UNIVERSE_RADIUS    ((MARKER_NUM_SIZE - 1) * GRID_SIZE * 0.5f)

#define PLAYER_SPEED       300.0f

static RENDER_LIST *sg_pRl;
static MODEL *sg_pModelMarker, *sg_pModelTower, *sg_pModelPlayer;
static OBJECT *sg_pMarkers[MARKER_NUM], *sg_pTowers[TOWER_NUM], *sg_pPlayer;

static VECTOR3F sg_cam;

void Demo06Init(void)
{
    int i, j;
    float objectRadius;
    float objectX, objectZ;

    if (!SrpCreateRenderList(&sg_pRl))
    {
        printf("Demo Init error.\n");
        return;
    }

    if (!SrpModelLoadPLG(&sg_pModelMarker, "./model/marker1.plg"))
    {
        printf("Demo Init error.\n");
        return;
    }

    objectRadius = SrpModelGetRadius(sg_pModelMarker);

    for (i = 0; i < MARKER_NUM_SIZE; i++)
    {
        for (j = 0; j < MARKER_NUM_SIZE; j++)
        {
            objectX = GRID_SIZE * j - UNIVERSE_RADIUS;
            objectZ = GRID_SIZE * i - UNIVERSE_RADIUS;

            if (!SrpCreateObject(&sg_pMarkers[i * MARKER_NUM_SIZE + j], 
                                 sg_pModelMarker, 
                                 objectX, objectRadius, objectZ,
                                 0.0f, 0.0f, 0.0f,
                                 5.0f, 5.0f, 5.0f))
            {
                printf("Demo Init error.\n");
                return;
            }
        }
    }

    if (!SrpModelLoadPLG(&sg_pModelTower, "./model/tower1.plg"))
    {
        printf("Demo Init error.\n");
        return;
    }

    for (i = 0; i < TOWER_NUM; i++)
    {
        objectX = rand() % (int)(UNIVERSE_RADIUS * 2.0f)  - UNIVERSE_RADIUS;
        objectZ = rand() % (int)(UNIVERSE_RADIUS * 2.0f)  - UNIVERSE_RADIUS;

        if (!SrpCreateObject(&sg_pTowers[i], 
                             sg_pModelTower, 
                             objectX, 0.0f, objectZ,
                             0.0f, 0.0f, 0.0f,
                             1.0f, 5.0f, 1.0f))
        {
            printf("Demo Init error.\n");
            return;
        }
    }

    if (!SrpModelLoadPLG(&sg_pModelPlayer, "./model/tank1.plg"))
    {
        printf("Demo Init error.\n");
        return;
    }

    if (!SrpCreateObject(&sg_pPlayer, sg_pModelPlayer, 
                         0.0f, 0.0f, -500.0f,
                         0.0f, 180.0f, 0.0f,
                         1.0f, 1.0f, 1.0f))
    {
        printf("Demo Init error.\n");
        return;
    }

    SrpRCSetMatrixMode(SRP_PROJECTION);
    SrpRCLoadIdentity();
    SrpTransformerSetProjection(90.0f, (float)SrpRCGetWidth() / (float)SrpRCGetHeight(), 
                                -2.0f, -20000.0f);

    SrpRCSetMatrixMode(SRP_VIEWPORT);
    SrpRCLoadIdentity();
    SrpTransformerSetViewport(640, 480);

    SrpRCSetMatrixMode(SRP_MODELVIEW);

    SrpRCSetClearColor(0, 0, 0, 0);

    SrpRCEnable(SRP_CULL_OBJECT);

    SrpVectorLoad3f(sg_cam, 0.0f, 80.0f, 0.0f);
}

void Demo06Main(float dt)
{
    static float viewAngle = 0.0f;
    static float yTurning = 0.0f;

    float playerSpeed;
    int i;
    VECTOR3F trans;

    if (KEYDOWN(VK_SPACE))
    {
        playerSpeed = PLAYER_SPEED * 4.0f;
    }
    else
    {
        playerSpeed = PLAYER_SPEED;
    }

    if (KEYDOWN(VK_UP))
    {
        sg_cam[0] -= playerSpeed * sin(SrpMathDegToRadf(viewAngle)) * dt;
        sg_cam[2] -= playerSpeed * cos(SrpMathDegToRadf(viewAngle)) * dt;
    }

    if (KEYDOWN(VK_DOWN))
    {
        sg_cam[0] += playerSpeed * sin(SrpMathDegToRadf(viewAngle)) * dt;
        sg_cam[2] += playerSpeed * cos(SrpMathDegToRadf(viewAngle)) * dt;
    }

    if (KEYDOWN(VK_LEFT))
    {
        viewAngle += 50.0f * dt;
        yTurning += 30.0f * dt;

        if (yTurning > 15.0f)
        {
            yTurning = 15.0f;
        }
    }

    if (KEYDOWN(VK_RIGHT))
    {
        viewAngle -= 50.0f * dt;
        yTurning -= 30.0f * dt;

        if (yTurning < -15.0f)
        {
            yTurning = -15.0f;
        }
    }
    else
    {
        if (yTurning > 0.0f)
        {
            yTurning -= 10.0f * dt;
        }
        else if (yTurning < 0.0f)
        {
            yTurning += 10.0f * dt;
        }
    }

    SrpRCClear();
    SrpRCSetDrawColor(0, 255, 0, 0);
    SrpResetRenderList(sg_pRl);
    SrpRCLoadIdentity();

    SrpRCPushMatrix();
    SrpVectorLoad3f(trans, 0.0f, -sg_cam[1], 0.0f);
    SrpTransformerSetTranslationf(trans);
    SrpRotateObject(sg_pPlayer, 0.0f, yTurning, 0.0f);
    SrpDrawObject(sg_pPlayer, sg_pRl);
    SrpRCPopMatrix();

    SrpTransformerSetRotationf(CARDINAL_Y, -viewAngle);

    SrpVectorSubtract3f(trans, ZERO_VECTOR, sg_cam);
    SrpTransformerSetTranslationf(trans);

    for (i = 0; i < MARKER_NUM; i++)
    {
        SrpDrawObject(sg_pMarkers[i], sg_pRl);
    }

    for (i = 0; i < TOWER_NUM; i++)
    {
        SrpDrawObject(sg_pTowers[i], sg_pRl);
    }

    SrpTransRenderListCamToProj(sg_pRl);
    SrpTransRenderListProjToScr(sg_pRl); 

    SrpDrawRenderList(sg_pRl);
}

void Demo06Quit(void)
{
    int i;

    for (i = 0; i < MARKER_NUM; i++)
    {
        SrpDeleteObject(sg_pMarkers[i]);
    }

    for (i = 0; i < TOWER_NUM; i++)
    {
        SrpDeleteObject(sg_pTowers[i]);
    }

    SrpDeleteObject(sg_pPlayer);

    SrpModelRelease(sg_pModelMarker);
    SrpModelRelease(sg_pModelTower);
    SrpModelRelease(sg_pModelPlayer);

    SrpDeleteRenderList(sg_pRl);
}

void SrpDemoCallback(void)
{
    SrpWinportSetDemoEntry(Demo06Init, Demo06Main, Demo06Quit);
}
