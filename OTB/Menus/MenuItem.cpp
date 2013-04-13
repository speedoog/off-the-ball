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

#include "MenuItem.h"

#include "MenuMain.h"
#include "../OTB.h"
#include "HGE/hgerect.h"


#define	MENU_COLOR_NORMAL	(0xFFC0C0C0)
#define	MENU_COLOR_SELECTED	(0xFFFF4040)

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
hgeGUIMenuItem::hgeGUIMenuItem(const Int32 nId, MenuMain* pMenu, const Float32 rPosx, const Float32 rPosy, const TString& sTitle)
{
	id		=nId;
	_pMenu	=pMenu;

	_sTitle	=sTitle;

	bStatic	=false;
	bVisible=true;
	bEnabled=true;
	_bFocused =false;

	Otb* pOTB =Otb::GetInstance();
	hgeFont* pFontMenu =pOTB->GetResources()._pFontMenus;

	const Float32 rTextHalfWidth =pFontMenu->GetStringWidth(sTitle.GetCharconst())/2.0f;
	rect.Set(rPosx-rTextHalfWidth, rPosy, rPosx+rTextHalfWidth, rPosy+pFontMenu->GetHeight()*pFontMenu->GetScale());
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void hgeGUIMenuItem::Update(float dt)
{
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void hgeGUIMenuItem::Render()
{
	Otb* pOTB =Otb::GetInstance();
	hgeFont* pFontMenu =pOTB->GetResources()._pFontMenus;

	hgeColor ColorCurrent =_bFocused?MENU_COLOR_SELECTED:MENU_COLOR_NORMAL;

	pFontMenu->SetColor(ColorCurrent.GetHWColor());
	pFontMenu->Render(rect.x1, rect.y1, HGETEXT_LEFT, _sTitle.GetCharconst());

// 	hge->Gfx_RenderBox(rect.x1, rect.y1, rect.x2, rect.y2, _bFocused?MENU_COLOR_SELECTED:MENU_COLOR_NORMAL);

//	Float32 nSize=rect.y2-rect.y1;
//	hge->Gfx_RenderBox(rect.x1 - 2.0f*nSize, rect.y1, rect.x1-nSize, rect.y1+nSize, _bFocused?MENU_COLOR_SELECTED:MENU_COLOR_NORMAL);
}

// ****************************************************************************************
//	Enter
//	This method is called when the GUI is about to appear on the screen
// ****************************************************************************************
void hgeGUIMenuItem::Enter()
{
}

// ****************************************************************************************
//	Leave
//	Called when the GUI is about to disappear from the screen
// ****************************************************************************************
void hgeGUIMenuItem::Leave()
{
}

// ****************************************************************************************
//	IsDone
//	Called to test whether the control have finished it's Enter/Leave animation
// ****************************************************************************************
bool hgeGUIMenuItem::IsDone()
{
	return true;
}

// ****************************************************************************************
//	Focus
//	Called when the control receives or loses keyboard input focus
// ****************************************************************************************
void hgeGUIMenuItem::Focus(bool bFocused)
{
	_bFocused =bFocused;
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
