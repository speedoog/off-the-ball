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

#ifndef __RESOURCES_H__
#define __RESOURCES_H__
#pragma once

#include "Base/Base.h"
#include "../HGE/hgefont.h"
#include "Audio/Audio.h"

class Resources
{
public:
				Resources();
				~Resources();

	void		Init();
	void		Kill();

	hgeFont* 	_pFontDebug;
	hgeFont* 	_pFontScore;
	hgeFont* 	_pFontTitle;
	hgeFont* 	_pFontMenus;
	hgeFont* 	_pFontMessages;

	HTEXTURE	_TexBall;
	hgeSprite*	_pSpriteBall;
	hgeSprite*	_pSpriteBallTrail;

	HTEXTURE	_texPowerBar;

	HTEXTURE	_texRacketTrail;
	hgeSprite*	_pSpriteRacketTrail;

	static UInt32 ColorPlayerBody[];

	// help
	HTEXTURE	_texPadXbox;
	hgeSprite*	_pSpritePadXbox;

	// Sound
	HSAMPLE		_hsBoing, _hsShoot, _hsNetHit, _hsRacketFlap, _hsZion;
	HSAMPLE		_hsMenuChangeItem, _hsMenuValidate, _hsMenuCancel;

protected:
	hgeFont*	LoadFont(const char* filename, const Float32 rScale =1.0f);
};

#endif	//__RESOURCES_H__
