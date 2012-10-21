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
//                   �>   .                                      <�                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////


#include "Level.h"
#include "Game.h"

// ********************************************
//	Ctor
// ********************************************
Level::Level()
{
}

// ********************************************
//	Dtor
// ********************************************
Level::~Level()
{

}

// ********************************************
//	Init
// ********************************************
void Level::Init(Game* pGame, const hgeVector& vSize, const float rInitialNetY)
{
	_pGame			=pGame;
	_vSize		  	=vSize;
	_rInitialNetY 	=rInitialNetY;
	Reset();
}

// ********************************************
//	Kill
// ********************************************
void Level::Kill()
{
}

// ********************************************
//	Reset
// ********************************************
void Level::Reset()
{
	_rNetY	  =_rInitialNetY;
	_rCounter =0.0f;
}

// ********************************************
//	Update
// ********************************************
void Level::Update(const float rDeltaTime)
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

// ********************************************
//	Render
// ********************************************
void Level::Render()
{
	hge->Gfx_RenderLine(-_vSize.x, 0,			 _vSize.x, 0,			0xFFFF5000);	// gnd
	hge->Gfx_RenderLine(-_vSize.x, _vSize.y,	 _vSize.x, _vSize.y,	0xFF00FF00);	// top
	hge->Gfx_RenderLine(-_vSize.x, 0,			-_vSize.x, _vSize.y,	0xFF00FF00);	// left
	hge->Gfx_RenderLine( _vSize.x, 0,			 _vSize.x, _vSize.y,	0xFF00FF00);	// right

	hge->Gfx_RenderLine( 0, 0, 0, _rInitialNetY, 0xFFFFFF00);	// net
	hge->Gfx_RenderLine( 0, _rInitialNetY, 0, _rNetY, 0xFFFF6000);	// net
}
