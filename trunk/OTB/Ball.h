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

#include "base.h"

class Game;

class Ball
{
public:
						Ball();
						~Ball();
			void		Init(Game* pGame);

			void		Reset(int nPlayer);
			void		Update(const float rDeltaTime);
			void		Render();
			void		Hit(const hgeVector& vVelocity);

	// inline
	inline	hgeVector&	GetPos() 	{ return _vPos; }
	inline	int			GetSide()	{ return _nSide; }

protected:
	Game*		_pGame;
	hgeVector	_vPos, _vLastPos;
	hgeVector	_vVelocity;
	int			_nSide;				// 0 or 1
};

#endif	//__BALL_H__
