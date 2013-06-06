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

#include "../HGE/hge.h"
#include "Otb.h"

#include "Base/TList.h"


HGE*	hge	  =NULL;

// OTB Singleton
Otb* Otb::_pOTB =NULL;

// Constants
const char* XML_SECTION_VIDEO			="Video";
const char* XML_ATTRIB_VIDEO_SCREENX	="ScreenSizeX";
const char* XML_ATTRIB_VIDEO_SCREENY	="ScreenSizeY";
const char* XML_ATTRIB_VIDEO_WINDOWED	="Windowed";
const Int32 DEFAULT_SCREENSIZEX 		=1280;
const Int32 DEFAULT_SCREENSIZEY 		=720;
const Bool 	DEFAULT_SCREENWINDOWED   	=true;


// ****************************************************************************************
//	FrameFunc
// ****************************************************************************************
bool FrameFunc()
{
	const Float32 rDeltaTime =hge->Timer_GetDelta();
	return Otb::GetInstance()->Update(rDeltaTime);
}

// ****************************************************************************************
//	RenderFunc
// ****************************************************************************************
bool RenderFunc()
{
	return Otb::GetInstance()->Render();
}

// ****************************************************************************************
//	GfxRestoreFunc
// ****************************************************************************************
bool GfxRestoreFunc()
{
//	Otb::GetInstance()->GetResources().Kill();
//	Otb::GetInstance()->GetResources().Init();
	return false;
}

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Otb::Otb()
: _bExitApp	(false)
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

	_pOTB =this;
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
Otb::~Otb()
{
}

