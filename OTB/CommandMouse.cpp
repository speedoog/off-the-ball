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


#include "CommandMouse.h"
#include "Game.h"

// ********************************************
//	Ctor
// ********************************************
CommandMouse::CommandMouse()
{
}

// ********************************************
//	Dtor
// ********************************************
CommandMouse::~CommandMouse()
{
}

// ********************************************
//	OnInit
// ********************************************
void CommandMouse::OnInit()
{
	hge->Input_GetMousePos(&_vLastMousePosition.x, &_vLastMousePosition.y);
}

// ********************************************
//	OnUpdate
// ********************************************
void CommandMouse::OnUpdate(const float rDeltaTime)
{
	float rLeft =hge->Input_GetKeyState(HGEK_LBUTTON)?1.0f:0.0f;
	float rRight=hge->Input_GetKeyState(HGEK_RBUTTON)?1.0f:0.0f;

	float rMove =rRight-rLeft;
	_pPlayer->SetInputMove(hgeVector(rMove,0));

	hgeVector vMousePos;
	hge->Input_GetMousePos(&vMousePos.x, &vMousePos.y);

	hgeVector vDelta=vMousePos-_vLastMousePosition;
	vDelta.y =-vDelta.y;
	if (vDelta.Length()<5)
		vDelta =hgeVector(0,0);
	_pPlayer->SetInputRacket(vDelta);

	_vLastMousePosition =vMousePos;
}
