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


#include "MenuMain.h"

#include "MenuItem.h"
#include "../OTB.h"
#include "../../HGE/hgecolor.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
MenuMain::MenuMain()
: MenuAbc()
{
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void MenuMain::Update(Float32 dt)
{
	Otb* pOTB =Otb::GetInstance();

	if (_pGUI)
	{
		// Title
		{
			hgeFont* pFont=pOTB->GetResources()._pFontTitle;
			static float rHue =0.0f;
			rHue+=dt*0.1f; if (rHue>1.0f) rHue-=1.0f;
			hgeColorHSV colTitle(rHue, 0.9f, 0.8f, 1.0f);
			pFont->SetColor(colTitle.GetHWColor());
		}

		InputCore& Input =pOTB->GetInputCommand();
		if (Input.GetMenuInput(InputCore::MC_UP)>=0.5f)
		{
			SoundChangeItem();
			_pGUI->ChangeItem(true);
		}

		if (Input.GetMenuInput(InputCore::MC_DOWN)>=0.5f)
		{
			SoundChangeItem();
			_pGUI->ChangeItem(false);
		}

		if (Input.GetMenuInput(InputCore::MC_VALIDATE)>=0.5f)
		{
			ItemValidate(_pGUI->GetFocus());
		}
		else if (Input.GetMenuInput(InputCore::MC_CANCEL)>0.5f || hge->Input_KeyDown(HGEK_ESCAPE))
		{
			ItemCancel();
		}
		else
		{
			int id=_pGUI->Update(dt);
			ItemValidate(id);
		}
	}
}

// hge->Gfx_SetDisplayMode(1280, 720, 32);
// _pGUI->SetFocus(1);
// _pGUI->Enter();
// 


// hge->Gfx_SetDisplayMode(320, 200, 32);
// _pGUI->SetFocus(1);
// _pGUI->Enter();

//hge->Gfx_SetDisplayMode(32, 17, 32);

// ****************************************************************************************
//	ItemValidate
// ****************************************************************************************
void MenuMain::ItemValidate(const int id)
{
	Otb*  pOTB =Otb::GetInstance();
	Game& game =pOTB->GetGame();

	switch(id)
	{
	case 0:
		return;

		// ------------- Menu Main ------------- 
	case MII_MAIN_START:
		StartMenuStart();
		break;

	case MII_MAIN_OPTIONS:
		StartMenuOptions();
		break;

	case MII_MAIN_HELP:
		StartMenuHelp();
		break;

	case MII_MAIN_CREDITS:
		StartMenuCredits();
		break;

	case MII_MAIN_EXIT:
		pOTB->ExitApp();
		break;

		// ------------- Start Options ------------- 
	case MII_START_SINGLE:
		game.Kill();
		game.InitSingle();
		Kill();
		break;
	case MII_START_VS:
		game.Kill();
		game.InitVs();
		Kill();
		break;
	case MII_START_CPU_TRAINNING:
		game.Kill();
		game.InitTrainingMode();
		Kill();
		break;

		// ------------- Menu Options ------------- 
	case MII_OPTIONS_VIDEO:
		StartMenuVideo();
		break;
	case MII_OPTIONS_AUDIO:
		StartMenuAudio();
		break;
	case MII_OPTIONS_INPUT:
		StartMenuInput();
		break;

		// ------------- Menu Video ------------- 
	case MII_OPTVIDEO_RESOLUTION:
		break;

	case MII_OPTVIDEO_WINDOWED:
		break;

		// ------------- Menu Audio ------------- 
	case MII_OPTAUDIO_MUSIC:
		break;
	case MII_OPTAUDIO_EFFECTS:
		break;

// 	case MII_OPTAUDIO_BACK:
// 		StartMenuOptions();
// 		break;

		// ------------- Menu Input ------------- 
// 	case MII_OPTINPUT_BACK:
// 		StartMenuOptions();
// 		break;

		// ------------- Menu Help ------------- 
// 	case MII_HELP_BACK:
// 		StartMenuMain();
// 		break;

		// ------------- Menu Credits ------------- 
// 	case MII_CREDITS_BACK:
// 		StartMenuMain();
// 		break;

	default:;
	}

	SoundValidate();
}


// ****************************************************************************************
//	ItemCancel
// ****************************************************************************************
void MenuMain::ItemCancel()
{
	SoundCancel();

	switch(_nMenuCurrent)
	{
	case MS_MAIN:
		// exit ?
		break;

	case MS_START:
	case MS_OPTIONS:
	case MS_HELP:
	case MS_CREDITS:
		StartMenuMain();
		break;

	case MS_VIDEO:
	case MS_AUDIO:
	case MS_INPUT:
		StartMenuOptions();
		break;
	}
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void MenuMain::Render()
{
	if (_pGUI)
	{
		RenderBackground();

		Otb* pOTB =Otb::GetInstance();
		Resources& resources =pOTB->GetResources();
		const hgeVector& vLevelSize =pOTB->GetGame().GetLevel().GetSize();

		// Title
		{
			hgeFont* pFont=resources._pFontTitle;
			float rPosY =vLevelSize.y;
			pFont->printf(0.0f, rPosY*0.96f, HGETEXT_CENTER, "Off  the  ball");
		}

		hgeFont* pFontMenu =resources._pFontMenus;
		pFontMenu->SetColor(0xFFFFFFFF);
		pFontMenu->Render(0.0f, GetMenuPosY(), HGETEXT_CENTER, SMARTENUM_GET_STRING(MenuScreen, _nMenuCurrent)+3 );

		_pGUI->Render();

		if (_nMenuCurrent==MS_HELP)
		{
			const Float32 rScalePad =1.0f/250.0f;
			hgeVector vCenter(0.0f, vLevelSize.y*0.5f);
			hgeVector vHalfSize(vLevelSize.x, vLevelSize.y);
			hgeVector vHalfSizeScaled =vHalfSize*rScalePad;
// 			resources._pSpritePadXbox->Render4V( vCenter.x-vHalfSizeScaled.x, vCenter.y+vHalfSizeScaled.y,
// 												 vCenter.x+vHalfSizeScaled.x, vCenter.y+vHalfSizeScaled.y,
// 												 vCenter.x+vHalfSizeScaled.x, vCenter.y-vHalfSizeScaled.y,
// 												 vCenter.x-vHalfSizeScaled.x, vCenter.y-vHalfSizeScaled.y );

			resources._pSpritePadXbox->RenderEx(vCenter.x, vCenter.y, 0.0f, rScalePad, -rScalePad);

			InputCore& inputCore =pOTB->GetInputCommand();
			hgeVector vInputLeft  =inputCore.GetAxisLeft(0);
			hgeVector vInputRigth =inputCore.GetAxisRight(0);

// 			Player& player =pOTB->GetGame().GetPlayer(0);
// 			hgeVector vInputLeft  =player.GetInputMove();
// 			hgeVector vInputRigth =player.GetInputRacket();

			static hgeVector vOffsetLeft(-1.79f, 0.8f);
			static hgeVector vOffsetRight(0.88f, -0.25f);
			static Float32	 rPadAmplitude =0.2f;
			resources._pSpritePadStick->RenderEx(	vCenter.x+vOffsetLeft.x+vInputLeft.x*rPadAmplitude,
													vCenter.y+vOffsetLeft.y+vInputLeft.y*rPadAmplitude,
													0.0f, rScalePad, -rScalePad);

			resources._pSpritePadStick->RenderEx(	vCenter.x+vOffsetRight.x+vInputRigth.x*rPadAmplitude,
													vCenter.y+vOffsetRight.y+vInputRigth.y*rPadAmplitude,
													0.0f, rScalePad, -rScalePad);


// 			hgeVector vHalfSizeScaled2 =vHalfSize*rScalePad*0.1f;
// 			resources._pSpritePadStick->Render4V( vCenter.x-vHalfSizeScaled2.x, vCenter.y+vHalfSizeScaled2.y,
// 												 vCenter.x+vHalfSizeScaled2.x, vCenter.y+vHalfSizeScaled2.y,
// 												 vCenter.x+vHalfSizeScaled2.x, vCenter.y-vHalfSizeScaled2.y,
// 												 vCenter.x-vHalfSizeScaled2.x, vCenter.y-vHalfSizeScaled2.y );


		}
	}
}

// ****************************************************************************************
//	StartMenuMain
// ****************************************************************************************
void MenuMain::StartMenuMain()
{
	ClearMenu();

	AddMenuItem( MII_MAIN_START,	"Start"		);
	AddMenuItem( MII_MAIN_OPTIONS,	"Options"	);
	AddMenuItem( MII_MAIN_HELP,		"Help"		);
	AddMenuItem( MII_MAIN_CREDITS,	"Credits"	);
	AddMenuItem( MII_MAIN_EXIT,		"Exit"		);

	_pGUI->SetFocus(MII_MAIN_START);
	_pGUI->Enter();

	_nMenuCurrent =MS_MAIN;
}

// ****************************************************************************************
//	StartMenuStart
// ****************************************************************************************
void MenuMain::StartMenuStart()
{
	ClearMenu();

	AddMenuItem( MII_START_SINGLE,			"Single"		);
	AddMenuItem( MII_START_VS,				"Vs"			);
	AddMenuItem( MII_START_CPU_TRAINNING,	"CPU Training"	);

	_pGUI->SetFocus(MII_START_SINGLE);
	_pGUI->Enter();

	_nMenuCurrent =MS_START;
}

// ****************************************************************************************
//	StartMenuOptions
// ****************************************************************************************
void MenuMain::StartMenuOptions()
{
	ClearMenu();

	AddMenuItem( MII_OPTIONS_VIDEO,	"Video"		);
	AddMenuItem( MII_OPTIONS_AUDIO,	"Audio"		);
	AddMenuItem( MII_OPTIONS_INPUT,	"Input"		);

	_pGUI->SetFocus(MII_OPTIONS_VIDEO);
	_pGUI->Enter();

	_nMenuCurrent =MS_OPTIONS;
}

// ****************************************************************************************
//	StartMenuVideo
// ****************************************************************************************
void MenuMain::StartMenuVideo()
{
	ClearMenu();

	AddMenuItem( MII_OPTVIDEO_RESOLUTION,	"Resolution"	);
	AddMenuItem( MII_OPTVIDEO_WINDOWED,		"Windowed"		);

	_pGUI->SetFocus(MII_OPTVIDEO_RESOLUTION);
	_pGUI->Enter();

	_nMenuCurrent =MS_VIDEO;
}

// ****************************************************************************************
//	StartMenuAudio
// ****************************************************************************************
void MenuMain::StartMenuAudio()
{
	ClearMenu();

	AddMenuItem( MII_OPTAUDIO_MUSIC,	"Music"	);
	AddMenuItem( MII_OPTAUDIO_EFFECTS,	"Sfx"	);

	_pGUI->SetFocus(MII_OPTAUDIO_MUSIC);
	_pGUI->Enter();

	_nMenuCurrent =MS_AUDIO;
}

// ****************************************************************************************
//	StartMenuInput
// ****************************************************************************************
void MenuMain::StartMenuInput()
{
	ClearMenu();

//	AddMenuItem( MII_OPTINPUT_BACK,			"Back"			);

//	_pGUI->SetFocus(MII_OPTINPUT_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_INPUT;
}

// ****************************************************************************************
//	StartMenuHelp
// ****************************************************************************************
void MenuMain::StartMenuHelp()
{
	ClearMenu();

//	AddMenuItem( MII_HELP_BACK,			"Back"			);

//	_pGUI->SetFocus(MII_HELP_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_HELP;
}

// ****************************************************************************************
//	StartMenuCredits
// ****************************************************************************************
void MenuMain::StartMenuCredits()
{
	ClearMenu();

//	AddMenuItem( MII_CREDITS_BACK,			"Back"			);

//	_pGUI->SetFocus(MII_CREDITS_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_CREDITS;
}
