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
	pFont =new hgeFont(filename);
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
	_pFont =LoadFont("Data/font1.fnt");
	_pFontScore =LoadFont("Data/FontScore.fnt");
}

// ********************************************
//	Kill
// ********************************************
void Resources::Kill()
{
	// delete all
	delete _pFont;
}