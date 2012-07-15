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

#ifndef __PLAYER_H__
#define __PLAYER_H__
#pragma once

#include "base.h"
class Game;

class Player
{
public:
					Player();
					~Player();
			void	Init(Game* pGame, const int nPlayerId);

	inline	void	SetPosition(const hgeVector& vPos) { _vPos =vPos; }

			void	Update(const float rDeltaTime);
			void	Render();
protected:
	Game*		_pGame;
	hgeVector	_vPos;

	float 		_rCharSpeed;
	hgeVector	_vCharSize;
	float 		_rCharRacketY;
	float 		_rRacketLen;
	hgeVector	_vRacketDir;

	float		_rCrossLast;
	float		_rHitCooldown;

	float		_rPosMin, _rPosMax;
	int			_nPlayerId;
};

#endif	//__PLAYER_H__
