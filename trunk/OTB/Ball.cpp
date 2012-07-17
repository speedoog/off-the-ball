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

float rGravity			=-9.0f;
float rRadius			=0.075f;
float rTimescale		=1.0f;
float rGroundResitution	=0.8f;
float rWallResitution	=0.8f;
float rCeilResitution	=0.8f;
float rNetResitution	=0.1f;

// ********************************************
//	Ctor
// ********************************************
Ball::Ball()
{
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
}

// ********************************************
//	Reset
// ********************************************
void Ball::Reset(int nPlayer)
{
	float rPosX =_pGame->GetPlayer(nPlayer).GetPosition().x;
	if (rPosX<0)
		rPosX-=0.2f;
	else
		rPosX+=0.2f;

	_vPos		=hgeVector(rPosX,2.5);
	_nSide		=nPlayer;
	_vLastPos	=_vPos;

	float rVelX =0.0f;//nPlayer==0?1.0f:-1.0f;
	_vVelocity	=hgeVector(rVelX,5);
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
		_vPos.y =0;
		_vVelocity.y =-rGroundResitution*_vVelocity.y;

		_pGame->GetRules().EventBallHitGround();
	}

	// Collision Walls
	Level& level=_pGame->GetLevel();
	const float rWall =level.GetSize().x;
	if (_vPos.x>rWall)
	{
		_vPos.x =rWall;
		_vVelocity.x =-rWallResitution*_vVelocity.x;
		_pGame->GetRules().EventBallHitWall();
	}
	if (_vPos.x<-rWall)
	{
		_vPos.x =-rWall;
		_vVelocity.x =-rWallResitution*_vVelocity.x;
		_pGame->GetRules().EventBallHitWall();
	}

	// Collision ceil
	const float rCeil =level.GetSize().y;
	if (_vPos.y>rCeil)
	{
		_vPos.y =rCeil;
		_vVelocity.y =-rWallResitution*_vVelocity.y;
	}

	// Collision Net
	int nSide =(_vPos.x<0)?0:1;
	if (nSide!=_nSide)
	{
		// ball change side
		if (_vPos.y<level.GetNetY())
		{
			// hit net
			_vVelocity.x *=-rNetResitution;
			_vPos.x=0.0f;
			nSide =_nSide;	// stay same side !
			_pGame->GetRules().EventBallHitNet();
		}
		else
		{
			_pGame->GetRules().EventBallChangeSide(nSide);
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
	_pGame->GetRules().EventBallHitRacket();
}

// ********************************************
//	Render
// ********************************************
void Ball::Render()
{
	hge->Gfx_RenderCircle(_vPos.x, _vPos.y, rRadius, 0xFFFFFF00);
}
