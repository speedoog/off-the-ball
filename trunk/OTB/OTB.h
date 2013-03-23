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

#ifndef __OTB_H__
#define __OTB_H__

#pragma once


#include "Base/Base.h"
#include "Game/Game.h"
#include "Input/InputDirectX.h"
#include "Xml/XMLParser.h"

class Otb
{
public:
						Otb();
						~Otb();
			void		Start();

			bool		Update(const float rDeltaTime);
			bool		Render();

	// Accessors
	inline	Resources&	GetResources()					{ return _Resources;  		}
	inline	InputCore&	GetInputCommand()				{ return _Input;	 		}

protected:
			void		DrawInputs();
			void		LoadSettings();
			void		ApplyWorldTransform();

protected:
	XML_PARSER		_XmlParser;
	Game			_Game;
	InputDirectX	_PadManager;

	Resources		_Resources;
	InputCore		_Input;

	Bool			_bExitApp;
	Bool			_bChangeVideoSettings;

public:
	// video settings
	int  	_nScreenSizeX;
	int  	_nScreenSizeY;
	bool 	_bWindowed;

	// World Transform
	float 	_rWorldTxTop;
	float 	_rWorldTxBottom;
	float 	_rWorldTxCenterX;
	float 	_rWorldTxRotation;


};


#endif //__OTB_H__
