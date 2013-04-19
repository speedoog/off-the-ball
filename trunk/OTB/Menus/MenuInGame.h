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

#ifndef __MENU_INGAME_H__
#define __MENU_INGAME_H__

#pragma once

#include "../Base/Base.h"
#include "../Base/SmartEnum.h"
#include "MenuAbc.h"

class Otb;
class hgeGUI;
class TString;

class MenuInGame : public MenuAbc
{
public:
	enum MenuItemId
	{
	// Main Menu
		MII_MAIN_RESUME	=1
	,	MII_MAIN_EXIT
	};

	SMARTENUM_DECLARE(	MenuScreen
						,	MS_PAUSE
						);

			MenuInGame();

	void	Render();
	void	Update(Float32 dt);

protected:
	void	StartMenuMain();

	void	ItemValidate(const int id);
	void	ItemCancel();

protected:
	MenuScreen	_nMenuCurrent;
};

#endif //__MENU_INGAME_H__
