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

#include "Base/Base.h"
class Game;

class Player
{
public:
								Player();
								~Player();
			void				Init(Game* pGame, const int nPlayerId);
			void				ResetPosition();

			void				Update(const float rDeltaTime);
			void				Render();

	inline	void				SetPosition(const hgeVector& vPos)				{ _vPos =vPos;						}
	inline	const hgeVector&	GetPosition() const								{ return _vPos;						}
	inline	void				SetVelocity(const hgeVector& vVel)				{ _vVelocity=vVel;					}
	inline	const hgeVector&	GetVelocity() const								{ return _vVelocity;				}
	inline	float				GetAt() const									{ return _nPlayerId==0?1.0f:-1.0f;	}
	inline	int					GetPlayerId() const								{ return _nPlayerId;				}

	inline	void				ScoreInc()										{ ++_nScore;						}
	inline	void				ScoreReset()									{ _nScore =0;						}
	inline	int					ScoreGet()										{ return _nScore;					}

	// inputs
	inline	void				SetInputMove(const hgeVector& vInputMove)		{ _vInputMove	=vInputMove;		}
	inline	void				SetInputRacket(const hgeVector& vInputRacket)	{ _vInputRacket =vInputRacket;		}

protected:
	Game*		_pGame;

	// Physic
	hgeVector	_vPos, _vInitialPos;
	hgeVector	_vVelocity;
	float		_rRacketRotationSpeed;

	// Settings
	float 		_rCharSpeedMax;
	hgeVector	_vCharSize;
	float 		_rCharRacketY;
	float 		_rRacketLen;
	hgeVector	_vRacketDir;
	float		_rPosMin, _rPosMax;
	int			_nPlayerId;

	// runtime values
	float		_rCrossLast;
	float		_rHitCooldown;
	int			_nScore;

	// Input
	hgeVector	_vInputMove, _vInputRacket;
};

#endif	//__PLAYER_H__
