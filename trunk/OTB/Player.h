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
#include "PowerBar.h"
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
	inline	hgeVector			GetRotationCenter() const						{ return hgeVector(_vPos.x, _rCharRacketY);}
	inline	void				SetVelocity(const hgeVector& vVel)				{ _vVelocity=vVel;					}
	inline	const hgeVector&	GetVelocity() const								{ return _vVelocity;				}
	inline	Float32				GetFront() const								{ return (_nPlayerId*-2.0f)+1.0f;	}
	inline	Float32				GetBack() const									{ return (_nPlayerId*2.0f)-1.0f;	}
	inline	int					GetPlayerId() const								{ return _nPlayerId;				}

	inline	void				ScoreInc()										{ ++_nScore;						}
	inline	void				ScoreReset()									{ _nScore =0;						}
	inline	int					ScoreGet()										{ return _nScore;					}

	inline	bool				UseTimeScale()									{ return _bUseTimeScale;			}

	// inputs
	inline	void				SetInputMove(const hgeVector& vInputMove)		{ _vInputMove	=vInputMove;	if (_vInputMove.LengthSq()>1.0f)	_vInputMove.Normalize();	}
	inline	void				SetInputRacket(const hgeVector& vInputRacket)	{ _vInputRacket =vInputRacket;	if (_vInputRacket.LengthSq()>1.0f)	_vInputRacket.Normalize(); 	}

protected:
	hgeVector	GetRaquetPos0() const;
	hgeVector	GetRaquetPos1() const;

protected:
	Game*		_pGame;

	PowerBar	_PowerBar;

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
	float		_rRacketOffset;
	float		_rPosMin, _rPosMax;
	int			_nPlayerId;

	// runtime values
	float		_rHitCooldown;
	int			_nScore;
	bool		_bUseTimeScale;

	// Input
	hgeVector	_vInputMove, _vInputRacket;
};

#endif	//__PLAYER_H__
