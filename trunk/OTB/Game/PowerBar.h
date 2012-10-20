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

#ifndef __POWERBAR_H__
#define __POWERBAR_H__
#pragma once

#include "../Base/Base.h"

class Game;

class PowerBar
{
public:
								PowerBar();
								~PowerBar();

			void				Init(const UInt32 nPlayerId, Game* pGame);
			void				Reset();
			Bool				Update(const float rDeltaTime, const Bool bUse);
			void				ChangeSide();
			void				Render();

protected:
	UInt32		_nPlayerId;
	Float32		_rValue;
	Game*		_pGame;
};


#endif	//__POWERBAR_H__
