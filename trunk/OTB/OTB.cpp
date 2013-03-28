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
#include "Otb.h"

#include "Base/TList.h"

HGE*			hge	  =0;
Otb*			pThis =NULL;

// Constants
const char* XML_SECTION_VIDEO			="Video";
const char* XML_ATTRIB_VIDEO_SCREENX	="ScreenSizeX";
const char* XML_ATTRIB_VIDEO_SCREENY	="ScreenSizeY";
const char* XML_ATTRIB_VIDEO_WINDOWED	="Windowed";
const int  	DEFAULT_SCREENSIZEX 		=1280;
const int  	DEFAULT_SCREENSIZEY 		=720;
const bool 	DEFAULT_SCREENWINDOWED   	=true;


// ------------ menu test ------------
#include "Menus/menuitem.h"
hgeGUI				*gui;
hgeFont				*fnt;
Bool				_bMenuTest=true;
// ------------ menu test ------------

bool FrameFunc()
{
	const float rDeltaTime =hge->Timer_GetDelta();
	return pThis->Update(rDeltaTime);
}

bool RenderFunc()
{
	return pThis->Render();
}

Otb::Otb()
: _bExitApp				(false)
, _bChangeVideoSettings	(false)
, _Game					(this)
{
	// initial video settings
	_nScreenSizeX 		=DEFAULT_SCREENSIZEX;
	_nScreenSizeY 		=DEFAULT_SCREENSIZEY;
	_bWindowed    		=DEFAULT_SCREENWINDOWED;

	// World Transform
	_rWorldTxTop		=8.0f;
	_rWorldTxBottom		=-0.5f;
	_rWorldTxCenterX	=0.0f;		// center of scale & rotation
	_rWorldTxRotation	=0.0f;

	pThis =this;
}

Otb::~Otb()
{

}

void Otb::Start()
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
	hge->System_SetState(HGE_LOGFILE,		"HGE.log");

	hge->System_SetState(HGE_FPS,			HGEFPS_VSYNC);
//	hge->System_SetState(HGE_FPS,			HGEFPS_UNLIMITED);
//	hge->System_SetState(HGE_FPS,			60);

	// Tries to initiate HGE
	if (hge->System_Initiate())
	{

		// init Low level stuff

		HWND hwnd =hge->System_GetState(HGE_HWND);
		_Input.Init(hwnd);
		_Resources.Init();

		// ------------ menu test ------------
		if (_bMenuTest)
		{
			_Game.InitDemoMode();

			fnt =_Resources._pFontMenus;
			gui=new hgeGUI();

			float rPosY =_Game.GetLevel().GetSize().y*0.7f;
			gui->AddCtrl(new hgeGUIMenuItem(1,fnt,0,rPosY,0.0f,"Play"));		rPosY -=0.4f;
			gui->AddCtrl(new hgeGUIMenuItem(2,fnt,0,rPosY,0.1f,"Options"));		rPosY -=0.4f;
			gui->AddCtrl(new hgeGUIMenuItem(3,fnt,0,rPosY,0.2f,"Help"));		rPosY -=0.4f;
			gui->AddCtrl(new hgeGUIMenuItem(4,fnt,0,rPosY,0.3f,"Credits"));		rPosY -=0.4f;
			gui->AddCtrl(new hgeGUIMenuItem(5,fnt,0,rPosY,0.4f,"Exit"));		rPosY -=0.4f;

			gui->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
//			gui->SetCursor(spr);
			gui->SetFocus(1);
			gui->Enter();
		}
		// ------------ menu test ------------
		else
		{
			_Game.InitByXml(&_XmlParser);
		}

		Bool bSuccess =hge->System_Start();

		_Game.Kill();

		// kill low level
		_Resources.Kill();
		_Input.Kill();

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
}

bool Otb::Update(const float rDeltaTime)
{
	_Input.Update();

	_Game.Update(rDeltaTime);

	// ------------ menu test ------------
	if (_bMenuTest)
	{
		int id=gui->Update(rDeltaTime);
	}
	// ------------ menu test ------------


	// Exit w/ Esc
	return (hge->Input_GetKeyState(HGEK_ESCAPE));
}

bool Otb::Render()
{
	ApplyWorldTransform();

	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	_Game.Render();

	// ------------ menu test ------------
	if (_bMenuTest)
	{
		gui->Render();
	}
	// ------------ menu test ------------

//	_Game.GetResources()._pFontDebug->printf(-5.0f, 5.0f, HGETEXT_LEFT, "%d", int(1.0f/hge->Timer_GetDelta()) );

//	DrawInputs();
//	par->Render();

	hge->Gfx_EndScene();
	return false;
}

void Otb::ApplyWorldTransform()
{
	const float rSizeY		=_rWorldTxTop-_rWorldTxBottom;
	const float rCenterY 	=_rWorldTxBottom+(rSizeY/2.0f);
	const float rCenterDx	=float(_nScreenSizeX)/2.0f;
	const float rCenterDy	=float(_nScreenSizeY)/2.0f;

	const float rGlobalScale =float(_nScreenSizeY)/rSizeY;

	// Transformations are applied in this order: first scaling, then rotation and finally displacement. 
	hge->Gfx_SetTransform(_rWorldTxCenterX, rCenterY, rCenterDx, rCenterDy, _rWorldTxRotation, rGlobalScale, -rGlobalScale);
}

void Otb::DrawInputs()
{
	/*
	InputCommand& Input =_Game.GetInputCommand();
	hgeFont*	  pFont	 =_Game.GetResources()._pFontDebug;

	float rTextPosY=7.6f;
	for (int i=0; i<InputDirectX::PAD_MAX_ENTRIES; ++i)
	{
		pFont->SetColor(0xFFFFA000);
		pFont->printf(-7.4f, rTextPosY, HGETEXT_LEFT, "%d %s", i, SMARTENUM_GET_STRING(InputDirectX::CtrlIdx, i)+4);

		pFont->SetColor(0xFFFF00A0);
		{
			InputDirectX::CtrlStatus status =Input.GetCtrlState(0, (InputDirectX::CtrlIdx)i);
			pFont->printf(-3, rTextPosY, HGETEXT_LEFT, "%d", status);
		}
		{
			InputDirectX::CtrlStatus status =Input.GetCtrlState(1, (InputDirectX::CtrlIdx)i);
			pFont->printf(0, rTextPosY, HGETEXT_LEFT, "%d", status);
		}
		rTextPosY-=0.3f;
	}
	*/
}

void Otb::LoadSettings()
{
	XML_PARSER::XMLRC rc =_XmlParser.LoadFromFile("OTB.xml");
	if (rc==XML_PARSER::XP_NO_ERROR)
	{
		XML_ELEMENT* pRoot =_XmlParser.GetRoot();

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
