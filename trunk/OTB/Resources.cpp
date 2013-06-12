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
#include "Otb.h"

UInt32 Resources::ColorPlayerBody[]	={ 0xFF39588E, 0xFFBE3839};

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Resources::Resources()
{
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
Resources::~Resources()
{
}

// ****************************************************************************************
//	LoadFont
// ****************************************************************************************
hgeFont* Resources::LoadFont(const char* filename, const Float32 rScale)
{
	hgeFont* pFont;
	pFont =new hgeFont(filename, true);
	pFont->SetProportion(-1);

	pFont->SetScale(-0.01f * rScale);
	return pFont;
}

// ****************************************************************************************
//	Init
// ****************************************************************************************
void Resources::Init()
{
	// load all
	_pFontDebug		=LoadFont("Data/Verdana60.fnt", 0.4f);
	_pFontScore		=LoadFont("Data/Bauhaus93.fnt");
	_pFontTitle		=LoadFont("Data/Bauhaus93.fnt");
	_pFontMenus		=LoadFont("Data/Verdana60.fnt", 0.5f);
	_pFontMessages	=LoadFont("Data/Lucida70.fnt");

// 	_BallTexture =hge->Texture_Load("Data/TennisBall3.png");
// 	_pSpriteBall =new hgeSprite(_BallTexture,0,0,16,16);
// 	_pSpriteBall->SetHotSpot(8,8);

	const Float32 TileSize =32.0f;
	const int x=3;
	const int y=2;
	_TexBall =hge->Texture_Load("Data/particles.png");
	_pSpriteBall =new hgeSprite(_TexBall, x*TileSize, y*TileSize, TileSize, TileSize);
	_pSpriteBall->SetHotSpot(16,16);

	_pSpriteBallTrail =new hgeSprite(*_pSpriteBall);

	_texPowerBar =hge->Texture_Load("Data/PowerBar.png");

	_texRacketTrail =hge->Texture_Load("Data/RacketTrail.png");
	_pSpriteRacketTrail =new hgeSprite(_texRacketTrail, 0.0f, 0.0f, 16.0f, 16.0f);
	_pSpriteRacketTrail->SetBlendMode(BLEND_COLORADD);

	// ---- help ----
	_texPadXbox =hge->Texture_Load("Data/Common_Controller_NoStick.png");
	const int nPadXboxSizeX =hge->Texture_GetWidth(_texPadXbox, true);
	const int nPadXboxSizeY =hge->Texture_GetHeight(_texPadXbox, true);
	_pSpritePadXbox	=new hgeSprite(_texPadXbox, 0, 0, nPadXboxSizeX, nPadXboxSizeY);
	_pSpritePadXbox->SetHotSpot(nPadXboxSizeX/2,nPadXboxSizeY/2);

	_texPadStick =hge->Texture_Load("Data/Stick.png");
	const int nPadStickSizeX =hge->Texture_GetWidth(_texPadStick, true);
	const int nPadStickSizeY =hge->Texture_GetHeight(_texPadStick, true);
	_pSpritePadStick	=new hgeSprite(_texPadStick, 0, 0, nPadStickSizeX, nPadStickSizeY);
	_pSpritePadStick->SetHotSpot(nPadStickSizeX/2,nPadStickSizeY/2);

	// ---- AUDIO ---
	Audio& audio =Otb::GetInstance()->GetAudio();
	_hsBoing 			=audio.SampleLoad("Data/snd/player_bounce3.wav");
	_hsShoot 			=audio.SampleLoad("Data/snd/shine_reject2.wav");
	_hsNetHit			=audio.SampleLoad("Data/snd/bang1.wav");
	_hsRacketFlap		=audio.SampleLoad("Data/snd/flap.wav");
	_hsMenuChangeItem	=audio.SampleLoad("Data/snd/menu.wav");
	_hsZion				=audio.SampleLoad("Data/snd/zion.wav");
	_hsMenuValidate		=audio.SampleLoad("Data/snd/bedroom_switch_a.wav");
	_hsMenuCancel		=audio.SampleLoad("Data/snd/bedroom_switch_b.wav");
}

// ****************************************************************************************
//	Kill
// ****************************************************************************************
void Resources::Kill()
{
	// delete all
	TDelete(_pFontDebug);
	TDelete(_pFontScore);
	TDelete(_pFontTitle);
	TDelete(_pFontMenus);
	TDelete(_pFontMessages);

	hge->Texture_Free(_TexBall);
	hge->Texture_Free(_texPowerBar);

	TDelete(_pSpriteBall);
	TDelete(_pSpriteBallTrail);

	// ---- AUDIO ---
	Audio& audio =Otb::GetInstance()->GetAudio();
	audio.SampleFree(_hsBoing 			);
	audio.SampleFree(_hsShoot 			);
	audio.SampleFree(_hsNetHit			);
	audio.SampleFree(_hsRacketFlap		);
	audio.SampleFree(_hsMenuChangeItem	);
	audio.SampleFree(_hsZion			);
	audio.SampleFree(_hsMenuValidate	);
	audio.SampleFree(_hsMenuCancel		);

}
