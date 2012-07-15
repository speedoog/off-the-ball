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


#include "Ball.h"
#include "Game.h"

float rGravity	=-9.0f;
float rRadius	=0.075f;
float rTimescale=1.0f;

// ********************************************
//	Ctor
// ********************************************
Ball::Ball()
{
	Reset();
}

// ********************************************
//	Dtor
// ********************************************
Ball::~Ball()
{

}

// ********************************************
//	Reset
// ********************************************
void Ball::Init(Game* pGame)
{
	_pGame =pGame;
	Reset();
}

// ********************************************
//	Reset
// ********************************************
void Ball::Reset()
{
	_vPos		=hgeVector(-5,5);
	_nSide		=0;
	_vLastPos	=_vPos;
	_vVelocity	=hgeVector(3,1);
}

// ********************************************
//	Update
// ********************************************
void Ball::Update(const float rDeltaTime)
{
	float rNewDT =rDeltaTime*rTimescale;
	_vLastPos	=_vPos;
	_vPos		+=_vVelocity*rNewDT;
	_vVelocity.y+=rGravity*rNewDT;

	// Collision GND
	if (_vPos.y<0)
	{
		if (_vVelocity.y>(-5.f))
		{
			_pGame->NewBall();
			return;
		}
		else
		{
			_vPos.y =0;
			_vVelocity.y =-_vVelocity.y*0.80f;		// ground restitution
		}
	}

	// Collision Walls
	Level& level=_pGame->GetLevel();
	const float rWall =level.GetSize().x;
	if ((_vPos.x>rWall) || (_vPos.x<-rWall))
	{
		_vVelocity.x =-_vVelocity.x;
	}

	// Collision Net
	int nSide =(_vPos.x<0);

	if (nSide!=_nSide)
	{
		// ball change side
		if (_vPos.y<level.GetNetY())
		{
			// hit net
			_vVelocity.x *=-0.1f;
			_vPos.x=0.0f;
			nSide =_nSide;	// stay same side !
		}
	}
	_nSide =nSide;
}

// ********************************************
//	Render
// ********************************************
void Ball::Hit(const hgeVector& vVelocity)
{
	_vVelocity =vVelocity;
}

// ********************************************
//	Render
// ********************************************
void Ball::Render()
{
	hge->Gfx_RenderCircle(_vPos.x, _vPos.y, rRadius, 0xFFFFFF00);
}
