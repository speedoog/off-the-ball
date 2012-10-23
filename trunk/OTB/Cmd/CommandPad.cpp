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


#include "CommandPad.h"
#include "../Game/Game.h"

// ********************************************
//	Ctor
// ********************************************
CommandPad::CommandPad()
: _pInput	(NULL)
{
	_nCmdType =CMD_PAD;
}

// ********************************************
//	Dtor
// ********************************************
CommandPad::~CommandPad()
{

}

// ********************************************
//	OnInit
// ********************************************
void CommandPad::OnInit(const UInt32 nPlayerId)
{
	_pInput =&_pGame->GetInputCommand();
}

// ********************************************
//	OnUpdate
// ********************************************
void CommandPad::OnUpdate(const float rDeltaTime)
{
	hgeVector vAxisLeft =_pInput->GetAxisLeft(_nCmdId);
	if (vAxisLeft.Length()>0.2f)						// deadzone
	{
		_pPlayer->SetInputMove(vAxisLeft);
	}
	else
	{
		_pPlayer->SetInputMove(hgeVector(0,0));
	}

	const hgeVector& vAxisRight =_pInput->GetAxisRight(_nCmdId);
	if (vAxisRight.Length()>0.3f)						// deadzone
	{
		_pPlayer->SetInputRacket(vAxisRight);
	}
// 	else
// 	{
// 		_pPlayer->SetInputRacket(hgeVector(0,0));
// 	}
}
