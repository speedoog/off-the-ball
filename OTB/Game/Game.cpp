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

#include "../Cmd/CommandPad.h"
#include "../Cmd/CommandKbdMouse.h"
#include "../Cmd/CommandMouse.h"
#include "../Cmd/CommandCpu.h"
#include "../Xml/XMLParser.h"

const char* XML_SECTION_GAME		="Game";
const char* XML_SECTION_GAME_P1		="P1";
const char* XML_SECTION_GAME_P2		="P2";
const char* XML_ATTRIBUTE_CMD		="Cmd";
const char* XML_ATTRIBUTE_ID		="Id";

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
	case CommandAbc::CMD_CPU:
		return new CommandCpu();
	}
}

// ********************************************
//	Init
// ********************************************
void Game::Init(XML_PARSER* pXml)
{
	_rTimeScale =1.0f;

	HWND hwnd =hge->System_GetState(HGE_HWND);
	_Input.Init(hwnd);

	_Resources.Init();

	_Level.Init(this, hgeVector(7.0f,7.5f), 1.3f);
	_Players[0].Init(this, 0);
	_Players[1].Init(this, 1);
	_Ball.Init(this);
	_BallRecorder.Init(this);
	_Rules.Init(this);

	const int nOffset =4;
	const char* pCmd1Default =SMARTENUM_GET_STRING(CommandAbc::CmdType, CommandAbc::CMD_PAD)+nOffset;
	const char* pCmd2Default =SMARTENUM_GET_STRING(CommandAbc::CmdType, CommandAbc::CMD_KBDMOUSE)+nOffset;
	const char* pCmd1 =pCmd1Default;
	const char* pCmd2 =pCmd2Default;
	int nCmdId1 =0;
	int nCmdId2 =1;
	if (pXml)
	{
		XML_ELEMENT* pRoot =pXml->GetRoot();
		// Read Game
		if (pRoot)
		{
			XML_ELEMENT* pXmlGame =pRoot->FindElement(XML_SECTION_GAME, false);
			if (pXmlGame)
			{
				XML_ELEMENT* pXmlGameP1 =pXmlGame->FindElement(XML_SECTION_GAME_P1, false);
				if (pXmlGameP1)
				{
					pCmd1	=pXmlGameP1->GetAttribute(XML_ATTRIBUTE_CMD, pCmd1Default);
					nCmdId1	=pXmlGameP1->GetAttribute(XML_ATTRIBUTE_ID, 0);
				}
				XML_ELEMENT* pXmlGameP2 =pXmlGame->FindElement(XML_SECTION_GAME_P2, false);
				if (pXmlGameP2)
				{
					pCmd2	=pXmlGameP2->GetAttribute(XML_ATTRIBUTE_CMD, pCmd2Default);
					nCmdId2	=pXmlGameP2->GetAttribute(XML_ATTRIBUTE_ID, 1);
				}
			}
		}
	}

	// P1 Init
	CommandAbc::CmdType cmd1 =SMARTENUM_FIND(CommandAbc::CmdType, pCmd1, nOffset);
	_pCmd[0] =CreateCommand(cmd1);
	_pCmd[0]->Init(this, &_Players[0], nCmdId1);

	// P2 Init
	CommandAbc::CmdType cmd2 =SMARTENUM_FIND(CommandAbc::CmdType, pCmd2, nOffset);
	_pCmd[1] =CreateCommand(cmd2);
	_pCmd[1]->Init(this, &_Players[1], nCmdId2);

	_Rules.ActionStartGame(0);		// start w/ player[0]
}

// ********************************************
//	Kill
// ********************************************
void Game::Kill()
{
	_Resources.Kill();
	_Input.Kill();
	_Level.Kill();
	_Ball.Kill();
	_BallRecorder.Kill();
}

// ********************************************
//	Update
// ********************************************
void Game::Update(const float rDeltaTime)
{
	_Input.Update();

	if (GetInputCommand().GetCtrlStateFloat(0, InputMapper::PAD_BTN_VALIDATE)<0.5f)
	{
//		const Float32 rTimeFactor =TChangeRange(0.5f, 1.0f, 1.0f, 0.3f, GetInputCommand().GetCtrlStateFloat(0, InputMapper::PAD_TIME_SCALE));

		//	const Float32 rTimeFactor =(UseTimeScale()==true)?0.3f:1.0f;
		const Float32 rTimeScaled =rDeltaTime*_rTimeScale;

		const Float32 rSimulationFrameRate =600.0f;
		const UInt32 nSliceCount =1+rSimulationFrameRate*rTimeScaled;
		const Float32 rSliceTime =1.0f/rSimulationFrameRate;
		for(UInt32 i=0; i<nSliceCount; ++i)
		{
			_pCmd[0]->Update(rSliceTime);
			_pCmd[1]->Update(rSliceTime);
			_Level.Update(rSliceTime);
			_Ball.Update(rSliceTime);
			_BallRecorder.Update(rSliceTime);
			_Players[0].Update(rSliceTime);
			_Players[1].Update(rSliceTime);
		}
	}

	// Cheat to restart (select btn)
	InputCore& Input =GetInputCommand();
	if (Input.GetCtrlState(0, InputMapper::PAD_BTN_PAUSE))
	{
		_Rules.ActionServiceStart(0);
	}

	_BallRecorder._bDbgBest		^=hge->Input_KeyDown(HGEK_1);
	_BallRecorder._bDbgRecord	^=hge->Input_KeyDown(HGEK_2);
	_BallRecorder._bDbgHeat		^=hge->Input_KeyDown(HGEK_3);

	if (hge->Input_GetKeyState(HGEK_DELETE)!=0)
	{
		_BallRecorder.DeleteCurrentRecord();
	}

	if (hge->Input_KeyDown(HGEK_5))		_rTimeScale =0.2f;
	if (hge->Input_KeyDown(HGEK_6))		_rTimeScale =0.5f;
	if (hge->Input_KeyDown(HGEK_7))		_rTimeScale =1.0f;
	if (hge->Input_KeyDown(HGEK_8))		_rTimeScale =2.0f;
	if (hge->Input_KeyDown(HGEK_9))		_rTimeScale =10.0f;
	if (hge->Input_KeyDown(HGEK_0))		_rTimeScale =100.0f;
}

// ********************************************
//	Render
// ********************************************
void Game::Render()
{
	_Level.Render();
	_Ball.Render();
	_BallRecorder.Render();
	_Players[0].Render();
	_Players[1].Render();

	_pCmd[0]->Render();
	_pCmd[1]->Render();

	_Rules.Render();
}
