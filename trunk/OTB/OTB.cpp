
/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_tut01 - Minimal HGE application
*/

#include "..\HGE\hge.h"
#include "..\HGE\hgefont.h"
#include "..\HGE\hgeparticle.h"
#include "PcPad.h"

HGE*				hge =0;
hgeFont*			fnt =0;
// hgeSprite*			spt =0;
// hgeParticleSystem*	par =0;
// HTEXTURE			tex;

PcPadManager	PadManager;

// int		nScreenSizeX =800;
// int		nScreenSizeY =450;
int		nScreenSizeX =1280;
int		nScreenSizeY =720;

float	rGroundY	 =nScreenSizeY/10.0f;
float	rPosX		 =nScreenSizeX/10.0f;
float	rCharSizeY	 =nScreenSizeY/4.0f;
float	rCharSizeX	 =rCharSizeY/5.0f;
float	rCharRacketY =rCharSizeY*0.75f;
float	rRacketLen	 =rCharSizeY*0.6f;
hgeVector vRacketDir(0, 1.0f);

float	rCharSpeed	 =nScreenSizeX/2.0f;

hgeVector vBallPos(nScreenSizeX/2.0f, nScreenSizeY/2.0f);
hgeVector vBallLastPos(nScreenSizeX/2.0f, nScreenSizeY/2.0f);
hgeVector vBallSpeed(500,5);

void ApplyWorldTransform() 
{
	static float rCenterX 	=0;			// center of scale & rotation
	static float rCenterY 	=float(nScreenSizeY)/2.0f;
	static float rCenterDx	=0;			// post-offset
	static float rCenterDy	=0;
	static float rRotation	=0;			// rotation in radians
	static float rScaleX	=1;
	static float rScaleY	=-1;

	// Transformations are applied in this order: first scaling, then rotation and finally displacement. 
	hge->Gfx_SetTransform(rCenterX, rCenterY, rCenterDx, rCenterDy, rRotation, rScaleX, rScaleY);
}

bool FrameFunc()
{
	PadManager.Update();

	float rDeltaTime =hge->Timer_GetDelta();

	// move char
	const hgeVector& vAxisLeft =PadManager.GetAxisLeft();
	rPosX +=vAxisLeft.x*rDeltaTime*rCharSpeed;

	// move racket
	const hgeVector& vAxisRight =PadManager.GetAxisRight();
	if (vAxisRight.Length()>0.0f)
	{
		hgeVector vAxisRightNorm(vAxisRight);
		vAxisRightNorm.Normalize();
		vRacketDir =hgeVector(vAxisRightNorm.x, -vAxisRightNorm.y);
	}

	// ball basic phy
	vBallLastPos =vBallPos;
	vBallPos     +=vBallSpeed*rDeltaTime;
	vBallSpeed.y +=1500.0f*rDeltaTime;			// gravity

	// check racket collide
	hgeVector vRacketCenter(rPosX, rCharRacketY);
	hgeVector vDiff =vBallPos-vRacketCenter;
	if (vDiff.Length()<rRacketLen)
	{
		vBallSpeed =-vBallSpeed;
	}

	// Collision GND
	if (vBallPos.y>(nScreenSizeY-rGroundY))
	{
		if (vBallSpeed.y<500)
		{
			//reset
			vBallPos =hgeVector(nScreenSizeX/2.0f, nScreenSizeY/2.0f);
		}
		else
		{
			vBallPos.y =(nScreenSizeY-rGroundY);
			vBallSpeed.y =-vBallSpeed.y*0.80f;
		}
	}
	// Collision Walls
	if (vBallPos.x>nScreenSizeX || vBallPos.x<0)
	{
		vBallSpeed.x =-vBallSpeed.x;
	}
// 	par->MoveTo(vBallPos.x,vBallPos.y);
// 	par->Update(rDeltaTime);

	ApplyWorldTransform();

	// Exit w/ Esc
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	// Continue execution
	return false;
}

void DrawInputs() 
{
	for (int i=0; i<PcPadManager::PAD_MAX_ENTRIES; ++i)
	{
		fnt->SetColor(0xFFFFA000);
		fnt->printf(10, i*25.0f, HGETEXT_LEFT, "%d %s", i, SMARTENUM_GET_STRING(PcPadManager::CtrlIdx, i)+4);

		fnt->SetColor(0xFFFF00A0);
		PcPadManager::CtrlStatus status =PadManager.GetCtrlState((PcPadManager::CtrlIdx)i);
		fnt->printf(250, i*25.0f, HGETEXT_LEFT, "%d", status);
	}
}

bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

//	DrawInputs();
	fnt->printf(10, 40.0f, HGETEXT_LEFT, "%d", int(1.0f/hge->Timer_GetDelta()) );

	// gnd
	hge->Gfx_RenderLine(0, nScreenSizeY-rGroundY, nScreenSizeX, nScreenSizeY-rGroundY, 0xFFFFFFFF);

	// char
	hge->Gfx_RenderBox(rPosX-rCharSizeX, nScreenSizeY-(rGroundY+5), rPosX+rCharSizeX, nScreenSizeY-(rGroundY+rCharSizeY), 0xFFFF0000);

	// racket
	float rRacketY =nScreenSizeY-(rGroundY+rCharRacketY);
	hge->Gfx_RenderLine(rPosX, rRacketY, rPosX+vRacketDir.x*rRacketLen, rRacketY+vRacketDir.y*rRacketLen, 0xFF60FF60);

	// ball
	hge->Gfx_RenderBox(vBallPos.x-5, vBallPos.y-5, vBallPos.x+5, vBallPos.y+5, 0xFFFFFF00);

//	par->Render();

	hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	// init
	hge->System_SetState(HGE_FRAMEFUNC,		FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC,	RenderFunc);
	hge->System_SetState(HGE_SHOWSPLASH,	false);
	hge->System_SetState(HGE_SCREENWIDTH,	nScreenSizeX);
	hge->System_SetState(HGE_SCREENHEIGHT,	nScreenSizeY);
	hge->System_SetState(HGE_TITLE,			"Off the Ball");
	hge->System_SetState(HGE_WINDOWED,		true);

	// Tries to initiate HGE with the states set. If something goes wrong, "false" is returned
	// and more specific description of what have happened can be read with System_GetErrorMessage().
	if (hge->System_Initiate())
	{
		HWND hwnd =hge->System_GetState(HGE_HWND);
		PadManager.Init(hwnd);

		// Load a font
		fnt=new hgeFont("Data/font2.fnt");
		fnt->SetScale(-1);
		fnt->SetProportion(-1);

		// Create and set up a particle system
// 		tex=hge->Texture_Load("Data/particles.png");
// 		spt=new hgeSprite(tex, 32, 32, 32, 32);
// 		spt->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
// 		spt->SetHotSpot(16,16);
// 		par=new hgeParticleSystem("Data/ParticleRed.psi",spt);
// 		par->Fire();


		hge->System_Start();

		// end of game
// 		delete par;
// 		delete spt;
		delete fnt;

		PadManager.Kill();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}


	// Now ESC has been pressed or the user
	// has closed the window by other means.

	// Restore video mode and free all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface. If there are no more references, the HGE object will be deleted.
	hge->Release();

	return 0;
}
