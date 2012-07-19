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


#include "CommandKbdMouse.h"
#include "Game.h"

// ********************************************
//	Ctor
// ********************************************
CommandKbdMouse::CommandKbdMouse()
{
}

// ********************************************
//	Dtor
// ********************************************
CommandKbdMouse::~CommandKbdMouse()
{
}

// ********************************************
//	OnInit
// ********************************************
void CommandKbdMouse::OnInit()
{
	hge->Input_GetMousePos(&_vLastMousePosition.x, &_vLastMousePosition.y);
}

// ********************************************
//	OnUpdate
// ********************************************
void CommandKbdMouse::OnUpdate(const float rDeltaTime)
{
	float rLeft =hge->Input_GetKeyState(HGEK_LEFT)?1.0f:0.0f;
	float rRight=hge->Input_GetKeyState(HGEK_RIGHT)?1.0f:0.0f;

	float rMove =rRight-rLeft;
	_pPlayer->SetInputMove(hgeVector(rMove,0));

	hgeVector vMousePos;
	hge->Input_GetMousePos(&vMousePos.x, &vMousePos.y);

	hgeVector vDelta=vMousePos-_vLastMousePosition;
	vDelta.y =-vDelta.y;
	_pPlayer->SetInputRacket(vDelta);

	_vLastMousePosition =vMousePos;
}
