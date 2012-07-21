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

const float rRacketRotationSpeedMax =8.0f*M_PI;

// ********************************************
//	Ctor
// ********************************************
Player::Player()
{
	_rCharSpeedMax 	=7.0f;
	_vCharSize 		=hgeVector(0.35f, 1.8f);
	_rCharRacketY	=_vCharSize.y*0.75f;
	_rRacketLen		=_vCharSize.y*0.75f;
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

	_vRacketDir =hgeVector(-GetAt(), 0);

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
		hgeVector vInputRacketNorm(_vInputRacket);
		vInputRacketNorm.Normalize();
		
		float rAngleInput	=vInputRacketNorm.Angle();
		float rAngleRacket	=_vRacketDir.Angle();
		float rAngleDiff =rAngleInput-rAngleRacket;
		if (rAngleDiff>(M_PI))		rAngleDiff-=2.0f*M_PI;
		if (rAngleDiff<(-M_PI))		rAngleDiff+=2.0f*M_PI;

		_rRacketRotationSpeed =rAngleDiff*rRacketRotationSpeedMax;
		_vRacketDir.Rotate(_rRacketRotationSpeed*rDeltaTime);
	}
	else
	{
		_rRacketRotationSpeed =0.0f;
	}

	// wait Serve
	Ball& ball =_pGame->GetBall();
	Rules* pRules =&_pGame->GetRules();
	if (pRules->IsWaitingToServe(_nPlayerId))
	{
		ball.Reset(this);				// sync ball
		if (_vInputMove.y>0.15f)						// deadzone
		{
			pRules->EventServeStart();
		}
	}
	else
	{
		// check ball collide
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
		_rCrossLast =rCross;
		if (rCrossMul<0 && _rHitCooldown<=0.0f)
		{
			hgeVector vProjection;
			float rRatio;
			SegmentDist(vRacket0, vRacket1, vBall, &vProjection, &rRatio);
			if (rRatio>0 && rRatio<1)
			{
				hgeVector vHit =vProjection-vBall;
				vHit.Normalize();

				// temp version
				float rRacketSpeedAbs =fabsf(_rRacketRotationSpeed);
				float rImpactSpeed =RfxClamp(rRacketSpeedAbs, 4.0f, 12.0f);

				ball.Hit( vHit*rImpactSpeed);	// test
				_rHitCooldown =0.5f;
			}
		}
		_rHitCooldown -=rDeltaTime;
	}

}

// ********************************************
//	Render
// ********************************************
void Player::Render()
{
	// Player
	hge->Gfx_RenderBox(	_vPos.x-_vCharSize.x, _vPos.y,
						_vPos.x+_vCharSize.x, _vPos.y+_vCharSize.y, 0xFF39588E);

	// Eye test
	float rEyeRadius =_vCharSize.x*0.25f;
	hgeVector vEyeCenter(_vPos.x+_vCharSize.x*0.75f*GetAt(), _vPos.y+_vCharSize.y*0.85f);
	hge->Gfx_RenderCircle(vEyeCenter.x, vEyeCenter.y, rEyeRadius, 0xFFFFFFFF);

// 	Ball& ball =_pGame->GetBall();
// 	const hgeVector& vBallPos =ball.GetPos();
// 	hgeVector vDiff =vBallPos-vEyeCenter;
// 	vDiff.Normalize();
// 	hgeVector vPosIris =vEyeCenter+rEyeRadius*0.666f*vDiff;
// 	hge->Gfx_RenderCircle(vPosIris.x, vPosIris.y, rEyeRadius*0.3333f, 0xFFC20004);

	// Racket
	hge->Gfx_RenderLine(_vPos.x, _rCharRacketY,
						_vPos.x+_vRacketDir.x*_rRacketLen, _rCharRacketY+_vRacketDir.y*_rRacketLen, 0xFF60FF60);
}
