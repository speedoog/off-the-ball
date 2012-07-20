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

#ifndef __COMMANDABC_H__
#define __COMMANDABC_H__
#pragma once

#include "base.h"
#include "SmartEnum.h"

class Game;
class Player;

class CommandAbc
{
public:
	SMARTENUM_DECLARE(CmdType
			, CMD_PAD
			, CMD_KBDMOUSE
			, CMD_TRACKPAD
			);

					CommandAbc();
					~CommandAbc();

			void	Init(Game* pGame, Player* pPlayer);
			void	Update(const float rDeltaTime);

protected:
	// virtuals
	virtual	void	OnInit()							{ };
	virtual	void	OnUpdate(const float rDeltaTime)	{ };

protected:
	Game*		_pGame;
	Player* 	_pPlayer;
};


#endif	//__COMMANDABC_H__
