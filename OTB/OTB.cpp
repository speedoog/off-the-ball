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
#include "..\HGE\hgeparticle.h"
#include "Game.h"
#include "xml/XMLParser.h"

HGE*			hge =0;
// hgeSprite*			spt =0;
// hgeParticleSystem*	par =0;
// HTEXTURE			tex;

Game			_Game;

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

static float rTop	=8.0f;
static float rBottom=-0.5f;
float rSizeY =rTop-rBottom;

void ApplyWorldTransform() 
{
	static float rCenterX 	=0;			// center of scale & rotation
	static float rCenterY 	=rBottom+(rSizeY/2.0f);
	static float rCenterDx	=float(_nScreenSizeX)/2.0f;
	static float rCenterDy	=float(_nScreenSizeY)/2.0f;
	static float rRotation	=0;
//	rRotation+=0.02f;

	float rGlobalScale =float(_nScreenSizeY)/rSizeY;

	// Transformations are applied in this order: first scaling, then rotation and finally displacement. 
	hge->Gfx_SetTransform(rCenterX, rCenterY, rCenterDx, rCenterDy, rRotation, rGlobalScale, -rGlobalScale);
}

bool FrameFunc()
{
	const float rDeltaTime =hge->Timer_GetDelta();

	_Game.Update(rDeltaTime);

	// Exit w/ Esc
	return (hge->Input_GetKeyState(HGEK_ESCAPE));
}


void DrawInputs() 
{
	PcPadManager& PadManager =_Game.GetPadManager();
	hgeFont*	  pFont		 =_Game.GetResources()._pFontDebug;

	float rTextPosY=9;
	for (int i=0; i<PcPadManager::PAD_MAX_ENTRIES; ++i)
	{
		pFont->SetColor(0xFFFFA000);
		pFont->printf(-9, rTextPosY, HGETEXT_LEFT, "%d %s", i, SMARTENUM_GET_STRING(PcPadManager::CtrlIdx, i)+4);

		pFont->SetColor(0xFFFF00A0);
		PcPadManager::CtrlStatus status =PadManager.GetCtrlState((PcPadManager::CtrlIdx)i);
		pFont->printf(-5, rTextPosY, HGETEXT_LEFT, "%d", status);
		rTextPosY-=0.3f;
	}
}


bool RenderFunc()
{
	ApplyWorldTransform();

	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	_Game.Render();

//	_Game.GetResources()._pFontDebug->printf(-5.0f, 5.0f, HGETEXT_LEFT, "%d", int(1.0f/hge->Timer_GetDelta()) );

//	DrawInputs();
//	par->Render();

	hge->Gfx_EndScene();

	return false;
}

const char* XML_SECTION_VIDEO			="Video";
const char* XML_ATTRIB_VIDEO_SCREENX	="ScreenSizeX";
const char* XML_ATTRIB_VIDEO_SCREENY	="ScreenSizeY";
const char* XML_ATTRIB_VIDEO_WINDOWED	="Windowed";

const int  DEFAULT_SCREENSIZEX 		=1280;
const int  DEFAULT_SCREENSIZEY 		=720;
const bool DEFAULT_SCREENWINDOWED   =true;

void LoadSettings()
{
	XML_PARSER Parser;
	XML_PARSER::XMLRC rc =Parser.LoadFromFile("OTB.xml");
	if (rc==XML_PARSER::XP_NO_ERROR)
	{
		XML_ELEMENT* pRoot =Parser.GetRoot();

		// Read Video
		{
			XML_ELEMENT* pXmlVideo =pRoot->FindElement(XML_SECTION_VIDEO, false);
			if (pXmlVideo)
			{
				_nScreenSizeX =pXmlVideo->GetAttribute(XML_ATTRIB_VIDEO_SCREENX,  DEFAULT_SCREENSIZEX);
				_nScreenSizeY =pXmlVideo->GetAttribute(XML_ATTRIB_VIDEO_SCREENY,  DEFAULT_SCREENSIZEY);
				_bWindowed	  =pXmlVideo->GetAttribute(XML_ATTRIB_VIDEO_WINDOWED, DEFAULT_SCREENWINDOWED)!=0;
			}
		}
	}
	else
	{
		MessageBox(NULL, "Failed to load OTB.xml", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	LoadSettings();

	hge = hgeCreate(HGE_VERSION);

	// Init HGE
	hge->System_SetState(HGE_FRAMEFUNC,		FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC,	RenderFunc);
	hge->System_SetState(HGE_SHOWSPLASH,	false);
	hge->System_SetState(HGE_SCREENWIDTH,	_nScreenSizeX);
	hge->System_SetState(HGE_SCREENHEIGHT,	_nScreenSizeY);
	hge->System_SetState(HGE_TITLE,			"Off the Ball");
	hge->System_SetState(HGE_WINDOWED,		_bWindowed);

	hge->System_SetState(HGE_FPS,			HGEFPS_VSYNC);
//	hge->System_SetState(HGE_FPS,			HGEFPS_UNLIMITED);
//	hge->System_SetState(HGE_FPS,			60);

	// Tries to initiate HGE
	if (hge->System_Initiate())
	{
		_Game.Init();

		hge->System_Start();

		_Game.Kill();
	}
	else
	{	
		// If HGE initialization failed show error message
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}


	// Now ESC has been pressed or the user has closed the window by other means.
	// Restore video mode and free all allocated resources
	hge->System_Shutdown();

	// Release the HGE interface. If there are no more references, the HGE object will be deleted.
	hge->Release();

	return 0;
}
