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

			void				Init(Game* pGame, const hgeVector& vSize, const float rInitialNetY);
			void				Reset();
			void				Update(const float rDeltaTime);
			void				Render();

	// inlines
	inline	const hgeVector&	GetSize()	{ return _vSize; }
	inline	float				GetNetY()	{ return _rNetY; }

protected:
	Game*		_pGame;
	hgeVector	_vSize;
	float		_rNetY;
	float		_rInitialNetY;
	float		_rCounter;
};


#endif	//__LEVEL_H__