// ****************************************************************************************
//	Start
// ****************************************************************************************
void Otb::Start()
{
	LoadSettings();

	hge = hgeCreate(HGE_VERSION);

	// Init HGE
	hge->System_SetState(HGE_FRAMEFUNC,		FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC,	RenderFunc);
	hge->System_SetState(HGE_GFXRESTOREFUNC,GfxRestoreFunc); 

//	hge->System_SetState(HGE_ZBUFFER,		true);
	hge->System_SetState(HGE_SHOWSPLASH,	false);
	hge->System_SetState(HGE_SCREENWIDTH,	_nScreenSizeX);
	hge->System_SetState(HGE_SCREENHEIGHT,	_nScreenSizeY);
	hge->System_SetState(HGE_TITLE,			"Off the Ball");
	hge->System_SetState(HGE_WINDOWED,		_bWindowed);
//	hge->System_SetState(HGE_LOGFILE,		"HGE.log");

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

		// start
		MainMenu();

		_Audio.MusicSetVolume(0.4f);
		_Audio.MusicLoad("data/music/freezepop_Lazy.mp3");
		_Audio.MusicPlay();

		Bool bSuccess =hge->System_Start();

		_Audio.MusicStop();

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

// ****************************************************************************************
//	MainMenu
// ****************************************************************************************
void Otb::MainMenu()
{
	_Game.Kill();
	_MenuMain.Kill();

	_Game.InitDemoMode();
	_MenuMain.Init();
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
bool Otb::Update(const Float32 rDeltaTime)
{
	_Input.Update();

	_MenuMain.Update(rDeltaTime);
	_Game.Update(rDeltaTime);

	return _bExitApp;
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
bool Otb::Render()
{
	ApplyWorldTransform();

	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	_Game.Render();
	_MenuMain.Render();
	_Input.Render();

//	_Game.GetResources()._pFontDebug->printf(-5.0f, 5.0f, HGETEXT_LEFT, "%d", Int32(1.0f/hge->Timer_GetDelta()) );

//	par->Render();

	hge->Gfx_EndScene();
	return false;
}

// ****************************************************************************************
//	ApplyWorldTransform
// ****************************************************************************************
void Otb::ApplyWorldTransform()
{
	_nScreenSizeX =hge->System_GetState(HGE_SCREENWIDTH);
	_nScreenSizeY =hge->System_GetState(HGE_SCREENHEIGHT);

	const Float32 rSizeY	=_rWorldTxTop-_rWorldTxBottom;
	const Float32 rCenterY 	=_rWorldTxBottom+(rSizeY/2.0f);
	const Float32 rCenterDx	=Float32(_nScreenSizeX)/2.0f;
	const Float32 rCenterDy	=Float32(_nScreenSizeY)/2.0f;

	const Float32 rGlobalScale =Float32(_nScreenSizeY)/rSizeY;

	// Transformations are applied in this order: first scaling, then rotation and finally displacement. 
	hge->Gfx_SetTransform(_rWorldTxCenterX, rCenterY, rCenterDx, rCenterDy, _rWorldTxRotation, rGlobalScale, -rGlobalScale);
}

// ****************************************************************************************
//	LoadSettings
// ****************************************************************************************
void Otb::LoadSettings()
{
	XML_PARSER::XMLRC rc =_XmlTree.LoadFromFile("OTB.xml");
	if (rc==XML_PARSER::XP_NO_ERROR)
	{
		XML_ELEMENT* pRoot =_XmlTree.GetRoot();

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

// ****************************************************************************************
//	RenderQuad
// ****************************************************************************************
void Otb::RenderQuad(const hgeVector& vMin, const hgeVector& vMax, const hgeColorRGB& color, HTEXTURE tex)
{
	Float32 rXMin 	=vMin.x;
	Float32 rXMax 	=vMax.x;
	Float32 rYMin	=vMin.y;
	Float32 rYMax	=vMax.y;

	hgeQuad quad;
	quad.v[0].x =rXMin;
	quad.v[0].y =rYMin;
	quad.v[0].z =0;
	quad.v[0].tx=0;
	quad.v[0].ty=0;

	quad.v[1].x =rXMin;
	quad.v[1].y =rYMax;
	quad.v[1].z =0;
	quad.v[1].tx=0;
	quad.v[1].ty=1;

	quad.v[2].x =rXMax;
	quad.v[2].y =rYMax;
	quad.v[2].z =0;
	quad.v[2].tx=1;
	quad.v[2].ty=1;

	quad.v[3].x =rXMax;
	quad.v[3].y =rYMin;
	quad.v[3].z =0;
	quad.v[3].tx=1;
	quad.v[3].ty=0;

	quad.blend	=BLEND_DEFAULT;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color.GetHWColor();

	quad.tex	=tex;

	hge->Gfx_RenderQuad(&quad);
}

// ****************************************************************************************
//	RenderQuad
// ****************************************************************************************
void Otb::RenderLine(const hgeVector& v0, const hgeVector& v1, const Float32 rZ, const hgeColorRGB& color, const Float32 rSize, HTEXTURE tex)
{
	hgeVector vDir	=v1-v0;
	vDir.Normalize();
	vDir *=rSize;

	hgeQuad quad;
	quad.v[0].x =v0.x-vDir.y;
	quad.v[0].y =v0.y+vDir.x;
	quad.v[0].z =0;
	quad.v[0].tx=0;
	quad.v[0].ty=0;

	quad.v[1].x =v0.x+vDir.y;
	quad.v[1].y =v0.y-vDir.x;
	quad.v[1].z =0;
	quad.v[1].tx=0;
	quad.v[1].ty=1;

	quad.v[2].x =v1.x+vDir.y;
	quad.v[2].y =v1.y-vDir.x;
	quad.v[2].z =0;
	quad.v[2].tx=1;
	quad.v[2].ty=1;

	quad.v[3].x =v1.x-vDir.y;
	quad.v[3].y =v1.y+vDir.x;
	quad.v[3].z =0;
	quad.v[3].tx=1;
	quad.v[3].ty=0;

	quad.blend	=BLEND_DEFAULT_Z;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color.GetHWColor();

	quad.tex	=tex;

	hge->Gfx_RenderQuad(&quad);
}

// ****************************************************************************************
//	RenderDisk
// ****************************************************************************************
void Otb::RenderDisk(const hgeVector& vCenter, const Float32 rRadius, const Float32 rZ, const hgeColorRGB& color)
{
	hgeTriple	triple;
	triple.blend =BLEND_DEFAULT_Z;
	triple.tex	 =NULL;
	for (UInt32 i=0; i<3; ++i)
	{
		triple.v[i].x 	=vCenter.x;
		triple.v[i].y 	=vCenter.y;
		triple.v[i].z 	=rZ;
		triple.v[i].tx	=0;
		triple.v[i].ty	=0;
		triple.v[i].col	=color.GetHWColor();
	}

	const int nSections=8;
	Float32 rAngleInc =(2*M_PI)/Float32(nSections);

	Float32 rAngle=0.0f;
	Float32 rX=rRadius*sinf(rAngle)+vCenter.x;
	Float32 rY=rRadius*cosf(rAngle)+vCenter.y;
	for (int i=0; i<nSections; ++i)
	{
		rAngle+=rAngleInc;
		Float32 rX2=rRadius*sinf(rAngle)+vCenter.x;
		Float32 rY2=rRadius*cosf(rAngle)+vCenter.y;

		triple.v[1].x =rX;
		triple.v[1].y =rY;

		triple.v[2].x =rX2;
		triple.v[2].y =rY2;

		hge->Gfx_RenderTriple(&triple);

		rX =rX2;
		rY =rY2;
	}
}
