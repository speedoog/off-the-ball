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

#include "Game.h"

#include "CommandPad.h"
#include "CommandKbdMouse.h"
#include "CommandMouse.h"

// ********************************************
//	Ctor
// ********************************************
Game::Game()
{
}

// ********************************************
//	Dtor
// ********************************************
Game::~Game()
{

}

// ********************************************
//	Init
// ********************************************
void Game::Init()
{
	HWND hwnd =hge->System_GetState(HGE_HWND);
	_PadManager.Init(hwnd);

	_Resources.Init();

	_Level.Init(hgeVector(7.0f,7.5f), 1.0f);
	_Players[0].Init(this, 0);
	_Players[1].Init(this, 1);
	_Ball.Init(this);
	_Rules.Init(this);

	// !!!!! read from XML !!!!!!!!!!!!!
	_pCmd0 =new CommandPad();
	_pCmd0->Init(this, &_Players[0]);

	_pCmd1 =new CommandPad();
	_pCmd1->Init(this, &_Players[1]);

// 	_pCmd1 =new CommandMouse();
// 	_pCmd1->Init(this, &_Players[1]);

	_Rules.ActionStartGame(0);		// start w/ player[0]
}

// ********************************************
//	Kill
// ********************************************
void Game::Kill()
{
	_Resources.Kill();
	_PadManager.Kill();
}

// ********************************************
//	Update
// ********************************************
void Game::Update(const float rDeltaTime)
{
	_PadManager.Update();

	_pCmd0->Update(rDeltaTime);
	_pCmd1->Update(rDeltaTime);

	_Level.Update(rDeltaTime);
	_Ball.Update(rDeltaTime);
	_Players[0].Update(rDeltaTime);
	_Players[1].Update(rDeltaTime);

	PcPadManager& Pad =GetPadManager();
	if (Pad.GetCtrlState(0, PcPadManager::PAD_BTN_SELECT))
	{
		_Rules.ActionServiceStart(0);
	}
}

// ********************************************
//	Render
// ********************************************
void Game::Render()
{
	_Level.Render();
	_Ball.Render();
	_Players[0].Render();
	_Players[1].Render();

	_Rules.Render();
}
