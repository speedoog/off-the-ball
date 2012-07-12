//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//     ________   _____  _____    __  .__             __________        .__  .__        //
//     \_____  \_/ ____\/ ____\ _/  |_|  |__   ____   \______   \_____  |  | |  |       //
//      /   |   \   __\\   __\  \   __\  |  \_/ __ \   |    |  _/\__  \ |  | |  |       //
//     /    |    \  |   |  |     |  | |   Y  \  ___/   |    |   \ / __ \|  |_|  |__     //
//     \_______  /__|   |__|     |__| |___|  /\___  >  |______  /(____  /____/____/     //
//             \/                          \/     \/          \/      \/                //
//                                                                                      //
//                          .o                                                          //
//                   ¨>   .                                      <¨                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////

#include "..\HGE\hge.h"
#include "..\HGE\hgefont.h"
#include "..\HGE\hgeparticle.h"
#include "PcPad.h"
#include "Game.h"

HGE*				hge =0;
hgeFont*			fnt =0;
// hgeSprite*			spt =0;
// hgeParticleSystem*	par =0;
// HTEXTURE			tex;

Game			_Game;
PcPadManager	PadManager;

// normal window
int  _nScreenSizeX =1280;
int  _nScreenSizeY =720;
bool _bWindowed    =true;

// eeepc window
// int	 _nScreenSizeX =900;
// int	 _nScreenSizeY =500;
// bool _bWindowed	   =true;

// eeepc fullscreen
// int  _nScreenSizeX =1024;
// int  _nScreenSizeY =600;
// bool _bWindowed    =false;

static float rTop	=10.0f;
static float rBottom=-1.0f;
static float rRatio =16.0f/10.0f;
float rSizeY =rTop-rBottom;
float rScreenRatio =float(_nScreenSizeX)/float(_nScreenSizeY);
float rGlobalScale =float(_nScreenSizeY)/rSizeY;


float	rGroundY	 =_nScreenSizeY/10.0f;
float	rPosX		 =_nScreenSizeX/10.0f;
float	rCharSizeY	 =_nScreenSizeY/4.0f;
float	rCharSizeX	 =rCharSizeY/5.0f;
float	rCharRacketY =rCharSizeY*0.75f;
float	rRacketLen	 =rCharSizeY*0.6f;
hgeVector vRacketDir(0, 1.0f);

float	rCharSpeed	 =_nScreenSizeX/2.0f;

hgeVector vBallPos(_nScreenSizeX/2.0f, _nScreenSizeY/2.0f);
hgeVector vBallLastPos(_nScreenSizeX/2.0f, _nScreenSizeY/2.0f);
hgeVector vBallSpeed(500,5);

void ApplyWorldTransform() 
{
/*
	static float rCenterX 	=0;			// center of scale & rotation
	static float rCenterY 	=float(_nScreenSizeY)/2.0f;
	static float rCenterDx	=0;			// post-offset
	static float rCenterDy	=0;
	static float rRotation	=0;			// rotation in radians
	static float rScaleX	=1;
	static float rScaleY	=-1;
	hge->Gfx_SetTransform(rCenterX, rCenterY, rCenterDx, rCenterDy, rRotation, rScaleX, rScaleY);
*/

	static float rCenterX 	=0;			// center of scale & rotation
	static float rCenterY 	=rBottom+(rSizeY/2.0f);
	static float rCenterDx	=float(_nScreenSizeX)/2.0f;
	static float rCenterDy	=float(_nScreenSizeY)/2.0f;
	static float rRotation	=0;
//	rRotation+=0.02f;

	// Transformations are applied in this order: first scaling, then rotation and finally displacement. 
	hge->Gfx_SetTransform(rCenterX, rCenterY, rCenterDx, rCenterDy, rRotation, rGlobalScale, -rGlobalScale);
}

bool FrameFunc()
{
	PadManager.Update();

	float rDeltaTime =hge->Timer_GetDelta();

	_Game.Update(rDeltaTime);

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
	if (vBallPos.y>(_nScreenSizeY-rGroundY))
	{
		if (vBallSpeed.y<500)
		{
			//reset
			vBallPos =hgeVector(_nScreenSizeX/2.0f, _nScreenSizeY/2.0f);
		}
		else
		{
			vBallPos.y =(_nScreenSizeY-rGroundY);
			vBallSpeed.y =-vBallSpeed.y*0.80f;
		}
	}
	// Collision Walls
	if (vBallPos.x>_nScreenSizeX || vBallPos.x<0)
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
	static float scale=-(0.1f/rSizeY);
	fnt->SetScale(scale);

	float rTextPosY=9;
	for (int i=0; i<PcPadManager::PAD_MAX_ENTRIES; ++i)
	{
		fnt->SetColor(0xFFFFA000);
		fnt->printf(-9, rTextPosY, HGETEXT_LEFT, "%d %s", i, SMARTENUM_GET_STRING(PcPadManager::CtrlIdx, i)+4);

		fnt->SetColor(0xFFFF00A0);
		PcPadManager::CtrlStatus status =PadManager.GetCtrlState((PcPadManager::CtrlIdx)i);
		fnt->printf(-5, rTextPosY, HGETEXT_LEFT, "%d", status);
		rTextPosY-=0.3f;
	}
}

bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);


	_Game.Render();

	//DrawInputs();

	/*
	fnt->printf(10, 40.0f, HGETEXT_LEFT, "%d", int(1.0f/hge->Timer_GetDelta()) );

	// gnd
	hge->Gfx_RenderLine(0, _nScreenSizeY-rGroundY, _nScreenSizeX, _nScreenSizeY-rGroundY, 0xFFFFFFFF);

	// char
	hge->Gfx_RenderBox(rPosX-rCharSizeX, _nScreenSizeY-(rGroundY+5), rPosX+rCharSizeX, _nScreenSizeY-(rGroundY+rCharSizeY), 0xFFFF0000);

	// racket
	float rRacketY =_nScreenSizeY-(rGroundY+rCharRacketY);
	hge->Gfx_RenderLine(rPosX, rRacketY, rPosX+vRacketDir.x*rRacketLen, rRacketY+vRacketDir.y*rRacketLen, 0xFF60FF60);

	// ball
	hge->Gfx_RenderBox(vBallPos.x-5, vBallPos.y-5, vBallPos.x+5, vBallPos.y+5, 0xFFFFFF00);
	*/

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
	hge->System_SetState(HGE_SCREENWIDTH,	_nScreenSizeX);
	hge->System_SetState(HGE_SCREENHEIGHT,	_nScreenSizeY);
	hge->System_SetState(HGE_TITLE,			"Off the Ball");
	hge->System_SetState(HGE_WINDOWED,		_bWindowed);

	// Tries to initiate HGE with the states set. If something goes wrong, "false" is returned
	// and more specific description of what have happened can be read with System_GetErrorMessage().
	if (hge->System_Initiate())
	{
		HWND hwnd =hge->System_GetState(HGE_HWND);
		PadManager.Init(hwnd);

		// Load a font
		fnt=new hgeFont("Data/font1.fnt");
		fnt->SetScale(-(1.0f/rGlobalScale));
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
