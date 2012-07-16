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


#include "Rules.h"
#include "Game.h"

// ********************************************
//	Ctor
// ********************************************
Rules::Rules()
{
}

// ********************************************
//	Dtor
// ********************************************
Rules::~Rules()
{
}

// ********************************************
//	Init
// ********************************************
void Rules::Init(Game* pGame)
{
	_pGame =pGame;
}

// ********************************************
//	Update
// ********************************************
void Rules::Update(const float rDeltaTime)
{
}

// ********************************************
//	Render
// ********************************************
void Rules::Render()
{
}

// ..........................................................................................................
// .												       													.
// .												ACTIONS													.
// .												       													.
// ..........................................................................................................

// ********************************************
//	ActionStartGame
// ********************************************
void Rules::ActionStartGame(int nPlayerStart)
{
	_pGame->GetPlayer(0).ScoreReset();
	_pGame->GetPlayer(1).ScoreReset();
	ActionServiceStart(nPlayerStart);
}

// ********************************************
//	ActionServiceStart
// ********************************************
void Rules::ActionServiceStart(int nPlayerServe)
{
	_bServing		=true;
	_bRacketHit 	=false;
	_nSide			=nPlayerServe;
	_nGroundTouch	=0;

	_pGame->GetLevel().Reset();
	_pGame->GetPlayer(0).Reset();
	_pGame->GetPlayer(1).Reset();
	_pGame->GetBall().Reset(nPlayerServe);
}

// ********************************************
//	ActionFail
// ********************************************
void Rules::ActionFail()
{
	int nOtherPlayer =1-_nSide;
	_pGame->GetPlayer(nOtherPlayer).ScoreInc();		// other player gain a point

	ActionServiceStart(nOtherPlayer);
}

// ********************************************
//	ActionServiceFailed
// ********************************************
void Rules::ActionServiceFailed()
{
	//... try second service ??
	ActionFail();
}

// ..........................................................................................................
// .												       													.
// .												EVENTS													.
// .												       													.
// ..........................................................................................................

// ********************************************
//	EventBallChangeSide
// ********************************************
void Rules::EventBallChangeSide(int nSide)
{
	_bServing		=false;
	_bRacketHit 	=false;
	_nSide			=nSide;
	_nGroundTouch	=0;
}

// ********************************************
//	EventBallHitGround
// ********************************************
void Rules::EventBallHitGround()
{
	if (_bServing)
	{
		ActionServiceFailed();
	}
	else
	{
		if (_bRacketHit)
		{
			ActionFail();
		}
		else
		{
			++_nGroundTouch;
			if (_nGroundTouch>=2)			// 4 touch is a fail !
			{
				ActionServiceFailed();
			}
		}
	}
}

// ********************************************
//	EventBallHitWall
// ********************************************
void Rules::EventBallHitWall()
{

}

// ********************************************
//	EventBallHitRacket
// ********************************************
void Rules::EventBallHitRacket()
{
	_bRacketHit =true;
}

// ********************************************
//	EventBallHitNet
// ********************************************
void Rules::EventBallHitNet()
{
	if (_bServing)
	{
		ActionServiceFailed();
	}
}

