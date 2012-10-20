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

// ********************************************
//	Ctor
// ********************************************
CommandCpu::CommandCpu()
: _nPlayerId (666)
{
}

// ********************************************
//	Dtor
// ********************************************
CommandCpu::~CommandCpu()
{

}

// ********************************************
//	OnInit
// ********************************************
void CommandCpu::OnInit(const UInt32 nPlayerId)
{
	_nPlayerId =nPlayerId;
	_rTimeCurrent =0.0f;
}

// ********************************************
//	OnUpdate
// ********************************************
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
		vInputMove	 =hgeVector(rBack, 1.0f);
		vInputRacket =hgeVector(rBack, 1.0f);
		_rTimeCurrent =0.0f;
	}
	else
	{
		if (rules.IsServing())											// ----------------- in Service ---------------
		{
			if (nBallSide==_nPlayerId)									// Service : my side
			{
				if (_rTimeCurrent>0.8f)
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
					vInputRacket.SetPolar(1.0f, _rTimeCurrent*10.0f);
				}
			}
		}
		else															// ----------------- in game ---------------
		{
			if (nBallSide==_nPlayerId)									// my side
			{
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
			}
			else
			{
				vInputRacket =hgeVector(rBack, 1.0f);
				vInputMove.SetPolar(1.0f, _rTimeCurrent*10.0f);
			}
		}
	}

	_pPlayer->SetInputMove(vInputMove);
	_pPlayer->SetInputRacket(vInputRacket);
}
