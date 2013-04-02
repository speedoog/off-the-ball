/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Tutorial 06 - Creating menus
*/

// In menuitem.cpp/h we define the
// behaviour of our custom GUI control

#ifndef __MENU_ITEM_H__
#define __MENU_ITEM_H__

#pragma once

#include "..\..\HGE\hge.h"
#include "..\..\HGE\hgegui.h"
#include "..\..\HGE\hgefont.h"
#include "..\..\HGE\hgecolor.h"
#include "../Base/Base.h"
#include "../Base/TString.h"

class Menu;

class hgeGUIMenuItem : public hgeGUIObject
{
public:
					hgeGUIMenuItem(const Int32 nId, Menu* pMenu, const Float32 rPosx, const Float32 rPosy, const TString& sTitle);

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
	Menu*		_pMenu;
//	Float32		_rDelay;
	TString		_sTitle;
	Bool		_bFocused;

// 	hgeColor	_ColS, _ColD, _ColS2, _ColD2;
// 	hgeColor	_ColCurrent;
// 	Float32		_rTimer, _rTimer2;
};

#endif	//__MENU_ITEM_H__