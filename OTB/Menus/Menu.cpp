
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

	StartMainMenu();
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
//	StartMainMenu
// ****************************************************************************************
void Menu::StartMainMenu()
{
	hgeFont* pFontMenu =_pOTB->GetResources()._pFontMenus;

	Float32 rPosX =0.0f;
	Float32 rPosY =_pOTB->GetGame().GetLevel().GetSize().y*0.7f;
	Float32 rDelay=0.0f;

	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_PLAY,	pFontMenu, rPosX, rPosY, rDelay, "Play")	);	rPosY -=0.4f; rDelay +=0.05f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_OPTIONS, pFontMenu, rPosX, rPosY, rDelay, "Options"));	rPosY -=0.4f; rDelay +=0.05f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_HELP,	pFontMenu, rPosX, rPosY, rDelay, "Help")	);	rPosY -=0.4f; rDelay +=0.05f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_CREDITS, pFontMenu, rPosX, rPosY, rDelay, "Credits"));	rPosY -=0.4f; rDelay +=0.05f;
	_pGUI->AddCtrl(new hgeGUIMenuItem(MII_MAIN_EXIT,	pFontMenu, rPosX, rPosY, rDelay, "Exit")	);	rPosY -=0.4f; rDelay +=0.05f;

	_pGUI->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
	//	_pGUI->SetCursor(spr);
	_pGUI->SetFocus(1);

	_pGUI->Enter();
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
		if(id == -1)
		{
			// enter new menu
			switch(lastid)
			{
			case MII_MAIN_PLAY:
				_pOTB->GetGame().Kill();
				_pOTB->GetGame().InitByXml(_pOTB, &_pOTB->GetXmlTree());
				Kill();
				break;
			case MII_MAIN_OPTIONS:
				hge->Gfx_SetDisplayMode(500, 500, 32);
				break;
			case 3:
			case 4:
				_pGUI->SetFocus(1);
				_pGUI->Enter();
				break;

			case MII_MAIN_EXIT:
				_pOTB->ExitApp();
				break;

			default:;
			}
			lastid =-1;	// discard
		}
		else if(id)
		{
			// exit current menu
			lastid=id;
			_pGUI->Leave();
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
