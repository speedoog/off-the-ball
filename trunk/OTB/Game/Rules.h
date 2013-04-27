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

#ifndef __RULES_H__
#define __RULES_H__
#pragma once

#include "../Base/Base.h"

class Game;

class Rules
{
public:
						Rules();
						~Rules();

			void		Init(Game* pGame);
			void		Update(const float rDeltaTime);
			void		Render();

	// Actions
			void		ActionStartGame(Int32 nPlayerStart);
			void		ActionServiceStart(Int32 nPlayerServe);
			void		ActionFail();
			void		ActionServiceFailed();

			void		FailStart();

	// Events
			void		EventBallChangeSide(Int32 nSide);
			void		EventBallHitGround();
			void		EventBallHitWall();
			void		EventBallHitRacket();
			void		EventBallHitNet();
			void		EventServeStart();

	inline	Bool		IsWaitingToServe(Int32 nPlayerId)			{ return _nServicePlayer==nPlayerId && _bWaitServe; }
	inline	Bool		IsWaitingToServe()							{ return _bWaitServe;				}
	inline	Bool		IsServing()									{ return _bServing;					}
	inline	Int32		GetBallSide() const							{ return _nBallSide;				}
	inline	Bool		GetRacketHit() const						{ return _bRacketHit;				}
	inline	Bool		GetFailMode() const							{ return _rFailTimer>0.0f;			}

	inline	void		SetShowScores(const Bool bShowScores)		{ _bShowScores =bShowScores;		}
	inline	void		SetShowRulesMsg(const Bool bShowRulesMsg)	{ _bShowRulesMsg =bShowRulesMsg;	}

protected:
	Game*	_pGame;

	Bool	_bServing;
	Bool	_bSecondServe;
	Bool	_bWaitServe;
	Bool	_bRacketHit;
	Int32	_nBallSide;
	Int32	_nGroundTouch;
	Int32	_nServicePlayer;

	// Fail mode
	Bool		_bScoreMsg;
	Float32		_rFailTimer;
	hgeVector	_vFailStartPos;
	Int32		_nFailPlayerScore;

	// Display Settings
	Bool		_bShowScores, _bShowRulesMsg;

	// Training stats
	Int32		_nDbgExchanges, _nDbgFails;
};


#endif	//__RULES_H__
