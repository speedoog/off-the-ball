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

#ifndef __COMMANDCPU_H__
#define __COMMANDCPU_H__
#pragma once

#include "../Base/Base.h"
#include "CommandAbc.h"

class InputCore;

class CommandCpu : public CommandAbc
{
public:
					CommandCpu();
					~CommandCpu();

protected:
	virtual void	OnInit(const UInt32 nPlayerId);
	virtual void	OnUpdate(const float rDeltaTime);
	virtual	void	OnRender();

protected:
	UInt32	_nPlayerId;
	Float32	_rTimeCurrent;
	Float32	_rBallRecordTime;
};

#endif	//__COMMANDCPU_H__
