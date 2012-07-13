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

			void			Update(const float rDeltaTime);
			void			Render();
	inline	Resources&		GetResources()		{ return _Resources;  }
	inline	PcPadManager&	GetPadManager()		{ return _PadManager; }

protected:
	Level			_Level;
	Player			_Player1, _Player2;
	Ball			_Ball;
	Resources		_Resources;
	PcPadManager	_PadManager;
};

#endif	//__GAME_H__
