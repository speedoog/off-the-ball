
/*


	Menu


*/


#include "Menu.h"

#include "MenuItem.h"
#include "../OTB.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Menu::Menu()
: _pOTB(NULL)
, _pGUI(NULL)
{
}

// ****************************************************************************************
//	Init
// ****************************************************************************************
void Menu::Init(Otb* pOTB)
{
	_pOTB =pOTB;
	_pGUI =new hgeGUI();
	_pGUI->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
//	_pGUI->SetCursor(spr);

	StartMenuMain();
}

// ****************************************************************************************
//	Kill
// ****************************************************************************************
void Menu::Kill()
{
	delete _pGUI;
	_pGUI =NULL;
	_pOTB =NULL;
}

// ****************************************************************************************
//	ClearMenu
// ****************************************************************************************
void Menu::ClearMenu()
{
	_pGUI->Leave();
	_pGUI->Clear();
	_rCurrentY =GetMenuPosY()-0.6f;
}

// ****************************************************************************************
//	AddMenuItem
// ****************************************************************************************
void Menu::AddMenuItem(const MenuItemId ItemId, const TString& sLabel)
{
	_pGUI->AddCtrl(new hgeGUIMenuItem(ItemId, this, 0.0f, _rCurrentY, sLabel.GetCharconst()) );
	_rCurrentY -=0.4f;
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void Menu::Update(Float32 dt)
{
	if (_pOTB && _pGUI)
	{
		// Title
		{
			hgeFont* pFont=GetOTB()->GetResources()._pFontTitle;
			static float rHue =0.0f;
			rHue+=dt*0.1f; if (rHue>1.0f) rHue-=1.0f;
			hgeColorHSV colTitle(rHue, 0.9f, 0.8f, 1.0f);
			pFont->SetColor(colTitle.GetHWColor());
		}

		InputCore& Input =_pOTB->GetInputCommand();
		hgeVector vLeftStick =Input.GetAxisLeft(0);
		if (TAbs(vLeftStick.y)>0.1f)
		{
			_pGUI->ChangeItem((vLeftStick.y>0.0f));
		}

		int id=_pGUI->Update(dt);

		switch(id)
		{
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
			_pOTB->ExitApp();
			break;

			// ------------- Start Options ------------- 
		case MII_START_SINGLE:
			_pOTB->GetGame().Kill();
			_pOTB->GetGame().InitSingle(_pOTB);
			Kill();
			break;
		case MII_START_VS:
			_pOTB->GetGame().Kill();
			_pOTB->GetGame().InitVs(_pOTB);
			Kill();
			break;
		case MII_START_CPU_TRAINNING:
			_pOTB->GetGame().Kill();
			_pOTB->GetGame().InitTrainingMode(_pOTB);
			Kill();
			break;

		case MII_START_POINTS:
			break;

		case MII_START_BACK:
			StartMenuMain();
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
		case MII_OPTIONS_BACK:
			StartMenuMain();
			break;

			// ------------- Menu Video ------------- 
		case MII_OPTVIDEO_RESOLUTION:
			break;

		case MII_OPTVIDEO_WINDOWED:
			break;

		case MII_OPTVIDEO_BACK:
			StartMenuOptions();
			break;

			// ------------- Menu Audio ------------- 
		case MII_OPTAUDIO_MUSIC:
			break;
		case MII_OPTAUDIO_EFFECTS:
			break;

		case MII_OPTAUDIO_BACK:
			StartMenuOptions();
			break;

			// ------------- Menu Input ------------- 
		case MII_OPTINPUT_BACK:
			StartMenuOptions();
			break;

			// ------------- Menu Help ------------- 
		case MII_HELP_BACK:
			StartMenuMain();
			break;

			// ------------- Menu Credits ------------- 
		case MII_CREDITS_BACK:
			StartMenuMain();
			break;

		default:;
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
//	Render
// ****************************************************************************************
void Menu::Render()
{
	if (_pOTB && _pGUI)
	{
		// Title
		{
			hgeFont* pFont=GetOTB()->GetResources()._pFontTitle;
			float rPosY =GetOTB()->GetGame().GetLevel().GetSize().y;
			pFont->printf(0.0f, rPosY*0.96f, HGETEXT_CENTER, "Off  the  wall");
		}

		hgeFont* pFontMenu =GetOTB()->GetResources()._pFontMenus;
		pFontMenu->SetColor(0xFFFFFFFF);
		pFontMenu->Render(0.0f, GetMenuPosY(), HGETEXT_CENTER, SMARTENUM_GET_STRING(MenuScreen, _nMenuCurrent)+3 );

		_pGUI->Render();
	}
}

// ****************************************************************************************
//	StartMenuMain
// ****************************************************************************************
void Menu::StartMenuMain()
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
void Menu::StartMenuStart()
{
	ClearMenu();

	AddMenuItem( MII_START_SINGLE,			"Single"		);
	AddMenuItem( MII_START_VS,				"Vs"			);
	AddMenuItem( MII_START_CPU_TRAINNING,	"CPU Training"	);
	AddMenuItem( MII_START_POINTS,			"Points"		);
	AddMenuItem( MII_START_BACK,			"Back"			);

	_pGUI->SetFocus(MII_START_SINGLE);
	_pGUI->Enter();

	_nMenuCurrent =MS_START;
}

// ****************************************************************************************
//	StartMenuOptions
// ****************************************************************************************
void Menu::StartMenuOptions()
{
	ClearMenu();

	AddMenuItem( MII_OPTIONS_VIDEO,	"Video"		);
	AddMenuItem( MII_OPTIONS_AUDIO,	"Audio"		);
	AddMenuItem( MII_OPTIONS_INPUT,	"Input"		);
	AddMenuItem( MII_OPTIONS_BACK,	"Back"		);

	_pGUI->SetFocus(MII_OPTIONS_VIDEO);
	_pGUI->Enter();

	_nMenuCurrent =MS_OPTIONS;
}

// ****************************************************************************************
//	StartMenuVideo
// ****************************************************************************************
void Menu::StartMenuVideo()
{
	ClearMenu();

	AddMenuItem( MII_OPTVIDEO_RESOLUTION,	"Resolution"	);
	AddMenuItem( MII_OPTVIDEO_WINDOWED,		"Windowed"		);
	AddMenuItem( MII_OPTVIDEO_BACK,			"Back"			);

	_pGUI->SetFocus(MII_OPTVIDEO_RESOLUTION);
	_pGUI->Enter();

	_nMenuCurrent =MS_VIDEO;
}

// ****************************************************************************************
//	StartMenuAudio
// ****************************************************************************************
void Menu::StartMenuAudio()
{
	ClearMenu();

	AddMenuItem( MII_OPTAUDIO_BACK,			"Back"			);

	_pGUI->SetFocus(MII_OPTAUDIO_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_AUDIO;
}

// ****************************************************************************************
//	StartMenuInput
// ****************************************************************************************
void Menu::StartMenuInput()
{
	ClearMenu();

	AddMenuItem( MII_OPTINPUT_BACK,			"Back"			);

	_pGUI->SetFocus(MII_OPTINPUT_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_INPUT;
}

// ****************************************************************************************
//	StartMenuHelp
// ****************************************************************************************
void Menu::StartMenuHelp()
{
	ClearMenu();

	AddMenuItem( MII_HELP_BACK,			"Back"			);

	_pGUI->SetFocus(MII_HELP_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_HELP;
}

// ****************************************************************************************
//	StartMenuCredits
// ****************************************************************************************
void Menu::StartMenuCredits()
{
	ClearMenu();

	AddMenuItem( MII_CREDITS_BACK,			"Back"			);

	_pGUI->SetFocus(MII_CREDITS_BACK);
	_pGUI->Enter();

	_nMenuCurrent =MS_CREDITS;
}

// ****************************************************************************************
//	GetMenuPosY
// ****************************************************************************************
Float32	Menu::GetMenuPosY()
{
	return _pOTB->GetGame().GetLevel().GetSize().y*0.75f;
}
