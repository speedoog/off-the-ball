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
#include "../Game.h"

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
	// reset previous cmd
	hgeVector vInputRacket(0.0f, 0.0f);
	hgeVector vInputMove(0.0f, 0.0f);

	_rTimeCurrent+= rDeltaTime;

	// fetch context
	UInt32 nOtherPlayerId =1-_nPlayerId;
	Ball& ball =_pGame->GetBall();
	hgeVector vBallPos =ball.GetPos();

	Player&	playerCPU =_pGame->GetPlayer(_nPlayerId);
	hgeVector vPlayerPos		=playerCPU.GetPosition();
	hgeVector vPlayerRotation	=playerCPU.GetRotationCenter();

	Rules&	rules =_pGame->GetRules();

	UInt32	nBallSide  =rules.GetBallSide();
	Bool	bRacketHit =rules.GetRacketHit();

	if (rules.IsWaitingToServe(_nPlayerId))
	{
		// wait for my serve
		vInputMove	 =hgeVector(1.0f, 1.0f);
		vInputRacket =hgeVector(1.0f, 1.0f);
		_rTimeCurrent =0.0f;
	}
	else
	{
		if (rules.IsServing())
		{
			if (nBallSide==_nPlayerId)
			{
				if (_rTimeCurrent>0.8f)
				{
					vInputRacket =hgeVector(-1.0f, 1.0f);
				}
				else
				{
					vInputRacket =hgeVector(1.0f, 1.0f);
				}
			}
			else
			{
				if (bRacketHit==false)
				{
					vInputMove.SetPolar(1.0f, _rTimeCurrent*4.0f);
					vInputRacket.SetPolar(1.0f, _rTimeCurrent*10.0f);
				}
			}
		}
		else
		{
			// ----------------- in game ---------------
			if (nBallSide==_nPlayerId)
			{
				Float32 rDiffX =vBallPos.x-vPlayerPos.x;

				vInputRacket =hgeVector(1.0f, 1.0f);

				hgeVector vDistRacketBall =vBallPos-vPlayerRotation;
				if (vDistRacketBall.Length()<0.9f)
				{
					vInputRacket =hgeVector(-1.0f, 1.0f);			// shoot
				}

				vInputMove =hgeVector(rDiffX, 0.0f);
			}
			else
			{
				vInputRacket =hgeVector(1.0f, 1.0f);
				vInputMove.SetPolar(1.0f, _rTimeCurrent*10.0f);
			}
		}
	}

	_pPlayer->SetInputMove(vInputMove);
	_pPlayer->SetInputRacket(vInputRacket);
}
