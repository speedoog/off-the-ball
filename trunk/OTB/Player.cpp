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

#include "player.h"

#include "Game.h"

// ********************************************
//	Ctor
// ********************************************
Player::Player()
{
	_rCharSpeedMax 	=5.0f;
	_vCharSize 		=hgeVector(0.35f, 1.8f);
	_rCharRacketY	=_vCharSize.y*0.75f;
	_rRacketLen		=_vCharSize.y*0.75f;
	_vRacketDir 	=hgeVector(0, 1.0f);
	_nScore			=0;
	_vInitialPos	=hgeVector(0,0);
}

// ********************************************
//	Dtor
// ********************************************
Player::~Player()
{

}

// ********************************************
//	Init
// ********************************************
void Player::Init(Game* pGame, const int nPlayerId)
{
	_pGame =pGame;
	_nPlayerId =nPlayerId;

	float rCenterX =pGame->GetLevel().GetSize().y / 2.0f;
	_vInitialPos =hgeVector(0,0);
	if (nPlayerId==0)
	{
		_vInitialPos.x =-rCenterX;
		_rPosMin =-pGame->GetLevel().GetSize().x+_rRacketLen;
		_rPosMax =-_rRacketLen;
	}
	else
	{
		_rPosMin =_rRacketLen;
		_rPosMax =pGame->GetLevel().GetSize().x-_rRacketLen;
		_vInitialPos.x =rCenterX;
	}

	ResetPosition();
}

// ********************************************
//	Reset
// ********************************************
void Player::ResetPosition()
{
	_vPos			=_vInitialPos;
	_vVelocity		=hgeVector(0,0);
	_rCrossLast		=0.0f;
	_rHitCooldown	=0.0f;
	_vInputMove		=hgeVector(0,0);
	_vInputRacket	=hgeVector(0,0);
}

float SegmentDist(const hgeVector& v0, const hgeVector& v1, const hgeVector& p, hgeVector* pProj=NULL, float* pRatio=NULL)
{
	hgeVector vAB =p-v0;
	hgeVector vCD =v1-v0;

	float rDot	 =vAB.Dot(vCD);
	float rLenSq =vCD.LengthSq();
	float rParam =rDot/rLenSq;

	hgeVector vProj;
	if (rParam<0)
	{
		vProj =v0;
	}
	else if (rParam>1)
	{
		vProj =v1;
	}
	else
	{
		vProj =v0+rParam*vCD;
	}

	if (pProj)
		*pProj =vProj;

	if (pRatio)
		*pRatio =rParam;

	return (vProj-p).Length();
}

// ********************************************
//	Update
// ********************************************
void Player::Update(const float rDeltaTime)
{
	// input char
	const float rAcceleration =30.0f;
	const float rDamping =4.0f;
	if (fabsf(_vInputMove.x)>0.15f)						// deadzone
	{
		_vVelocity.x+=_vInputMove.x*rDeltaTime*rAcceleration;
		if (_vVelocity.x>_rCharSpeedMax)		_vVelocity.x =_rCharSpeedMax;
		if (_vVelocity.x<-_rCharSpeedMax)		_vVelocity.x =-_rCharSpeedMax;
	}
	else
	{
//		_vVelocity *=0.99f;	// damping tout pouri
		_vVelocity -=_vVelocity*rDeltaTime*rDamping;
	}
	// Move
	_vPos +=_vVelocity*rDeltaTime;

	// check wall/net collision
	bool bHitWall =false;
	if (_vPos.x<_rPosMin)
	{
		_vPos.x =_rPosMin;
		bHitWall=true;
	}
	if (_vPos.x>_rPosMax)
	{
		_vPos.x =_rPosMax;
		bHitWall =true;
	}
	if (bHitWall)
	{
		_vVelocity.x=-_vVelocity.x*0.3f;
	}

	// Rotate racket
	if (_vInputRacket.Length()>0.3f)						// deadzone
	{
		hgeVector vAxisRightNorm(_vInputRacket);
		vAxisRightNorm.Normalize();
		_vRacketDir =hgeVector(vAxisRightNorm.x, vAxisRightNorm.y);
	}

	// check ball collide
	Ball& ball =_pGame->GetBall();
	hgeVector vBall		=ball.GetPos();
	hgeVector vRacket0 	=_vPos+hgeVector(0, _rCharRacketY);
	hgeVector vRacket1 	=vRacket0+_vRacketDir*_rRacketLen;

	hgeVector vBallDiff=vRacket0-vBall;
	float rCross =0.0f;
	if (vBallDiff.Length()<_rRacketLen)
	{
		hgeVector vRacketD	=vRacket1-vRacket0;
		hgeVector vRacketB	=vBall-vRacket0;
		rCross =vRacketD.Cross(vRacketB);
	}
	
	float rCrossMul =_rCrossLast*rCross;

	if (rCrossMul<0 && _rHitCooldown<=0.0f)
	{
		hgeVector vProjection;
		float rRatio;
		SegmentDist(vRacket0, vRacket1, vBall, &vProjection, &rRatio);
		if (rRatio>0 && rRatio<1)
		{
			hgeVector vHit =vProjection-vBall;
			vHit.Normalize();
			ball.Hit( vHit*10.0f);	// test
			_rHitCooldown =0.5f;
		}
	}

	_rHitCooldown -=rDeltaTime;

	_rCrossLast =rCross;
}

// ********************************************
//	Render
// ********************************************
void Player::Render()
{
	// Player
	hge->Gfx_RenderBox(	_vPos.x-_vCharSize.x, _vPos.y,
						_vPos.x+_vCharSize.x, _vPos.y+_vCharSize.y);

	// Racket
	hge->Gfx_RenderLine(_vPos.x, _rCharRacketY,
						_vPos.x+_vRacketDir.x*_rRacketLen, _rCharRacketY+_vRacketDir.y*_rRacketLen, 0xFF60FF60);
}
