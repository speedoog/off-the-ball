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

#include "base.h"

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
			void		ActionStartGame(int nPlayerStart);
			void		ActionServiceStart(int nPlayerServe);
			void		ActionFail();
			void		ActionServiceFailed();

	// Events
			void		EventBallChangeSide(int nSide);
			void		EventBallHitGround();
			void		EventBallHitWall();
			void		EventBallHitRacket();
			void		EventBallHitNet();
			void		EventServeStart();

	inline	bool		IsWaitingToServe(int nPlayerId)	{ return _nServicePlayer==nPlayerId && _bWaitServe; }

protected:
	Game*	_pGame;

	bool	_bServing;
	bool	_bSecondServe;
	bool	_bWaitServe;
	bool	_bRacketHit;
	int		_nSide;
	int		_nGroundTouch;
	int		_nServicePlayer;
};


#endif	//__RULES_H__
