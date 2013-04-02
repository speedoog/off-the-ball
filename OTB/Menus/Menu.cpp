
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
//	StartMenuMain
// ****************************************************************************************
void Menu::StartMenuMain()
{
	_pGUI->Clear();

	Float32 rPosX =0.0f;
	Float32 rPosY =_pOTB->GetGame().GetLevel().GetSize().y*0.7f;
	Float32 rDelay=0.0f;

	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_PLAY,	this, rPosX, rPosY, "Play")	);		rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_OPTIONS, this, rPosX, rPosY, "Options"));	rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_HELP,	this, rPosX, rPosY, "Help")	);		rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_CREDITS, this, rPosX, rPosY, "Credits"));	rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_EXIT,	this, rPosX, rPosY, "Exit")	);		rPosY -=0.4f;

	_pGUI->SetFocus(MII_MAIN_PLAY);
	_pGUI->Enter();

	_nMenuCurrent =MS_MAIN;
}

// ****************************************************************************************
//	StartMenuOptions
// ****************************************************************************************
void Menu::StartMenuOptions()
{
	_pGUI->Clear();

	Float32 rPosX =0.0f;
	Float32 rPosY =_pOTB->GetGame().GetLevel().GetSize().y*0.7f;
	Float32 rDelay=0.0f;

	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_OPTIONS_VIDEO,	this, rPosX, rPosY, "Video"));	rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_OPTIONS_AUDIO,	this, rPosX, rPosY, "Audio"));	rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_OPTIONS_INPUT,	this, rPosX, rPosY, "Input"));	rPosY -=0.4f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_OPTIONS_BACK,		this, rPosX, rPosY, "Back")	);	rPosY -=0.4f;

	_pGUI->SetFocus(MII_OPTIONS_VIDEO);
	_pGUI->Enter();

	_nMenuCurrent =MS_OPTIONS;
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void Menu::Update(Float32 dt)
{
	if (_pOTB && _pGUI)
	{
		static int lastid=0;

		int id=_pGUI->Update(dt);

		switch(id)
		{
		case MII_MAIN_PLAY:
			_pOTB->GetGame().Kill();
			_pOTB->GetGame().InitByXml(_pOTB, &_pOTB->GetXmlTree());
			Kill();
			break;

		case MII_MAIN_OPTIONS:
			StartMenuOptions();
			break;

		case MII_OPTIONS_BACK:
			StartMenuMain();
			break;

		case MII_MAIN_HELP:
			{
				hge->Gfx_SetDisplayMode(320, 200, 32);
				_pGUI->SetFocus(1);
				_pGUI->Enter();
			}
			break;
		case MII_MAIN_CREDITS:
			hge->Gfx_SetDisplayMode(32, 17, 32);
			break;

//		case 4:
//			_pGUI->SetFocus(1);
//			_pGUI->Enter();
//			break;

		case MII_MAIN_EXIT:
			_pOTB->ExitApp();
			break;

		default:;
		}
	}
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void Menu::Render()
{
	if (_pOTB && _pGUI)
	{
		_pGUI->Render();
	}
}

// hge->Gfx_SetDisplayMode(1280, 720, 32);
// _pGUI->SetFocus(1);
// _pGUI->Enter();
// 
