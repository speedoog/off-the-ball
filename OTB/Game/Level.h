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

#ifndef __LEVEL_H__
#define __LEVEL_H__
#pragma once

#include "../Base/Base.h"

class Game;

class Level
{
public:
								Level();
								~Level();

			void				Init(Game* pGame, const hgeVector& vSize, const Float32 rInitialNetY);
			void				Kill();
			void				Reset();
			void				Update(const Float32 rDeltaTime);
			void				Render();

	// inlines
	inline	const hgeVector&	GetSize()		{ return _vSize; }
	inline	Float32				GetNetY()		{ return _rNetY; }
	inline	Float32				GetFutureNetY()	{ if (_rCounter>15.0f) return _rNetY+0.2f; else return _rNetY; }

protected:
	Game*		_pGame;
	hgeVector	_vSize;
	Float32		_rNetY;
	Float32		_rInitialNetY;
	Float32		_rCounter;
};


#endif	//__LEVEL_H__
