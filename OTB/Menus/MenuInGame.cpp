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


#include "MenuInGame.h"

#include "MenuItem.h"
#include "../OTB.h"
#include "../../HGE/hgecolor.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
MenuInGame::MenuInGame()
: MenuAbc()
{
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void MenuInGame::Update(Float32 dt)
{
	Otb* pOTB =Otb::GetInstance();

	if (_pGUI)
	{
		InputCore& Input =pOTB->GetInputCommand();
		if (Input.GetMenuInput(InputCore::MC_UP)>=0.5f)
		{
			_pGUI->ChangeItem(true);
		}

		if (Input.GetMenuInput(InputCore::MC_DOWN)>=0.5f)
		{
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

// ****************************************************************************************
//	ItemValidate
// ****************************************************************************************
void MenuInGame::ItemValidate(const int id)
{
	Otb*  pOTB =Otb::GetInstance();
	Game& game =pOTB->GetGame();

	switch(id)
	{
		// ------------- Menu Main ------------- 
	case MII_MAIN_RESUME:
		game.SetPaused(false);
		break;

	case MII_MAIN_EXIT:
		pOTB->MainMenu();
		break;

	default:;
	}
}


// ****************************************************************************************
//	ItemCancel
// ****************************************************************************************
void MenuInGame::ItemCancel()
{
	switch(_nMenuCurrent)
	{
	case MS_PAUSE:
		// exit ?
		break;
	}
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void MenuInGame::Render()
{
	if (_pGUI)
	{
		RenderBackground();

		Otb* pOTB =Otb::GetInstance();

		hgeFont* pFontMenu =pOTB->GetResources()._pFontMenus;
		pFontMenu->SetColor(0xFFFFFFFF);
		pFontMenu->Render(0.0f, GetMenuPosY(), HGETEXT_CENTER, SMARTENUM_GET_STRING(MenuScreen, _nMenuCurrent)+3 );

		_pGUI->Render();
	}
}

// ****************************************************************************************
//	StartMenuMain
// ****************************************************************************************
void MenuInGame::StartMenuMain()
{
	ClearMenu();

	AddMenuItem( MII_MAIN_RESUME,	"Resume"	);
	AddMenuItem( MII_MAIN_EXIT,		"Exit"		);

	_pGUI->SetFocus(MII_MAIN_RESUME);
	_pGUI->Enter();

	_nMenuCurrent =MS_PAUSE;
}
