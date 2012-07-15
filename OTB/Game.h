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

#ifndef __GAME_H__
#define __GAME_H__
#pragma once

#include "base.h"
#include "Level.h"
#include "Player.h"
#include "Ball.h"
#include "Resources.h"
#include "PcPad.h"

class Game
{
public:
							Game();
							~Game();
			void			Init();
			void			Kill();

			void			NewBall();
			void			Update(const float rDeltaTime);
			void			Render();

	// Accessors
	inline	Resources&		GetResources()		{ return _Resources;  	}
	inline	PcPadManager&	GetPadManager()		{ return _PadManager; 	}
	inline	Player&			GetPlayer(int i)	{ return _Players[i]; 	}
	inline	Ball&			GetBall()			{ return _Ball;			}
	inline	Level&			GetLevel()			{ return _Level;		}

protected:
	Level			_Level;
	Player			_Players[2];
	Ball			_Ball;
	Resources		_Resources;
	PcPadManager	_PadManager;
};

#endif	//__GAME_H__
