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

#include "player.h"

#include "Game.h"
#include "../Otb.h"

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
: _RacketHistory(1.0f/100.0f, 0.5f, 1000)
{
	_rCharSpeedMax 	=7.0f;
	_vCharSize 		=hgeVector(0.35f, 1.8f)*0.75f;
	_rCharRacketY	=_vCharSize.y*0.75f;
	_rRacketLen		=_vCharSize.y*0.6f;
	_nScore			=0;
	_vInitialPos	=hgeVector(0,0);
	_rRacketOffset	=0.0f;
	_hcZion			=0;
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

	// Init audio
	if (_hcZion==0)
	{
		Otb* pOTB =Otb::GetInstance();
		Audio& audio =pOTB->GetAudio();
		_hcZion =audio.SamplePlay(pOTB->GetResources()._hsZion, 0.0f, 0.0f, 1.0f, true);
		audio.ChannelSetAttrib(_hcZion, BASS_ATTRIB_PAN, -GetFront());
	}
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

	_RacketHistory.Reset();
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

		// play flap sound
		if (_pGame->GetTimeScale()<=2.0f)
		{
			Otb* pOTB =Otb::GetInstance();
			Audio& audio =pOTB->GetAudio();

			Float32 rVolume =TChangeRangeClamped(0.0f, M_PI, 0.0f, 0.6f, TAbs(rAngleDiff));
			audio.ChannelSetAttrib(_hcZion, BASS_ATTRIB_VOL, rVolume);

			Float32 rFreq =TChangeRangeClamped(0.0f, M_PI*0.5f, 0.0f, 22100.0f, TAbs(rAngleDiff));
			audio.ChannelSetAttrib(_hcZion, BASS_ATTRIB_FREQ, rFreq);
		}

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
		if (_vInputMove.y>0.3f)					// deadzone
		{
			pRules->EventServeStart();
			ball.Launch(_vInputMove);
		}
	}

	hgeVector 	vRaquet0 =GetRaquetPos0();
	hgeVector 	vRaquet1 =GetRaquetPos1();

	if (pRules->GetFailMode()==false)
	{
		// check ball collide
		hgeVector vBallPos		=ball.GetPos();

		if (_rHitCooldown<=0.0f)
		{
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

	_RacketHistory.Update(rDeltaTime, RacketData(vRaquet0, vRaquet1));
}

//const UInt32 nColorPlayerRaquet	=0xFF60E060;
const UInt32 nColorPlayerRaquet	=0xFFFFFFFF;
const UInt32 nColorPlayerEye	=0xFFFFFFFF;

// ****************************************************************************************
//	Render
// ****************************************************************************************
void Player::Render()
{
	hgeColorRGB	colBody;	colBody.SetHWColor(Resources::ColorPlayerBody[_nPlayerId]);
	hgeColorRGB colRaquet;	colRaquet.SetHWColor(nColorPlayerRaquet);
	hgeColorRGB colEye;		colEye.SetHWColor(nColorPlayerEye);

	// racket trail
	_RacketHistory.Draw(_pGame->GetResources()._pSpriteRacketTrail, colBody);

	// Player
	Otb::RenderQuad(hgeVector(_vPos.x-_vCharSize.x, _vPos.y), hgeVector(_vPos.x+_vCharSize.x, _vPos.y+_vCharSize.y), colBody.GetHWColor());
	hge->Gfx_RenderBox(	_vPos.x-_vCharSize.x, _vPos.y,
						_vPos.x+_vCharSize.x, _vPos.y+_vCharSize.y, nColorPlayerEye);

	// Eye test
	Float32 rEyeRadius =_vCharSize.x*0.25f;
	hgeVector vEyeCenter(_vPos.x+_vCharSize.x*0.75f*GetFront(), _vPos.y+_vCharSize.y*0.85f);
	Otb::RenderDisk(vEyeCenter, rEyeRadius, 0.0f, colEye);

	// Racket
	hgeVector vRaquet0=GetRaquetPos0();
	hgeVector vRaquet1=GetRaquetPos1();
	Otb::RenderLine(vRaquet0, vRaquet1, -1.0f, colRaquet, 0.03f);

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
