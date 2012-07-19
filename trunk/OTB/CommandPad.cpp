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
#include "Game.h"

// ********************************************
//	Ctor
// ********************************************
CommandPad::CommandPad()
: _pPadManager	(NULL)
, _nPadIdx		(0)
{
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
void CommandPad::OnInit()
{
	_pPadManager =&_pGame->GetPadManager();
	_nPadIdx =_pPlayer->GetPlayerId();
}

// ********************************************
//	OnUpdate
// ********************************************
void CommandPad::OnUpdate(const float rDeltaTime)
{
	hgeVector vAxisLeft =_pPadManager->GetAxisLeft(_nPadIdx);
	if (fabsf(vAxisLeft.x)>0.15f)						// deadzone
	{
		_pPlayer->SetInputMove(vAxisLeft);
	}
	else
	{
		_pPlayer->SetInputMove(hgeVector(0,0));
	}

	const hgeVector& vAxisRight =_pPadManager->GetAxisRight(_nPadIdx);
	if (vAxisRight.Length()>0.3f)						// deadzone
	{
		_pPlayer->SetInputRacket(vAxisRight);
	}
	else
	{
		_pPlayer->SetInputRacket(hgeVector(0,0));
	}
}
