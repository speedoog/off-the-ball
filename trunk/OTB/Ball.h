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

#ifndef __BALL_H__
#define __BALL_H__
#pragma once

#include "Base/Base.h"
#include "BallHistory.h"

class Game;
class Player;

class Ball
{
public:
						Ball();
						~Ball();
			void		Init(Game* pGame);

			void		Reset(Player* pPlayer);
			void		Update(const float rDeltaTime);
			void		Render();

			void		Launch(const hgeVector& vDir);
			void		RacketHit(const hgeVector& vVelocity);

	// inline
	inline	hgeVector&	GetPos() 						{ return _vPos;			}
	inline	hgeVector&	GetVelocity() 					{ return _vVelocity;	}
	inline	int			GetSide()						{ return _nSide;		}
	inline	Float32		GetRadius() 					{ return _rRadius;		}
	inline	void		SetPaused(const bool bPaused)	{ _bPaused =bPaused;	}

protected:
	Game*		_pGame;
	hgeVector	_vPos, _vLastPos;
	hgeVector	_vVelocity;
	Float32		_rRadius;
	int			_nSide;				// 0 or 1
	bool		_bPaused;

	BallHistory	_History;
};

#endif	//__BALL_H__
