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

#ifndef __MENU_MAIN_H__
#define __MENU_MAIN_H__

#pragma once

#include "../Base/Base.h"
#include "../Base/SmartEnum.h"

#include "MenuAbc.h"

class Otb;
class hgeGUI;
class TString;

class MenuMain : public MenuAbc
{
public:
	enum MenuItemId
	{
	// Main Menu
		MII_MAIN_START	=1
	,	MII_MAIN_OPTIONS
	,	MII_MAIN_HELP
	,	MII_MAIN_CREDITS
	,	MII_MAIN_EXIT

	// Start Menu
	,	MII_START_SINGLE
	,	MII_START_VS
	,	MII_START_CPU_TRAINNING

	// Options Menu
	,	MII_OPTIONS_VIDEO
	,	MII_OPTIONS_AUDIO
	,	MII_OPTIONS_INPUT

	// Video Menu
	,	MII_OPTVIDEO_RESOLUTION
	,	MII_OPTVIDEO_WINDOWED

	// Audio Menu
	,	MII_OPTAUDIO_MUSIC
	,	MII_OPTAUDIO_EFFECTS

	// Input Menu
//	,	MII_OPTINPUT_BACK

	// help Menu
//	,	MII_HELP_BACK

	// Credits Menu
//	,	MII_CREDITS_BACK
	};

	SMARTENUM_DECLARE(	MenuScreen
						,	MS_MAIN
						,	MS_START
						,	MS_OPTIONS
						,	MS_VIDEO
						,	MS_AUDIO
						,	MS_INPUT
						,	MS_HELP
						,	MS_CREDITS
						);

					MenuMain();

	virtual void	Render();
	virtual void	Update(Float32 dt);

protected:
	virtual void	StartMenuMain();

	void	StartMenuStart();
	void	StartMenuOptions();
	void	StartMenuVideo();
	void	StartMenuAudio();
	void	StartMenuInput();
	void	StartMenuHelp();
	void	StartMenuCredits();

	void	ItemValidate(const int id);
	void	ItemCancel();

protected:
	MenuScreen		_nMenuCurrent;
};

#endif //__MENU_MAIN_H__
