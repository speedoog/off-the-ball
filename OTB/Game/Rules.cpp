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

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Rules::Rules()
{
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
Rules::~Rules()
{
}

// ****************************************************************************************
//	Init
// ****************************************************************************************
void Rules::Init(Game* pGame)
{
	_pGame =pGame;
	_nDbgExchanges		=0;
	_nDbgFails			=0;
	_bShowScores		=true;
	_bShowRulesMsg		=true;
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void Rules::Update(const Float32 rDeltaTime)
{
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void Rules::Render()
{
	Level&		level		=_pGame->GetLevel();
	Resources&	resources	=_pGame->GetResources();

	Float32 rPosY =_pGame->GetLevel().GetSize().y;

	// render score (move it in another class ?)
	if (_bShowScores)
	{
		hgeVector vLvlSize =level.GetSize();
		hgeFont* pFontScore =resources._pFontScore;
		pFontScore->printf(-vLvlSize.x*0.98f, vLvlSize.y, HGETEXT_LEFT,  "%d", _pGame->GetPlayer(0).ScoreGet());
		pFontScore->printf( vLvlSize.x*0.98f, vLvlSize.y, HGETEXT_RIGHT, "%d", _pGame->GetPlayer(1).ScoreGet());
	}

	hgeFont* pFontMessages =resources._pFontMessages;
	pFontMessages->SetScale(-0.005f);

	if (_bShowRulesMsg)
	{
		if (_bServing)
		{
			if (_bWaitServe)
			{
				pFontMessages->printf(0.0f, rPosY/2.0f,	HGETEXT_CENTER,	"Player %d Waiting for serve", _nServicePlayer+1);
			}
			else
			{
				if (_bSecondServe)
				{
					pFontMessages->printf(0.0f, rPosY/2.0f,	HGETEXT_CENTER,	"Player %d Second Serve", _nServicePlayer+1);
				}
				else
				{
					pFontMessages->printf(0.0f, rPosY/2.0f,	HGETEXT_CENTER,	"Player %d Serve", _nServicePlayer+1);
				}
			}
		}
	}

	if (_pGame->GetTraining())
	{
		resources._pFontMessages->printf(0.0f, rPosY*0.95f, HGETEXT_CENTER, "Training   %02.2f %%", 100.0f*(1.0f-Float32(_nDbgFails)/Float32(_nDbgExchanges+1)) );
	}
}

// ..........................................................................................
// .										       											.
// .										ACTIONS											.
// .										       											.
// ..........................................................................................

// ****************************************************************************************
//	ActionStartGame
// ****************************************************************************************
void Rules::ActionStartGame(int nPlayerStart)
{
	_nServicePlayer =nPlayerStart;
	_pGame->GetPlayer(0).ScoreReset();
	_pGame->GetPlayer(1).ScoreReset();
	ActionServiceStart(nPlayerStart);
}

// ****************************************************************************************
//	ActionServiceStart
// ****************************************************************************************
void Rules::ActionServiceStart(int nPlayerServe)
{
	_bServing		=true;
	_bRacketHit 	=false;
	_nBallSide		=nPlayerServe;
	_nGroundTouch	=0;

	_pGame->GetLevel().Reset();
	_pGame->GetPlayer(0).ResetPosition();
	_pGame->GetPlayer(1).ResetPosition();
	Player* pPlayerServe =&_pGame->GetPlayer(nPlayerServe);
	Ball*	pBall		 =&_pGame->GetBall();
	pBall->Reset(pPlayerServe);
	pBall->SetPaused(true);
	_bWaitServe =true;
}

// ****************************************************************************************
//	ActionFail
// ****************************************************************************************
void Rules::ActionFail()
{
	++_nDbgFails;
	_pGame->GetBallRecorder().StopRecord(false);

	// update score
	int nOtherPlayer =1-_nBallSide;
	_pGame->GetPlayer(nOtherPlayer).ScoreInc();		// other player gain a point

	_nServicePlayer =1-_nServicePlayer;				// change server
	_bSecondServe	=false;
	ActionServiceStart(_nServicePlayer);
}

// ****************************************************************************************
//	ActionServiceFailed
// ****************************************************************************************
void Rules::ActionServiceFailed()
{
	if (_bSecondServe)
	{
		_bSecondServe =false;
		ActionFail();
	}
	else
	{
		// Second serve allowed, same player
		_bSecondServe =true;
		ActionServiceStart(_nServicePlayer);
	}
}

// ..........................................................................................
// .										       											.
// .										EVENTS											.
// .										       											.
// ..........................................................................................

// ****************************************************************************************
//	EventBallChangeSide
// ****************************************************************************************
void Rules::EventBallChangeSide(int nSide)
{
	_bServing		=false;
	_bRacketHit 	=false;
	_nBallSide		=nSide;
	_nGroundTouch	=0;

	++_nDbgExchanges;
	_pGame->GetBallRecorder().StopRecord(true);
	_pGame->GetBallRecorder().StartRecord();
}

// ****************************************************************************************
//	EventBallHitGround
// ****************************************************************************************
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
			if (_nGroundTouch>=2)			// 2 touch is a fail !
			{
				ActionFail();
			}
		}
	}
}

// ****************************************************************************************
//	EventBallHitWall
// ****************************************************************************************
void Rules::EventBallHitWall()
{

}

// ****************************************************************************************
//	EventBallHitRacket
// ****************************************************************************************
void Rules::EventBallHitRacket()
{
	if (_bServing && _bRacketHit)			// double hit during service
	{
		ActionServiceFailed();
	}
	else
	{
		_bRacketHit =true;
	}
}

// ****************************************************************************************
//	EventBallHitNet
// ****************************************************************************************
void Rules::EventBallHitNet()
{
	if (_bServing)
	{
		ActionServiceFailed();
	}
	else
	{
		ActionFail();
	}
}

// ****************************************************************************************
//	EventServeStart
// ****************************************************************************************
void Rules::EventServeStart()
{
	_pGame->GetBall().SetPaused(false);
	_bWaitServe =false;
}
