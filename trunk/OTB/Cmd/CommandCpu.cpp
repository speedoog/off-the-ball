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


#include "CommandCpu.h"
#include "../Game/Game.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
CommandCpu::CommandCpu()
: _nPlayerId	(666)
{
	_nCmdType =CMD_CPU;
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
CommandCpu::~CommandCpu()
{

}

// ****************************************************************************************
//	OnInit
// ****************************************************************************************
void CommandCpu::OnInit(const UInt32 nPlayerId)
{
	_nPlayerId =nPlayerId;
	_rTimeCurrent =0.0f;
}

// ****************************************************************************************
//	OnUpdate
// ****************************************************************************************
void CommandCpu::OnUpdate(const float rDeltaTime)
{
	static const Float32 rAnticipationTime =0.1f;

	// reset previous cmd
	hgeVector vInputRacket(0.0f, 0.0f);
	hgeVector vInputMove(0.0f, 0.0f);

	_rTimeCurrent+= rDeltaTime;

	// fetch context
	UInt32 nOtherPlayerId =1-_nPlayerId;
	Ball& ball =_pGame->GetBall();
	hgeVector vBallPos =ball.GetPos();
	hgeVector vBallFuturePos =vBallPos+ball.GetVelocity()*rAnticipationTime;

	Player&	playerCPU =_pGame->GetPlayer(_nPlayerId);
	hgeVector vPlayerPos		=playerCPU.GetPosition();
	hgeVector vPlayerRotation	=playerCPU.GetRotationCenter();
	const Float32 rFront=playerCPU.GetFront();
	const Float32 rBack =playerCPU.GetBack();

	Rules&	rules =_pGame->GetRules();

	UInt32	nBallSide  =rules.GetBallSide();
	Bool	bRacketHit =rules.GetRacketHit();

	if (rules.IsWaitingToServe(_nPlayerId))								// Waiting for my serve
	{
		vInputMove	 =hgeVector(TRand(0.75f*rBack, rBack), 1.0f);
		vInputRacket =hgeVector(TRand(0.75f*rBack, rBack), TRand(0.75f, 1.0f));
		_rTimeCurrent =0.0f;
		//_rShootRand =(Float32(rand())/Float32(RAND_MAX))*0.2f+0.70f;
		//_rShootRandMin =0.68f;
		//_rShootRandMax =0.83f;
		_rShootRand =TRand(0.7f, 0.8f);
	}
	else
	{
		if (rules.IsServing())											// ----------------- in Service ---------------
		{
			_rBallRecordTime =0.0f;
			if (nBallSide==_nPlayerId)									// Service : my side
			{
				if (_rTimeCurrent>_rShootRand)
				{
					vInputRacket =hgeVector(rFront, 1.0f);				// shoot !
				}
				else
				{
					vInputRacket =hgeVector(rBack, 1.0f);
				}
			}
			else														// Service : other side
			{
				if (bRacketHit==false)
				{
					vInputMove.SetPolar(1.0f, _rTimeCurrent*4.0f);
					//vInputRacket.SetPolar(1.0f, _rTimeCurrent*10.0f);
					vInputRacket =hgeVector(rBack, -1.0f);
				}
			}
		}
		else															// ----------------- in game ---------------
		{
			if (nBallSide==_nPlayerId)									// my side
			{
				/*
				if (vBallFuturePos.y<vPlayerRotation.y)					// low ball
				{
					vInputRacket =hgeVector(0.0f, -1.0f);
					hgeVector vDistRacketBall =vBallPos-vPlayerRotation;
					if (vDistRacketBall.Length()<0.5f)
					{
						vInputRacket =hgeVector(rBack, 0.0f);			// shoot ! raise ball
					}
				}
				else
				{
					vInputRacket =hgeVector(rBack, 1.0f);
					hgeVector vDistRacketBall =vBallPos-vPlayerRotation;
					if (vDistRacketBall.Length()<1.0f)
					{
						vInputRacket =hgeVector(rFront, 1.0f);			// shoot !
					}
				}

				Float32 rDiffX =vBallFuturePos.x-(vPlayerPos.x+rBack*0.5f);
				vInputMove =hgeVector(rDiffX, 0.0f);
				*/
	
				BallRecord* pBallRecord =_pGame->GetBallRecorder().GetBestMatch();
				if (pBallRecord)
				{
					playerCPU.getPowerBar().SetValue(Float32(pBallRecord->_nSucced)/Float32(pBallRecord->_nTry) );

					UInt32 nBest;
					UInt32 nStart =BallRecord::GetFrameFromTime(_rBallRecordTime);
					Float32 rDiff;
					nBest =pBallRecord->GetBestMatch(hgeVector(rBack*vBallPos.x, vBallPos.y), nStart, 10, rDiff);

					if (rDiff>0.4f)
					{
						_rBallRecordTime +=rDeltaTime;
						nBest =BallRecord::GetFrameFromTime(_rBallRecordTime);
					}
					else
					{
						_rBallRecordTime =BallRecord::GetTimeFromFrame(nBest);
					}

					BallRecordFrame& frame =pBallRecord->GetFrame(nBest);

					Float32 rBallDiffX =(rBack*frame._vBallPos.x)-vBallPos.x;
					vInputMove.x =rBack*(frame._rPlayerPositionX)-vPlayerPos.x-rBallDiffX;
//					vInputMove.x*=1.5f;	// litle boost

					vInputRacket.x =rBack*frame._vInputRaquet.x;
					vInputRacket.y =frame._vInputRaquet.y;
				}

			}
			else
			{
				playerCPU.getPowerBar().SetValue( 0.0f );

				vInputRacket =hgeVector(rBack, -1.0f);
				vInputMove.SetPolar(1.0f, _rTimeCurrent*3.0f);
				_rBallRecordTime =0.0f;
			}
		}
	}

	_pPlayer->SetInputMove(vInputMove);
	_pPlayer->SetInputRacket(vInputRacket);
}

// ****************************************************************************************
//	OnRender
// ****************************************************************************************
void CommandCpu::OnRender()
{
	if (_pGame->GetBallRecorder()._bDbgBest)
	{
		if (_rBallRecordTime!=0.0f)
		{
			BallRecord* pBallRecord =_pGame->GetBallRecorder().GetBestMatch();
			if (pBallRecord)
			{
				BallRecordFrame& frame =pBallRecord->GetFrame(_rBallRecordTime);

				const Float32 rRevert =(_pGame->GetBall().GetSide()==0)?-1.0f:1.0f;
				hge->Gfx_RenderCircle(frame._vBallPos.x*rRevert, frame._vBallPos.y, 0.05f, 0xFFFFFF00);
			}
		}
	}
}