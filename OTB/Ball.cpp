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

float rGravity			=-7.0f;
float rRadius			=0.075f;
float rTimescale		=1.0f;
float rGroundResitution	=0.6f;
float rWallResitution	=0.6f;
float rCeilResitution	=0.8f;
float rNetResitution	=0.1f;

// ********************************************
//	Ctor
// ********************************************
Ball::Ball()
: _nSide	(0)
, _bPaused	(false)
, _vVelocity(0,0)
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
void Ball::Reset(Player* pPlayer)
{
	float rPosX =pPlayer->GetPosition().x + pPlayer->GetAt()*0.1f;

	_vPos		=hgeVector(rPosX,1.8f);
	_nSide		=pPlayer->GetPlayerId();
	_vLastPos	=_vPos;

	float rVelX =pPlayer->GetAt()*-1.0f;
	_vVelocity	=hgeVector(rVelX,4);
}

// ********************************************
//	Update
// ********************************************
void Ball::Update(const float rDeltaTime)
{
	if (_bPaused)
		return;

	// update ball physic
	{
		float rBallPhysicDeltaTime =rDeltaTime*rTimescale;
		_vLastPos	=_vPos;
		hgeVector vOldVelocity =_vVelocity;
		_vVelocity.y+=rGravity*rBallPhysicDeltaTime;
		_vPos		+=(vOldVelocity+_vVelocity)*0.5f*rBallPhysicDeltaTime;
	}

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
		const Float32 rDiff  =_vPos.y-level.GetNetY();
		const Bool	  bLet   =TAbs(rDiff)<0.1f && _pGame->GetRules().IsServing()==false;
		const Bool	  bHitNet=rDiff<0.0f && !bLet;

		// ball change side
		if (bHitNet)
		{
			// hit net
			_vVelocity.x *=-rNetResitution;
			_vPos.x=0.0f;
			nSide =_nSide;	// stay same side !
			_pGame->GetRules().EventBallHitNet();
		}
		else
		{
			if (bLet)
			{
				_vVelocity.y =-_vVelocity.y;
			}
			_pGame->GetRules().EventBallChangeSide(nSide);
		}
	}
	_nSide =nSide;
}

// ********************************************
//	RacketHit
// ********************************************
void Ball::RacketHit(const hgeVector& vVelocity)
{
	_vVelocity =vVelocity;
	_pGame->GetRules().EventBallHitRacket();
}

// ********************************************
//	Launch
// ********************************************
void Ball::Launch(const hgeVector& vDir)
{
	hgeVector vVelocity =vDir;
	if (vVelocity.y<0.0f)
	{
		vVelocity.y =0.0f;
	}

	vVelocity.Normalize();
	vVelocity*=1.25f;
	vVelocity+=hgeVector(0.0f, 2.5f);	// y boost
	_vVelocity =vVelocity;
}

// ********************************************
//	Render
// ********************************************
void Ball::Render()
{
	hge->Gfx_RenderCircle(_vPos.x, _vPos.y, rRadius, 0xFFFFFF00);
}
