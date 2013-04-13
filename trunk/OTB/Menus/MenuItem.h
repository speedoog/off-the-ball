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

#ifndef __MENU_ITEM_H__
#define __MENU_ITEM_H__

#pragma once

#include "..\..\HGE\hge.h"
#include "..\..\HGE\hgegui.h"
#include "..\..\HGE\hgefont.h"
#include "..\..\HGE\hgecolor.h"
#include "../Base/Base.h"
#include "../Base/TString.h"

class MenuMain;

class hgeGUIMenuItem : public hgeGUIObject
{
public:
					hgeGUIMenuItem(const Int32 nId, MenuMain* pMenu, const Float32 rPosx, const Float32 rPosy, const TString& sTitle);

	virtual void	Render();
	virtual void	Update(float dt);

	virtual void	Enter();
	virtual void	Leave();
	virtual bool	IsDone();
	virtual void	Focus(bool bFocused);
	virtual void	MouseOver(bool bOver);

	virtual bool	MouseLButton(bool bDown);
	virtual bool	KeyClick(int key, int chr);

private:
	MenuMain*		_pMenu;
	TString		_sTitle;
	Bool		_bFocused;
};

#endif	//__MENU_ITEM_H__