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
	Level&		level		=_pGame->GetLevel();
	Resources&	resources	=_pGame->GetResources();

	// render score (move it in another class ?)
	hgeVector vLvlSize =level.GetSize();
	hgeFont* pFontScore =resources._pFontScore;
	pFontScore->printf(vLvlSize.x*0.98f, vLvlSize.y, HGETEXT_RIGHT, "%d", _pGame->GetPlayer(0).ScoreGet());
	pFontScore->printf(-vLvlSize.x*0.98f, vLvlSize.y, HGETEXT_LEFT, "%d", _pGame->GetPlayer(1).ScoreGet());

	hgeFont* pFontMessages =resources._pFontMessages;
	float rPosY =_pGame->GetLevel().GetSize().y;
	if (_bServing)
	{
		pFontMessages->SetScale(-0.005f);
		pFontMessages->printf(0.0f, rPosY/2.0f,	HGETEXT_CENTER,	"Player %d Serve", _nSide+1);
	}
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
	_bSecondServe	=false;
	_nServicePlayer =nPlayerStart;
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
	_pGame->GetPlayer(0).ResetPosition();
	_pGame->GetPlayer(1).ResetPosition();
	Player* pPlayerServe =&_pGame->GetPlayer(nPlayerServe);
	_pGame->GetBall().Reset(pPlayerServe);
}

// ********************************************
//	ActionFail
// ********************************************
void Rules::ActionFail()
{
	// update score
	int nOtherPlayer =1-_nSide;
	_pGame->GetPlayer(nOtherPlayer).ScoreInc();		// other player gain a point

	_nServicePlayer =1-_nServicePlayer;				// change server
	ActionServiceStart(_nServicePlayer);
}

// ********************************************
//	ActionServiceFailed
// ********************************************
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
			if (_nGroundTouch>=2)			// 2 touch is a fail !
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
	if (_bServing && _bRacketHit)			// double hit during service
	{
		ActionServiceFailed();
	}
	else
	{
		_bRacketHit =true;
	}
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
	else
	{
		ActionFail();
	}
}
