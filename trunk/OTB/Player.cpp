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
//                   �>   .                                      <�                     //
//                  /_                       |                    | ___                 //
//               __/\ `\                     |                   / \                    //
//                   \,                      |                 ,/  /                    //
// ------------------------------------------ ----------------------------------------- //
//                        Copyright(c) 2012 by Bertrand Faure                           //
//////////////////////////////////////////////////////////////////////////////////////////

#include "player.h"

#include "Game.h"
#include "PcPad.h"

float rCharSpeed 	=1.5f;
float rCharSizeX 	=0.35f;
float rCharSizeY 	=1.8f;
float rCharRacketY	=rCharSizeY*0.75f;
float rRacketLen	=rCharSizeY*0.6f;

hgeVector vRacketDir(0, 1.0f);

// ********************************************
//	Ctor
// ********************************************
Player::Player(Game* pGame)
{
	_pGame =pGame;
}

// ********************************************
//	Dtor
// ********************************************
Player::~Player()
{

}

// ********************************************
//	Update
// ********************************************
void Player::Update(const float rDeltaTime)
{
	PcPadManager& Pad =_pGame->GetPadManager();

	// move char
	const hgeVector& vAxisLeft =Pad.GetAxisLeft();
	_vPos.x +=vAxisLeft.x*rDeltaTime*rCharSpeed;

	// move racket
	const hgeVector& vAxisRight =Pad.GetAxisRight();
	if (vAxisRight.Length()>0.0f)
	{
		hgeVector vAxisRightNorm(vAxisRight);
		vAxisRightNorm.Normalize();
		vRacketDir =hgeVector(vAxisRightNorm.x, -vAxisRightNorm.y);
	}

}

// ********************************************
//	Render
// ********************************************
void Player::Render()
{
	hge->Gfx_RenderBox(_vPos.x-rCharSizeX, _vPos.y, _vPos.x+rCharSizeX, _vPos.y+rCharSizeY);

	// racket
	hge->Gfx_RenderLine(_vPos.x, rCharRacketY,
						_vPos.x+vRacketDir.x*rRacketLen, rCharRacketY+vRacketDir.y*rRacketLen, 0xFF60FF60);

}
