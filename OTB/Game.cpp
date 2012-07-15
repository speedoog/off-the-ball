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
}

// ********************************************
//	Init
// ********************************************
void Game::NewBall()
{
	int nSide =_Ball.GetSide();
	_Players[1-nSide].IncScore();

	_Level.Init(hgeVector(7.0f,7.5f), 1.0f);
	_Players[0].Init(this, 0);
	_Players[1].Init(this, 1);
	_Ball.Init(this);
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

	_Level.Update(rDeltaTime);
	_Ball.Update(rDeltaTime);
	_Players[0].Update(rDeltaTime);
	_Players[1].Update(rDeltaTime);

	PcPadManager& Pad =GetPadManager();
	if (Pad.GetCtrlState(PcPadManager::PAD_BTN_SELECT))
	{
		NewBall();
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

	// render score (move it in another class ?)
	hgeVector vLvlSize =_Level.GetSize();
	hgeFont* pFont =GetResources()._pFontScore;
	pFont->printf(vLvlSize.x*0.98f, vLvlSize.y, HGETEXT_RIGHT, "%d", _Players[0].GetScore());
	pFont->printf(-vLvlSize.x*0.98f, vLvlSize.y, HGETEXT_LEFT, "%d", _Players[1].GetScore());
}
