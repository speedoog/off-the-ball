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

#ifndef __OTB_H__
#define __OTB_H__

#pragma once


#include "Base/Base.h"
#include "Game/Game.h"
#include "Menus/MenuMain.h"
#include "Input/InputDirectX.h"
#include "Xml/XMLParser.h"
#include "Audio/Audio.h"

class Otb
{
public:
						Otb();
						~Otb();
			void		Start();

			void		MainMenu();

			bool		Update(const Float32 rDeltaTime);
			bool		Render();

	// Accessors
	inline	Resources&	GetResources()					{ return _Resources;  	}
	inline	Game&		GetGame()						{ return _Game;			}
	inline	InputCore&	GetInputCommand()				{ return _Input;	 	}
	inline	XML_PARSER&	GetXmlTree()					{ return _XmlTree;		}
	inline	Audio&		GetAudio()						{ return _Audio;		}

	inline	void		ExitApp()						{ _bExitApp =true;		}

	static	Otb*		GetInstance()					{ return _pOTB;			}

	static	void		RenderQuad(const hgeVector& vMin, const hgeVector& vMax, const hgeColorRGB& color, HTEXTURE tex=0);
	static	void		RenderLine(const hgeVector& v0, const hgeVector& v1, const Float32 rZ, const hgeColorRGB& color, const Float32 rSize, HTEXTURE tex=0);
	static	void		RenderDisk(const hgeVector& vCenter, const Float32 rRadius, const Float32 rZ, const hgeColorRGB& color);

protected:
			void		LoadSettings();
			void		ApplyWorldTransform();

protected:
	// singleton
	static Otb*		_pOTB;

	// Main objects
	Audio			_Audio;
	XML_PARSER		_XmlTree;
	Game			_Game;
	MenuMain		_MenuMain;
	InputDirectX	_PadManager;
	Resources		_Resources;
	InputCore		_Input;

	// Exit request ?
	Bool			_bExitApp;

	// video settings
	Int32  			_nScreenSizeX;
	Int32  			_nScreenSizeY;
	Bool 			_bWindowed;

	// World Transform
	Float32 		_rWorldTxTop;
	Float32 		_rWorldTxBottom;
	Float32 		_rWorldTxCenterX;
	Float32 		_rWorldTxRotation;
};


#endif //__OTB_H__
