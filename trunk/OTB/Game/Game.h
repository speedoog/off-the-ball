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

#ifndef __GAME_H__
#define __GAME_H__
#pragma once

#include "../Base/Base.h"
#include "Level.h"
#include "Player.h"
#include "Ball.h"
#include "BallRecord.h"
#include "../Resources.h"
#include "../Input/InputCommand.h"
#include "Rules.h"
#include "../Cmd/CommandAbc.h"

class XML_PARSER;
class Otb;

class Game
{
public:
							Game();
							~Game();

			void			InitByXml(Otb* pOTB, XML_PARSER* pXml=NULL);
			void			InitDemoMode(Otb* pOTB);
			void			Kill();

			void			Update(const float rDeltaTime);
			void			Render();

	// Accessors
			Resources&		GetResources();
			InputCore&		GetInputCommand();
	inline	Player&			GetPlayer(const UInt32 iSide)	{ return _Players[iSide]; 	}
	inline	Ball&			GetBall()						{ return _Ball;				}
	inline	BallRecorder&	GetBallRecorder()				{ return _BallRecorder;		}
	inline	Level&			GetLevel()						{ return _Level;			}
	inline	Rules&			GetRules()						{ return _Rules;			}
	inline	Bool			UseTimeScale()					{ return _Players[0].UseTimeScale() || _Players[1].UseTimeScale();	}
	inline	CommandAbc*		GetCmd(const UInt32 iSide)		{ return _pCmd[iSide];		}

	inline	Bool			GetShowPowerBar() const						{ return _bShowPowerBar;			}
	inline	void			SetShowPowerBar(const Bool bShowPowerBar) 	{ _bShowPowerBar =bShowPowerBar;	}

protected:
			void			InitBase(Otb* pOTB);

protected:
	Level			_Level;
	Player			_Players[2];
	Ball			_Ball;
	BallRecorder	_BallRecorder;
	Rules			_Rules;

	Otb*			_pOTB;

	Float32			_rTimeScale;
//	Bool			_bUseTimeScale;

	Bool			_bShowPowerBar;
	Bool			_bDemoMode;
	Bool			_bShowTitle;

	// temp ...
	CommandAbc*		_pCmd[2];
};

#endif	//__GAME_H__
