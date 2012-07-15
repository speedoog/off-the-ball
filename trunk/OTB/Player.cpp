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
#include "PcPad.h"


// ********************************************
//	Ctor
// ********************************************
Player::Player()
{
	_rCharSpeed 	=5.0f;
	_vCharSize 		=hgeVector(0.35f, 1.8f);
	_rCharRacketY	=_vCharSize.y*0.7f;
	_rRacketLen		=_vCharSize.y*0.7f;
	_vRacketDir 	=hgeVector(0, 1.0f);
}

// ********************************************
//	Dtor
// ********************************************
Player::~Player()
{

}

// ********************************************
//	Reset
// ********************************************
void Player::Init(Game* pGame, const int nPlayerId)
{
	_pGame =pGame;
	_nPlayerId =nPlayerId;

	float rCenterX =pGame->GetLevel().GetSize().y / 2.0f;
	hgeVector vInitPos(0,0);
	if (nPlayerId==0)
	{
		vInitPos.x =-rCenterX;
		_rPosMin =-pGame->GetLevel().GetSize().x+_rRacketLen;
		_rPosMax =-_rRacketLen;
	}
	else
	{
		_rPosMin =_rRacketLen;
		_rPosMax =pGame->GetLevel().GetSize().x-_rRacketLen;
		vInitPos.x =rCenterX;
	}

	SetPosition(vInitPos);
	_rCrossLast		=0.0f;
	_rHitCooldown	=0.0f;
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
	PcPadManager& Pad =_pGame->GetPadManager();

	// Move char
	hgeVector vAxisLeft =Pad.GetAxisLeft();
	if (fabsf(vAxisLeft.x)>0.15f)						// deadzone
	{
		_vPos.x +=vAxisLeft.x*rDeltaTime*_rCharSpeed;
	}

	// check wall/net collision
	if (_vPos.x<_rPosMin)
		_vPos.x =_rPosMin;
	if (_vPos.x>_rPosMax)
		_vPos.x =_rPosMax;

	// Rotate racket
	const hgeVector& vAxisRight =Pad.GetAxisRight();
	if (vAxisRight.Length()>0.3f)						// deadzone
	{
		hgeVector vAxisRightNorm(vAxisRight);
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
			ball.Hit( vHit*10.0f );	// test
			_rHitCooldown =1.0f;
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
	// player
	hge->Gfx_RenderBox(	_vPos.x-_vCharSize.x, _vPos.y,
						_vPos.x+_vCharSize.x, _vPos.y+_vCharSize.y);

// 	Ball& ball =_pGame->GetBall();
// 	hgeVector vCenter =_vPos+hgeVector(0, _rCharRacketY);
// 	hgeVector vBallDiff=vCenter-ball.GetPos();
// 	if (vBallDiff.Length()<_rRacketLen)
// 	{
// 		hge->Gfx_RenderLine(_vPos.x, _rCharRacketY,
// 			_vPos.x+_vRacketDir.x*_rRacketLen, _rCharRacketY+_vRacketDir.y*_rRacketLen, 0xFFFF0000);
// 	}
// 	else

	// racket
	hge->Gfx_RenderLine(_vPos.x, _rCharRacketY,
						_vPos.x+_vRacketDir.x*_rRacketLen, _rCharRacketY+_vRacketDir.y*_rRacketLen, 0xFF60FF60);

}
