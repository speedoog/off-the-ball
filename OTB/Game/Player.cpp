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

const Float32 rRacketInputDeadZone		=0.3f;
const Float32 rRacketRestitution		=0.75f;
const Float32 rRacketOffsetMin			=0.35f;
const Float32 rRacketOffsetMax			=0.35f;
const Float32 rRacketRotationSpeedMax	=4.0f*M_PI;
const Float32 rPlayerStrenght			=0.65f;

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Player::Player()
{
	_rCharSpeedMax 	=7.0f;
	_vCharSize 		=hgeVector(0.35f, 1.8f)*0.75f;
	_rCharRacketY	=_vCharSize.y*0.75f;
	_rRacketLen		=_vCharSize.y*0.6f;
	_nScore			=0;
	_vInitialPos	=hgeVector(0,0);
	_rRacketOffset	=0.0f;
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
Player::~Player()
{

}

// ****************************************************************************************
//	Init
// ****************************************************************************************
void Player::Init(Game* pGame, const Int32 nPlayerId)
{
	_pGame =pGame;
	_nPlayerId =nPlayerId;

	const Float32 rFieldPosRatio =1.0f;		// back of the field

	Float32 rCenterX =pGame->GetLevel().GetSize().y / 2.0f;
	_vInitialPos =hgeVector(0,0);
	if (nPlayerId==0)
	{
		_rPosMin =-pGame->GetLevel().GetSize().x+_rRacketLen;
		_rPosMax =-_rRacketLen;
		_vInitialPos.x =TBlend(_rPosMin, _rPosMax, (1.0f-rFieldPosRatio));
	}
	else
	{
		_rPosMin =_rRacketLen;
		_rPosMax =pGame->GetLevel().GetSize().x-_rRacketLen;
		_vInitialPos.x =TBlend(_rPosMin, _rPosMax, rFieldPosRatio);
	}

//	_vRacketDir =hgeVector(-GetAt(), 0);

	_PowerBar.Init(nPlayerId, pGame);
	_bUseTimeScale =false;

	ResetPosition();
}

// ****************************************************************************************
//	Reset
// ****************************************************************************************
void Player::ResetPosition()
{
	_vPos			=_vInitialPos;
	_vVelocity		=hgeVector(0,0);
	_rHitCooldown	=0.0f;
	_vInputMove		=hgeVector(0,0);
	_vInputRacket	=hgeVector(_nPlayerId==0?-1.0f:1.0f, 0);
	_rRacketOffset	=0.0f;
	_rRacketRotationSpeed =0.0f;
	_vRacketDir =hgeVector(-GetFront(), 0);

	_PowerBar.Reset();
	_bUseTimeScale =false;
}

Float32 SegmentDist(const hgeVector& v0, const hgeVector& v1, const hgeVector& p, hgeVector* pProj=NULL, Float32* pRatio=NULL)
{
	hgeVector vAB =p-v0;
	hgeVector vCD =v1-v0;

	Float32 rDot	 =vAB.Dot(vCD);
	Float32 rLenSq =vCD.LengthSq();
	Float32 rParam =rDot/rLenSq;

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

hgeVector LineProj(const hgeVector& v0, const hgeVector& v1, const hgeVector& p)
{
	hgeVector vAB =p-v0;
	hgeVector vCD =v1-v0;

	Float32 rDot	 =vAB.Dot(vCD);
	Float32 rLenSq =vCD.LengthSq();
	Float32 rParam =rDot/rLenSq;

	hgeVector vProj;
	vProj =v0+rParam*vCD;

	return vProj;
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void Player::Update(const Float32 rDeltaTime)
{
	Rules* pRules =&_pGame->GetRules();
	const Bool bWaitServe =pRules->IsWaitingToServe(_nPlayerId);

	// input char
	if (bWaitServe==false)
	{
		const Float32 rAcceleration =300.0f;
		const Float32 rDamping =40.0f;
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
	}

	// check wall/net collision
	Bool bHitWall =false;
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
	Float32	rInputLen =_vInputRacket.Length();
	if (rInputLen>rRacketInputDeadZone)						// deadzone
	{
		hgeVector vInputRacketNorm(_vInputRacket);
		vInputRacketNorm.Normalize();
		
		Float32 rAngleInput	=vInputRacketNorm.Angle();
		Float32 rAngleRacket	=_vRacketDir.Angle();
		Float32 rAngleDiff =rAngleInput-rAngleRacket;
		if (rAngleDiff>(M_PI))		rAngleDiff-=2.0f*M_PI;
		if (rAngleDiff<(-M_PI))		rAngleDiff+=2.0f*M_PI;

		_rRacketRotationSpeed =rAngleDiff*rRacketRotationSpeedMax;
		_vRacketDir.Rotate(_rRacketRotationSpeed*rDeltaTime);

		_rRacketOffset =TChangeRange(rRacketInputDeadZone, 1.0f, rRacketOffsetMin, rRacketOffsetMax, rInputLen);
	}
	else
	{
		_rRacketRotationSpeed =0.0f;
		_rRacketOffset =rRacketOffsetMin;
	}

	// wait Serve
	Ball& ball =_pGame->GetBall();
	if (bWaitServe)
	{
		ball.Reset(this);					// sync ball
		if (_vInputMove.Length()>0.5f)					// deadzone
		{
			pRules->EventServeStart();
			ball.Launch(_vInputMove);
		}
	}
	//else
	{
		// check ball collide
		hgeVector vBallPos		=ball.GetPos();

		if (_rHitCooldown<=0.0f)
		{
			hgeVector 	vRaquet0=GetRaquetPos0();
			hgeVector 	vRaquet1=GetRaquetPos1();
			hgeVector 	vProj;
			Float32		rProjRatio;
			Float32		rDist =SegmentDist(vRaquet0, vRaquet1, vBallPos, &vProj, &rProjRatio);

			Float32		rBallRadius =ball.GetRadius();

			if (rDist<rBallRadius)
			{
				hgeVector vBallVelocity =ball.GetVelocity();

				// Compute ball bounce
				hgeVector vBallPrev =vBallPos-vBallVelocity;

				hgeVector vBallProj =LineProj(vRaquet0, vRaquet1, vBallPrev);

				hgeVector vBallPrjX =vBallPos-vBallProj;
				hgeVector vBallPrjY =vBallPrev-vBallProj;

				hgeVector vReturnVel =(vBallPrjX+vBallPrjY)*rRacketRestitution;

				// Compute racket Impact
				hgeVector vRacketDirNext =_vRacketDir;
				vRacketDirNext.Rotate(_rRacketRotationSpeed*rDeltaTime);

				hgeVector vRacketNorm =vRacketDirNext-_vRacketDir;
				hgeVector vRacketHit =vRacketNorm;
				vRacketHit *=rPlayerStrenght*(1.0f/rDeltaTime);

//				Float32 rRacketSpeedAbs =TAbs(_rRacketRotationSpeed);
//				Float32 rImpactSpeed	=TClamp(rRacketSpeedAbs, 4.0f, 9.0f);

				vRacketNorm.Normalize();
				Float32 rPlayerVelFactor =TAbs(vRacketNorm.Dot(hgeVector::UNIT_Y));

				hgeVector vNewVel =vReturnVel + vRacketHit+ (_vVelocity*rPlayerVelFactor);

				if (bWaitServe)
				{
					pRules->EventServeStart();
					ball.Launch(hgeVector(0.0f, 0.0f));
				}

				ball.RacketHit(vNewVel);	// test
				_rHitCooldown =0.3f;
			}
		}
		_rHitCooldown -=rDeltaTime;

	}

	if (pRules->IsWaitingToServe()==false)
	{
		// power
		_bUseTimeScale =false;
		if (ball.GetSide()==_nPlayerId)
		{
			InputCore& Input =_pGame->GetInputCommand();
			const Float32 rInputTimeScale =Input.GetCtrlStateFloat(_nPlayerId, InputMapper::PAD_TIME_SCALE);
			if (rInputTimeScale>0.75f)
			{
				_bUseTimeScale =true;
			}
		}
		_bUseTimeScale =_PowerBar.Update(rDeltaTime, _bUseTimeScale);
	}
}

const UInt32 nColorPlayerBody	=0xFF39588E;
const UInt32 nColorPlayerRaquet	=0xFF60FF60;
const UInt32 nColorPlayerEye	=0xFFFFFFFF;

// ****************************************************************************************
//	Render
// ****************************************************************************************
void Player::Render()
{
	hgeColorHSV	colBody;	colBody.SetHWColor(nColorPlayerBody);	
	hgeColorHSV	colRaquet;	colRaquet.SetHWColor(nColorPlayerRaquet);		
	hgeColorHSV	colEye;		colEye.SetHWColor(nColorPlayerEye);			

	if (_pGame->GetDemoMode())
	{
		hgeColorHSV colScale(1.0f, 0.4f, 0.3f, 1.0f);
		colBody		=colBody*colScale;
		colRaquet	=colRaquet*colScale;
		colEye		=colEye*colScale;
	}

	// Player
	hge->Gfx_RenderBox(	_vPos.x-_vCharSize.x, _vPos.y,
						_vPos.x+_vCharSize.x, _vPos.y+_vCharSize.y, colBody.GetHWColor());

	// Eye test
	Float32 rEyeRadius =_vCharSize.x*0.25f;
	hgeVector vEyeCenter(_vPos.x+_vCharSize.x*0.75f*GetFront(), _vPos.y+_vCharSize.y*0.85f);
	hge->Gfx_RenderCircle(vEyeCenter.x, vEyeCenter.y, rEyeRadius, colEye.GetHWColor());

	// Racket
	hgeVector vRaquet0=GetRaquetPos0();
	hgeVector vRaquet1=GetRaquetPos1();

	hge->Gfx_RenderLine(vRaquet0.x, vRaquet0.y,
						vRaquet1.x, vRaquet1.y, colRaquet.GetHWColor());

	if (_pGame->GetTraining())
	{
		_PowerBar.Render();
	}
}

// ****************************************************************************************
//	GetRaquetPos0
// ****************************************************************************************
hgeVector Player::GetRaquetPos0() const
{
	hgeVector vRotationCenter =GetRotationCenter();
	hgeVector vRaquet0=vRotationCenter;
	vRaquet0 +=_vRacketDir*_rRacketOffset;
	return vRaquet0;
}

// ****************************************************************************************
//	GetRaquetPos1
// ****************************************************************************************
hgeVector Player::GetRaquetPos1() const
{
	hgeVector vRotationCenter =GetRotationCenter();
	hgeVector vRaquet1=vRotationCenter;
	vRaquet1 +=_vRacketDir*(_rRacketOffset+_rRacketLen);
	return vRaquet1;
}
