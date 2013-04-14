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
#include "../Game/Game.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
CommandMouse::CommandMouse()
{
	_nCmdType =CMD_TRACKPAD;
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
CommandMouse::~CommandMouse()
{
}

// ****************************************************************************************
//	OnInit
// ****************************************************************************************
void CommandMouse::OnInit(const UInt32 nPlayerId)
{
	hge->Input_GetMousePos(&_vLastMousePosition.x, &_vLastMousePosition.y);
	_vCirclePos =hgeVector(0,0);
}

// ****************************************************************************************
//	OnUpdate
// ****************************************************************************************
void CommandMouse::OnUpdate(const float rDeltaTime)
{
	float rLeft =hge->Input_GetKeyState(HGEK_LBUTTON)?1.0f:0.0f;
	float rRight=hge->Input_GetKeyState(HGEK_LBUTTON)?1.0f:0.0f;
	float rUp	=hge->Input_GetKeyState(HGEK_SPACE)?1.0f:0.0f;
	_pPlayer->SetInputMove(hgeVector(rRight-rLeft, rUp));

	hgeVector vMousePos;
	hge->Input_GetMousePos(&vMousePos.x, &vMousePos.y);
	hgeVector vDelta=vMousePos-_vLastMousePosition;
	vDelta.y =-vDelta.y;
	_vLastMousePosition =vMousePos;

	_vCirclePos+=vDelta/30.0f;
	if (_vCirclePos.Length()!=0.0f)
	{
		if (_vCirclePos.Length()>1.0f)
		{
			_vCirclePos.Normalize();
		}
		_pPlayer->SetInputRacket(_vCirclePos);
	}
}
