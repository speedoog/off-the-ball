/*


	Menu


*/

#ifndef __MENU_H__
#define __MENU_H__

#pragma once

class Otb;
class hgeGUI;

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

			Menu();
	void	Init(Otb* pOTB);
	void	Kill();

	void	Render();
	void	Update(float dt);
protected:
	void	StartMainMenu();

protected:
	Otb*	_pOTB;
	hgeGUI*	_pGUI;
};

#endif //__MENU_H__
