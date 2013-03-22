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


#include "Resources.h"

// ********************************************
//	Ctor
// ********************************************
Resources::Resources()
{
}

// ********************************************
//	Dtor
// ********************************************
Resources::~Resources()
{
}

// ********************************************
//	LoadFont
// ********************************************
hgeFont* Resources::LoadFont(const char* filename)
{
	hgeFont* pFont;
	pFont =new hgeFont(filename, true);
	pFont->SetProportion(-1);

	float scale =-0.01f;
	pFont->SetScale(scale);
	return pFont;
}

// ********************************************
//	Init
// ********************************************
void Resources::Init()
{
	// load all
	_pFontDebug		=LoadFont("Data/font1.fnt");
	_pFontScore		=LoadFont("Data/Bauhaus93.fnt");
	_pFontMessages	=LoadFont("Data/Lucida70.fnt");

	_BallTexture =hge->Texture_Load("Data/TennisBall3.png");
	_pSpriteBall =new hgeSprite(_BallTexture,0,0,16,16);
	_pSpriteBall->SetHotSpot(8,8);

	_pSpriteBallTrail =new hgeSprite(*_pSpriteBall);

	_texPowerBar =hge->Texture_Load("Data/PowerBar.png");
}

// ********************************************
//	Kill
// ********************************************
void Resources::Kill()
{
	// delete all
	delete _pFontDebug;

	hge->Texture_Free(_BallTexture);

}
