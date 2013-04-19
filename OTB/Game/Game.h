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
#include "../Menus/MenuInGame.h"

class XML_PARSER;
class Otb;

class Game
{
public:
							Game();
							~Game();

			void			InitByXml(XML_PARSER* pXml=NULL);
			void			InitDemoMode();
			void			InitTrainingMode();
			void			InitSingle();
			void			InitVs();
			void			Kill();

			void			Update(const Float32 rDeltaTime);
			void			Render();

	// Accessors
			Resources&		GetResources();
			InputCore&		GetInputCommand();
	inline	Player&			GetPlayer(const UInt32 iSide)		{ return _Players[iSide]; 	}
	inline	Ball&			GetBall()							{ return _Ball;				}
	inline	BallRecorder&	GetBallRecorder()					{ return _BallRecorder;		}
	inline	Level&			GetLevel()							{ return _Level;			}
	inline	Rules&			GetRules()							{ return _Rules;			}
	inline	Bool			UseTimeScale()						{ return _Players[0].UseTimeScale() || _Players[1].UseTimeScale();	}
	inline	CommandAbc*		GetCmd(const UInt32 iSide)			{ return _pCmd[iSide];		}

	inline	Bool			GetTraining() const					{ return _bTraining;		}
	inline	void			SetTraining(const Bool bTraining) 	{ _bTraining =bTraining;	}
	inline	Bool			GetDemoMode() const					{ return _bDemoMode;		}
	inline	void			SetPaused(const Bool bPaused)		{ _bPaused =bPaused;		}

	static	CommandAbc*		CreateCommand(const CommandAbc::CmdType cmd);

protected:
			void			InitBase();
			void			InitAiMode();

protected:
	Level			_Level;
	Player			_Players[2];
	Ball			_Ball;
	BallRecorder	_BallRecorder;
	Rules			_Rules;

	Float32			_rTimeScale;
//	Bool			_bUseTimeScale;

	Bool			_bTraining;
	Bool			_bDemoMode;
	Bool			_bPaused;

	MenuInGame		_MenuInGame;

	// temp ...
	CommandAbc*		_pCmd[2];
};

#endif	//__GAME_H__
