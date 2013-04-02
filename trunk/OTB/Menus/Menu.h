/*


	Menu


*/

#ifndef __MENU_H__
#define __MENU_H__

#pragma once

#include "../Base/Base.h"

class Otb;
class hgeGUI;
class TString;

class Menu
{
public:
	enum MenuItemId
	{
	// Main Menu
		MII_MAIN_PLAY		=1
	,	MII_MAIN_OPTIONS
	,	MII_MAIN_HELP
	,	MII_MAIN_CREDITS
	,	MII_MAIN_EXIT

	// Options Menu
	,	MII_OPTIONS_VIDEO
	,	MII_OPTIONS_AUDIO
	,	MII_OPTIONS_INPUT
	,	MII_OPTIONS_BACK

	// Video Menu
	,	MII_OPTVIDEO_RESOLUTION
	,	MII_OPTVIDEO_WINDOWED
	,	MII_OPTVIDEO_BACK

	// Audio Menu
	,	MII_OPTAUDIO_BACK

	// Input Menu
	,	MII_OPTINPUT_BACK

	// help Menu
	,	MII_HELP_BACK

	// Credits Menu
	,	MII_CREDITS_BACK
	};

	enum MenuScreen
	{
		MS_MAIN		=0
	,	MS_OPTIONS
	,	MS_VIDEO
	,	MS_AUDIO
	,	MS_INPUT
	,	MS_HELP
	,	MS_CREDITS
	};

			Menu();
	void	Init(Otb* pOTB);
	void	Kill();

	void	Render();
	void	Update(float dt);

	inline	Otb*	GetOTB()	{ return _pOTB; }

protected:

	void	ClearMenu();
	void	AddMenuItem(const MenuItemId ItemId, const TString& sLabel);

	void	StartMenuMain();
	void	StartMenuOptions();
	void	StartMenuVideo();
	void	StartMenuAudio();
	void	StartMenuInput();
	void	StartMenuHelp();
	void	StartMenuCredits();

protected:
	Otb*			_pOTB;
	hgeGUI*			_pGUI;

	MenuScreen		_nMenuCurrent;
	Float32			_rCurrentY;
};

#endif //__MENU_H__
