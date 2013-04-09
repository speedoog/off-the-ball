/*
** Haaf's Game Engine 1.7
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Tutorial 06 - Creating menus
*/

// In menuitem.cpp/h we define the
// behaviour of our custom GUI control

#include "MenuItem.h"

#include "Menu.h"
#include "../OTB.h"
#include "HGE/hgerect.h"


// #define	MENU_COLOR_NORMAL	0xFFFFE060
// #define	MENU_COLOR_SELECTED	0xFFFFFFFF

#define	MENU_COLOR_NORMAL	(0xFFC0C0C0)
#define	MENU_COLOR_SELECTED	(0xFFFF4040)

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
hgeGUIMenuItem::hgeGUIMenuItem(const Int32 nId, Menu* pMenu, const Float32 rPosx, const Float32 rPosy, const TString& sTitle)
{
	id		=nId;
	_pMenu	=pMenu;

//	_rDelay	=rDelay;
	_sTitle	=sTitle;

// 	_ColCurrent.SetHWColor(0x00000000);
// 	_rTimer	=-1.0f;
// 	_rTimer2=-1.0f;

	bStatic	=false;
	bVisible=true;
	bEnabled=true;
	_bFocused =false;

	hgeFont* pFontMenu =_pMenu->GetOTB()->GetResources()._pFontMenus;

	const Float32 rTextHalfWidth =pFontMenu->GetStringWidth(sTitle.GetCharconst())/2.0f;
	rect.Set(rPosx-rTextHalfWidth, rPosy, rPosx+rTextHalfWidth, rPosy+pFontMenu->GetHeight()*pFontMenu->GetScale());
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void hgeGUIMenuItem::Update(float dt)
{
	/*
	if(_rTimer2 != -1.0f)
	{
		_rTimer2+=dt;
		if(_rTimer2 >= _rDelay+0.1f)
		{
			_ColCurrent=_ColS2+_ColD2;
			_rTimer2=-1.0f;
		}
		else
		{
			if(_rTimer2 < _rDelay)
			{
				_ColCurrent=_ColS2;
			}
			else
			{
				_ColCurrent=_ColS2+_ColD2*(_rTimer2-_rDelay)*10;
			}
		}
	}
	else if(_rTimer != -1.0f)
	{
		_rTimer+=dt;
		if(_rTimer >= 0.2f)
		{
			_ColCurrent=_ColS+_ColD;
			_rTimer=-1.0f;
		}
		else
		{
			_ColCurrent=_ColS+_ColD*_rTimer*5;
		}
	}
	*/
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void hgeGUIMenuItem::Render()
{
	hgeFont* pFontMenu =_pMenu->GetOTB()->GetResources()._pFontMenus;

	hgeColor ColorCurrent =_bFocused?MENU_COLOR_SELECTED:MENU_COLOR_NORMAL;

	pFontMenu->SetColor(ColorCurrent.GetHWColor());
	pFontMenu->Render(rect.x1, rect.y1, HGETEXT_LEFT, _sTitle.GetCharconst());

	//hge->Gfx_RenderBox(rect.x1, rect.y1, rect.x2, rect.y2, _bFocused?MENU_COLOR_SELECTED:MENU_COLOR_NORMAL);

// 	Float32 nSize=rect.y2-rect.y1;
// 	hge->Gfx_RenderBox(rect.x1 - 2.0f*nSize, rect.y1, rect.x1-nSize, rect.y1+nSize, _bFocused?MENU_COLOR_SELECTED:MENU_COLOR_NORMAL);
}

// ****************************************************************************************
//	Enter
//	This method is called when the GUI is about to appear on the screen
// ****************************************************************************************
void hgeGUIMenuItem::Enter()
{
	/*
	hgeColor tcolor2;

	_ColS2.SetHWColor(MENU_COLOR_NORMAL&0x00FFFFFF);
	tcolor2.SetHWColor(MENU_COLOR_NORMAL);
	_ColD2=tcolor2-_ColS2;

	tcolor2.SetHWColor(0x30000000);

	_rTimer2=0.0f;
	*/
}

// ****************************************************************************************
//	Leave
//	Called when the GUI is about to disappear from the screen
// ****************************************************************************************
void hgeGUIMenuItem::Leave()
{
	/*
	hgeColor tcolor2;

	_ColS2.SetHWColor(MENU_COLOR_NORMAL);
	tcolor2.SetHWColor(MENU_COLOR_NORMAL&0x00FFFFFF);
	_ColD2=tcolor2-_ColS2;

	tcolor2.SetHWColor(0x00000000);

	_rTimer2=0.0f;
	*/
}

// ****************************************************************************************
//	IsDone
//	Called to test whether the control have finished it's Enter/Leave animation
// ****************************************************************************************
bool hgeGUIMenuItem::IsDone()
{
//	if (_rTimer2==-1.0f)
	{
		return true;
	}
// 	else 
// 	{
// 		return false;
// 	}
}

// ****************************************************************************************
//	Focus
//	Called when the control receives or loses keyboard input focus
// ****************************************************************************************
void hgeGUIMenuItem::Focus(bool bFocused)
{
	_bFocused =bFocused;

/*
	hgeColor ColorTemp;
	
	if(bFocused)
	{
//		hge->Effect_Play(snd);
		_ColS.SetHWColor(MENU_COLOR_NORMAL);
		ColorTemp.SetHWColor(MENU_COLOR_SELECTED);
	}
	else
	{
		_ColS.SetHWColor(MENU_COLOR_SELECTED);
		ColorTemp.SetHWColor(MENU_COLOR_NORMAL);
	}

	_ColD=ColorTemp-_ColS;
	_rTimer=0.0f;
*/
}

// ****************************************************************************************
//	MouseOver
//	called to notify the control that the mouse cursor has entered or left it's area
// ****************************************************************************************
void hgeGUIMenuItem::MouseOver(bool bOver)
{
	if(bOver)
	{
		gui->SetFocus(id);
	}
}

// ****************************************************************************************
//	MouseOver
//	Called to notify the control that the left mouse button state has changed.
//	If it returns true - the caller will receive the control's ID
// ****************************************************************************************
bool hgeGUIMenuItem::MouseLButton(bool bDown)
{
	if(!bDown)
	{
		return true;
	}
	else 
	{
//		hge->Effect_Play(snd);
		return false;
	}
}

// ****************************************************************************************
//	KeyClick
//	This method is called to notify the control that a key has been clicked.
//	If it returns true - the caller will receive the control's ID
// ****************************************************************************************
bool hgeGUIMenuItem::KeyClick(int key, int chr)
{
	if(key==HGEK_ENTER || key==HGEK_SPACE)
	{
		MouseLButton(true);
		return MouseLButton(false);
	}
	return false;
}