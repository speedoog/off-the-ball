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


#include "PowerBar.h"

#include "Game.h"
#include "../hge/hgeColor.h"

// ********************************************
//	Ctor
// ********************************************
PowerBar::PowerBar()
{
}

// ********************************************
//	Dtor
// ********************************************
PowerBar::~PowerBar()
{
}

// ********************************************
//	Init
// ********************************************
void PowerBar::Init(const UInt32 nPlayerId, Game* pGame)
{
	_pGame		=pGame;
	_nPlayerId	=nPlayerId;
	Reset();
}

// ********************************************
//	Reset
// ********************************************
void PowerBar::Reset()
{
	_rValue =0.0f;
}

// ********************************************
//	Update
// ********************************************
Bool PowerBar::Update(const float rDeltaTime, const Bool bUse)
{
	if (bUse)
	{
		if (_rValue>0.0f)
		{
			_rValue -=rDeltaTime*2.5f;			// go down
			return true;
		}
	}
	else
	{
		_rValue +=rDeltaTime/6.0f;				// go up
		if (_rValue>1.0f)
		{
			_rValue =1.0f;
		}
	}
	return false;
}

// ********************************************
//	ChangeSide
// ********************************************
void PowerBar::ChangeSide()
{
	_rValue +=0.25f;
	if (_rValue>1.0f)
	{
		_rValue =1.0f;
	}
}

// ********************************************
//	Render
// ********************************************
void PowerBar::Render()
{
	Float32 rSide =(_nPlayerId==0)?-1.0f:1.0f;

	Float32 rXMin 	=TChangeRange(1.0f, 0.0f, 1.0f*rSide, 6.0f*rSide, 0.0f);
	Float32 rXMax 	=TChangeRange(1.0f, 0.0f, 1.0f*rSide, 6.0f*rSide, 1.0f);
	Float32 rXValue =TChangeRange(1.0f, 0.0f, 1.0f*rSide, 6.0f*rSide, _rValue);

	hgeColorHSV clHSV(_rValue/3.0f, 1.0f, 0.5, 1.0);

	Float32 rY0 =-0.1f;
	Float32 rY1 =-0.3f;

	hgeQuad quad;
	quad.v[0].x =rXMin;
	quad.v[0].y =rY0;
	quad.v[0].z =0;
	quad.v[0].tx=0;
	quad.v[0].ty=0;

	quad.v[1].x =rXMin;
	quad.v[1].y =rY1;
	quad.v[1].z =0;
	quad.v[1].tx=0;
	quad.v[1].ty=1;

	quad.v[2].x =rXValue;
	quad.v[2].y =rY1;
	quad.v[2].z =0;
	quad.v[2].tx=1;
	quad.v[2].ty=1;

	quad.v[3].x =rXValue;
	quad.v[3].y =rY0;
	quad.v[3].z =0;
	quad.v[3].tx=1;
	quad.v[3].ty=0;

	quad.blend	=BLEND_DEFAULT;
	quad.tex	=_pGame->GetResources()._texPowerBar;
	quad.v[0].col = quad.v[1].col = quad.v[2].col = quad.v[3].col = clHSV.GetHWColor();

	hge->Gfx_RenderQuad(&quad);

	hge->Gfx_RenderBox(rXMin, -0.1f, rXMax, -0.3f, 0xFF808080);	// gnd
}
