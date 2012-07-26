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

#include "CommandPad.h"
#include "CommandKbdMouse.h"
#include "CommandMouse.h"
#include "xml/XMLParser.h"
#include "Base/TString.h"

const char* XML_SECTION_GAME			="Game";
const char* XML_ATTRIB_GAME_P1CMD		="P1Cmd";
const char* XML_ATTRIB_GAME_P2CMD		="P2Cmd";

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

CommandAbc* CreateCommand(const CommandAbc::CmdType cmd)
{
	switch(cmd)
	{
	default:
	case CommandAbc::CMD_PAD:
		return new CommandPad();
	case CommandAbc::CMD_KBDMOUSE:
		return new CommandKbdMouse();
	case CommandAbc::CMD_TRACKPAD:
		return new CommandMouse();
	}
}

// ********************************************
//	Init
// ********************************************
void Game::Init(XML_PARSER* pXml)
{
	HWND hwnd =hge->System_GetState(HGE_HWND);
	_PadManager.Init(hwnd);

	_Resources.Init();

	_Level.Init(hgeVector(7.0f,7.5f), 1.0f);
	_Players[0].Init(this, 0);
	_Players[1].Init(this, 1);
	_Ball.Init(this);
	_Rules.Init(this);

	const int nOffset =4;
	const char* pCmd1Default =SMARTENUM_GET_STRING(CommandAbc::CmdType, CommandAbc::CMD_PAD)+nOffset;
	const char* pCmd2Default =SMARTENUM_GET_STRING(CommandAbc::CmdType, CommandAbc::CMD_KBDMOUSE)+nOffset;
	const char* pCmd1 =pCmd1Default;
	const char* pCmd2 =pCmd2Default;
	if (pXml)
	{
		XML_ELEMENT* pRoot =pXml->GetRoot();
		// Read Game
		{
			XML_ELEMENT* pXmlGame =pRoot->FindElement(XML_SECTION_GAME, false);
			if (pXmlGame)
			{
				pCmd1 =pXmlGame->GetAttribute(XML_ATTRIB_GAME_P1CMD, pCmd1Default);
				pCmd2 =pXmlGame->GetAttribute(XML_ATTRIB_GAME_P2CMD, pCmd1Default);
			}
		}
	}

	// P1 Init
	CommandAbc::CmdType cmd1 =SMARTENUM_FIND(CommandAbc::CmdType, pCmd1, nOffset);
	_pCmd0 =CreateCommand(cmd1);
	_pCmd0->Init(this, &_Players[0]);

	// P2 Init
	CommandAbc::CmdType cmd2 =SMARTENUM_FIND(CommandAbc::CmdType, pCmd2, nOffset);
	_pCmd1 =CreateCommand(cmd2);
	_pCmd1->Init(this, &_Players[1]);

	_Rules.ActionStartGame(0);		// start w/ player[0]
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

	_pCmd0->Update(rDeltaTime);
	_pCmd1->Update(rDeltaTime);

	_Level.Update(rDeltaTime);
	_Ball.Update(rDeltaTime);
	_Players[0].Update(rDeltaTime);
	_Players[1].Update(rDeltaTime);

	PcPadManager& Pad =GetPadManager();
	if (Pad.GetCtrlState(0, PcPadManager::PAD_BTN_SELECT))
	{
		_Rules.ActionServiceStart(0);
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

	_Rules.Render();
}
