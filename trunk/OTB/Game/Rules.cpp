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

static const Float32 rFailDuration			=2.0f;
static const Float32 rWinAnimationDuration	=2.0f;

static const UInt32	 nMaxPoints				=2;

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
	_rFailTimer			=-1.0f;
	_nFailPlayerScore	=-1;
	_bSecondServe		=false;
	_nPlayerWin			=-1;
	_rWinAnimation		=0.0f;
}

// ****************************************************************************************
//	Update
// ****************************************************************************************
void Rules::Update(const Float32 rDeltaTime)
{
	if (GetFailMode())
	{
		_rFailTimer -=rDeltaTime;
		if (_rFailTimer<0)
		{
			if (_nFailPlayerScore!=-1)
			{
				Player& playerPoint =_pGame->GetPlayer(_nFailPlayerScore);
				playerPoint.ScoreInc();		// player gain a point

				if (_pGame->GetDemoMode()==false && _pGame->GetTraining()==false && playerPoint.ScoreGet()>=nMaxPoints)
				{
					_nPlayerWin =_nFailPlayerScore;
					_rWinAnimation =0.0f;
				}
				else
				{
					ActionServiceStart(_nServicePlayer);
				}
			}
			else
			{
				// second serve ...
				ActionServiceStart(_nServicePlayer);
			}
		}
	}

	if (_nPlayerWin!=-1)
	{
		_rWinAnimation+=rDeltaTime;
		if (_rWinAnimation>rWinAnimationDuration)
		{
			_pGame->SetPaused(true);
		}
	}
}

// ****************************************************************************************
//	Render
// ****************************************************************************************
void Rules::Render()
{
	Level&		level		=_pGame->GetLevel();
	Resources&	resources	=_pGame->GetResources();

	Float32 rPosY =_pGame->GetLevel().GetSize().y;

	hgeVector vLvlSize =level.GetSize();
	hgeVector vPosScoreGame[2]	={ hgeVector(-vLvlSize.x*0.98f, vLvlSize.y),		hgeVector(vLvlSize.x*0.98f, vLvlSize.y)		 };
	hgeVector vPosScoreEnd[2]	={ hgeVector(-vLvlSize.x*0.20f, vLvlSize.y*0.5f),	hgeVector(vLvlSize.x*0.20f, vLvlSize.y*0.5f) };

	hgeFont* pFontMessages =resources._pFontMessages;
	pFontMessages->SetScale(-0.005f);
	pFontMessages->SetColor(0xFFFFFFFF);

	if (_bShowRulesMsg)
	{
		if (_bScoreMsg && _nFailPlayerScore!=-1)
		{
			hgeFont* pFontScore =resources._pFontScore;
			Int32 nOtherPlayer =1-_nBallSide;

			hgeVector vTargetPos =vPosScoreGame[nOtherPlayer];

			Float32 rMsgRatio =TChangeRange(rFailDuration, 0.0f, 0.0f, 1.0f, _rFailTimer);
			hgeVector vPosMsg =TBlend(_vFailStartPos, vTargetPos, TSmoothStep(TSmoothStep(rMsgRatio)));

			pFontScore->SetScale(-0.01f*TChangeRange(0.0f, 1.0f, 1.0f, 0.5f, rMsgRatio));

			hgeColorRGB colMsg(Resources::ColorPlayerBody[nOtherPlayer]);
			hgeColorRGB colScale(1.0f, 1.0f, 1.0f, 1.0f-rMsgRatio);
			hgeColorRGB colFinal;
			colFinal =colMsg*colScale;

			pFontScore->SetColor(colFinal.GetHWColor());
			pFontScore->printf(vPosMsg.x, vPosMsg.y,	HGETEXT_CENTER,	"+1", nOtherPlayer+1);
		}

		const Float32 rPosGameMsgX =vPosScoreGame[_nServicePlayer].x*0.7f;
		const Float32 rPosGameMsgY =rPosY*0.7f;

		if (_bServing && !GetFailMode())
		{
			pFontMessages->printf(rPosGameMsgX, rPosGameMsgY,	HGETEXT_CENTER,	_bSecondServe?"Second Serve":"Serve");
		}

		if (_nPlayerWin==0)
		{
			pFontMessages->SetColor(Resources::ColorPlayerBody[0]);
			pFontMessages->printf(0.0f, rPosGameMsgY, HGETEXT_CENTER, "Blue Player Win");
		}
		else if (_nPlayerWin==1)
		{
			pFontMessages->SetColor(Resources::ColorPlayerBody[1]);
			pFontMessages->printf(0.0f, rPosGameMsgY, HGETEXT_CENTER, "Red Player Win");
		}
	}

	// render score (move it in another class ?)
	if (_bShowScores)
	{
		hgeFont* pFontScore =resources._pFontScore;
		pFontScore->SetScale(-0.01f * 1.0f);
		for(Int32 iPlayer=0; iPlayer<2; ++iPlayer)
		{
			hgeVector vPosScore;
			if (_nPlayerWin==-1)
			{
				vPosScore =vPosScoreGame[iPlayer];
			}
			else
			{
				Float32 rMsgRatio =TChangeRangeClamped(0.0f, rWinAnimationDuration, 0.0f, 1.0f, _rWinAnimation);
				vPosScore =TBlend(vPosScoreGame[iPlayer], vPosScoreEnd[iPlayer], TSmoothStep(TSmoothStep(rMsgRatio)));
			}

			pFontScore->SetColor(0xFFFFFFFF);
			pFontScore->printf( vPosScore.x, vPosScore.y, iPlayer/*align*/,  "%d", _pGame->GetPlayer(iPlayer).ScoreGet());
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
void Rules::ActionStartGame(Int32 nPlayerStart)
{
	_nServicePlayer =nPlayerStart;
	_pGame->GetPlayer(0).ScoreReset();
	_pGame->GetPlayer(1).ScoreReset();
	ActionServiceStart(nPlayerStart);
}

// ****************************************************************************************
//	ActionServiceStart
// ****************************************************************************************
void Rules::ActionServiceStart(Int32 nPlayerServe)
{
	_bScoreMsg		=false;
	_rFailTimer		=-1.0f;
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
	Int32 nOtherPlayer =1-_nBallSide;
	_nFailPlayerScore =nOtherPlayer;
	_bScoreMsg =true;

	_nServicePlayer =1-_nServicePlayer;				// change server
	_bSecondServe	=false;

	// start Fail
	FailStart();
}

// ****************************************************************************************
//	FailStart
// ****************************************************************************************
void Rules::FailStart()
{
	_rFailTimer		=rFailDuration;
	_vFailStartPos	=_pGame->GetBall().GetPos();
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
		FailStart();
		_nFailPlayerScore =-1;
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
void Rules::EventBallChangeSide(Int32 nSide)
{
	if (GetFailMode())
	{
		return;
	}

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
	if (GetFailMode())
	{
		return;
	}

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
	if (GetFailMode())
	{
		return;
	}

}

// ****************************************************************************************
//	EventBallHitRacket
// ****************************************************************************************
void Rules::EventBallHitRacket()
{
	if (GetFailMode())
	{
		return;
	}

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
	if (GetFailMode())
	{
		return;
	}

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
