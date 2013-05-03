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

#ifndef __MENU_ABC_H__
#define __MENU_ABC_H__

#pragma once

#include "../Base/Base.h"
#include "../Base/SmartEnum.h"

class Otb;
class hgeGUI;
class TString;

class MenuAbc
{
public:
					MenuAbc();
			void	Init();
			void	Kill();

	virtual void	Render()					{ }
	virtual void	Update(Float32 dt)			{ }
	virtual void	StartMenuMain()				{ }

protected:
	Float32	GetMenuPosY();

	void	ClearMenu();
	void	AddMenuItem(const UInt32 ItemId, const TString& sLabel);

	void	RenderBackground();

	void	SoundChangeItem();
	void	SoundValidate();
	void	SoundCancel();

protected:
	hgeGUI*		_pGUI;
	Float32		_rCurrentY;
};

#endif //__MENU_ABC_H__
