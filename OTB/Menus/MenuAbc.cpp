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


#include "MenuAbc.h"

#include "MenuItem.h"
#include "../OTB.h"
#include "../../HGE/hgecolor.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
MenuAbc::MenuAbc()
: _pGUI(NULL)
{
}

// ****************************************************************************************
//	Init
// ****************************************************************************************
void MenuAbc::Init()
{
	_pGUI =new hgeGUI();
	_pGUI->SetNavMode(HGEGUI_UPDOWN | HGEGUI_CYCLED);
//	_pGUI->SetCursor(spr);

	StartMenuMain();
}

// ****************************************************************************************
//	Kill
// ****************************************************************************************
void MenuAbc::Kill()
{
	delete _pGUI;
	_pGUI =NULL;
}

// ****************************************************************************************
//	ClearMenu
// ****************************************************************************************
void MenuAbc::ClearMenu()
{
	_pGUI->Leave();
	_pGUI->Clear();
	_rCurrentY =GetMenuPosY()-0.6f;
}

// ****************************************************************************************
//	AddMenuItem
// ****************************************************************************************
void MenuAbc::AddMenuItem(const UInt32 ItemId, const TString& sLabel)
{
	_pGUI->AddCtrl(new hgeGUIMenuItem(ItemId, this, 0.0f, _rCurrentY, sLabel.GetCharconst()) );
	_rCurrentY -=0.4f;
}

// ****************************************************************************************
//	GetMenuPosY
// ****************************************************************************************
Float32	MenuAbc::GetMenuPosY()
{
	Otb* pOTB =Otb::GetInstance();
	return pOTB->GetGame().GetLevel().GetSize().y*0.75f;
}

// ****************************************************************************************
//	RenderBackground
// ****************************************************************************************
void MenuAbc::RenderBackground()
{
	RenderQuad( hgeVector(-50.0f, -50.0f), hgeVector(50.0f, 50.0f), hgeColorRGB(0.0f, 0.0f, 0.0f, 0.4f));

	RenderQuad( hgeVector(-3.0f, 1.0f), hgeVector(3.0f, 7.0f), hgeColorRGB(0.0f, 0.0f, 0.63f, 0.4f));
}

// ****************************************************************************************
//	RenderQuad
// ****************************************************************************************
void MenuAbc::RenderQuad(const hgeVector& vMin, const hgeVector& vMax, const hgeColorRGB& color)
{
	Float32 rXMin 	=vMin.x;
	Float32 rXMax 	=vMax.x;
	Float32 rYMin	=vMin.y;
	Float32 rYMax	=vMax.y;

	hgeQuad quad;
	quad.v[0].x =rXMin;
	quad.v[0].y =rYMin;
	quad.v[0].z =0;
	quad.v[0].tx=0;
	quad.v[0].ty=0;

	quad.v[1].x =rXMin;
	quad.v[1].y =rYMax;
	quad.v[1].z =0;
	quad.v[1].tx=0;
	quad.v[1].ty=1;

	quad.v[2].x =rXMax;
	quad.v[2].y =rYMax;
	quad.v[2].z =0;
	quad.v[2].tx=1;
	quad.v[2].ty=1;

	quad.v[3].x =rXMax;
	quad.v[3].y =rYMin;
	quad.v[3].z =0;
	quad.v[3].tx=1;
	quad.v[3].ty=0;

	quad.blend	=BLEND_DEFAULT;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = color.GetHWColor();

	Otb* pOTB =Otb::GetInstance();
	quad.tex	=pOTB->GetResources()._texPowerBar;

	hge->Gfx_RenderQuad(&quad);
}