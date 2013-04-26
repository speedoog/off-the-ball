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


#include "Level.h"
#include "Game.h"
#include "../OTB.h"

const UInt32 nColorLevelGround	=0xFFFF5000;
const UInt32 nColorLevelSides	=0xFF00FF00;
const UInt32 nColorLevelNet		=0xFFFFFF00;
const UInt32 nColorLevelNetGrow	=0xFFFF6000;

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Level::Level()
{
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
Level::~Level()
{

}

// ****************************************************************************************
//	Init
// ****************************************************************************************
void Level::Init(Game* pGame, const hgeVector& vSize, const Float32 rInitialNetY)
{
	_pGame			=pGame;
	_vSize		  	=vSize;
	_rInitialNetY 	=rInitialNetY;
	Reset();
}

// ****************************************************************************************
//	Kill
// ****************************************************************************************
void Level::Kill()
{
}

// ****************************************************************************************
//	Reset
// ****************************************************************************************
void Level::Reset()
{
	_rNetY	  =_rInitialNetY;
	_rCounter =0.0f;
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void Level::Update(const Float32 rDeltaTime)
{
	if (_pGame->GetRules().IsWaitingToServe()==false)
	{
		_rCounter +=rDeltaTime;
		if (_rCounter>15.0f)
		{
			_rNetY +=rDeltaTime*0.05f;
		}
	}
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void Level::Render()
{
	hgeColorRGB	colGround;	colGround.SetHWColor(nColorLevelGround);	
	hgeColorRGB	colSides;	colSides.SetHWColor(nColorLevelSides);		
	hgeColorRGB	colNet;		colNet.SetHWColor(nColorLevelNet);			
	hgeColorRGB	colNetGrow;	colNetGrow.SetHWColor(nColorLevelNetGrow);

	hge->Gfx_RenderLine(-_vSize.x, 0,			 _vSize.x, 0,			colGround.GetHWColor());	// gnd
	hge->Gfx_RenderLine(-_vSize.x, _vSize.y,	 _vSize.x, _vSize.y,	colSides.GetHWColor());		// top
	hge->Gfx_RenderLine(-_vSize.x, 0,			-_vSize.x, _vSize.y,	colSides.GetHWColor());		// left
	hge->Gfx_RenderLine( _vSize.x, 0,			 _vSize.x, _vSize.y,	colSides.GetHWColor());		// right

	Otb::RenderLine(hgeVector(0,0),				hgeVector(0,_rInitialNetY), 0.0f, colNet.GetHWColor(),	 0.02f);
	Otb::RenderLine(hgeVector(0,_rInitialNetY), hgeVector(0,_rNetY),		0.0f, colNetGrow.GetHWColor(), 0.02f);
//	hge->Gfx_RenderLine( 0, 0,				0, _rInitialNetY,	colNet.GetHWColor());		// net
//	hge->Gfx_RenderLine( 0, _rInitialNetY,	0, _rNetY,			colNetGrow.GetHWColor());	// net growing part
}
