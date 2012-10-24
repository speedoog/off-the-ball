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


#include "BallRecord.h"

#include "Game.h"
#include "../Base/TStream.h"

const Float32 BallRecord::_rFrameTime =(1.0f/60.0f);

// ********************************************
//	Ctor
// ********************************************
BallRecorder::BallRecorder()
: _pGame		(NULL)
, _bRecording	(false)
, _pBestMatch	(NULL)
, _bDbgBest		(true)
, _bDbgRecord	(false)
, _bDbgHeat		(false)
{
}

// ********************************************
//	Dtor
// ********************************************
BallRecorder::~BallRecorder()
{
}

// ********************************************
//	Init
// ********************************************
void BallRecorder::Init(Game* pGame)
{
	_bRecording =false;
	_pGame		=pGame;

	_BallRecord.InitNew();

	// Load DB
	TStream Stream;
	Bool bOk =Stream.OpenFile("AI/test.bin", TStream::SM_READ);
	if (bOk)
	{
		Stream >> _Database;
		Stream.CloseFile();
	}
}

// ********************************************
//	Kill
// ********************************************
void BallRecorder::Kill()
{
	// Save DB
	TStream Stream;
	Stream.OpenFile("AI/test.bin", TStream::SM_WRITE);
	Stream << _Database;
	Stream.CloseFile();
}

// ********************************************
//	StartRecord
// ********************************************
void BallRecorder::StartRecord()
{
	Ball& ball =_pGame->GetBall();
	Int32 nBallSide =ball.GetSide();

	CommandAbc* pCmd =_pGame->GetCmd(_pGame->GetBall().GetSide());
	if (pCmd->GetCmdType()==CommandAbc::CMD_CPU)
	{
		_pBestMatch =_Database.FindBest(ball);
		if (_pBestMatch)
		{
			++(_pBestMatch->_nTry);
		}
	}

//	CommandAbc* pCmd =_pGame->GetCmd(nBallSide);
// 	if (pCmd->GetCmdType()!=CommandAbc::CMD_PAD)
// 		return;
	
	_bRecording		=true;
	_rLastFrameTime	=0.0f;
	_rCurrentTime	=0.0f;
	_BallRecord.InitNew();

	InsertFrame();

	const hgeVector& vVelocity =ball.GetVelocity();
	_BallRecord._vInitialBallPos	=ball.GetPos();
	_BallRecord._vInitialBallVelocity=ball.GetVelocity();
	_BallRecord._nBallSide			=nBallSide;
}

// ********************************************
//	StopRecord
// ********************************************
void BallRecorder::StopRecord(const Bool bSuccess)
{
	if (_bRecording)
	{
		_bRecording =false;

		// Save record only if success here
		if (bSuccess)
		{
			CommandAbc* pCmd =_pGame->GetCmd(1-_pGame->GetBall().GetSide());
			if (pCmd->GetCmdType()==CommandAbc::CMD_CPU)
			{
				if (_pBestMatch)
				{
					++(_pBestMatch->_nSucced);
				}
			}

			if (_BallRecord._aArray.GetSize()>1)
			{
				_BallRecord.FixSide();
				_Database._lDatabase.PushTail(_BallRecord);
			}
		}
	}
	_pBestMatch =NULL;
}

// ********************************************
//	Update
// ********************************************
void BallRecorder::Update(const Float32 rDeltaTime)
{
	if (_bRecording)
	{
		_rCurrentTime +=rDeltaTime;
		Float32 rDeltaFrame =_rCurrentTime-_rLastFrameTime;
		if (rDeltaFrame>BallRecord::_rFrameTime)
		{
			InsertFrame();
		}
	}
}

// ********************************************
//	Render
// ********************************************
void BallRecorder::Render()
{
	const Bool bReverse =(_pGame->GetBall().GetSide()==0);
	if (_bDbgBest)
	{
		if (_pBestMatch)
		{
			_pBestMatch->Render(0x60FF0000, bReverse);
		}
	}

	if (_bDbgRecord)
	{
		if (_bRecording)
		{
			_BallRecord.Render(0xFF00FF00);
		}
	}

	if (_bDbgHeat)
	{
		FOR_EACH(_Database._lDatabase, it, BallRecordVector)
		{
			(*it).Render(0x100000FF, false, false);
		}
	}
}

// ********************************************
//	InsertFrame
// ********************************************
void BallRecorder::InsertFrame()
{
	// insert new frame
	BallRecordFrame& ballFrameRecord =_BallRecord.NewFrame();

	// fill info
	Ball& ball =_pGame->GetBall();
	Int32 nBallSide =ball.GetSide();
	Player& player =_pGame->GetPlayer(nBallSide);

	ballFrameRecord._vBallPos			=ball.GetPos();
	ballFrameRecord._vInputMove			=player.GetInputMove();
	ballFrameRecord._vInputRaquet		=player.GetInputRacket();
	ballFrameRecord._rPlayerPositionX	=player.GetPosition().x;

	_rLastFrameTime =_rCurrentTime;
}

// ********************************************
//	DeleteCurrentRecord
// ********************************************
void BallRecorder::DeleteCurrentRecord()
{
	if (_pBestMatch)
	{
		UInt32 nIndex =_pBestMatch-_Database._lDatabase.GetArray();
		_Database._lDatabase.RemoveIndexFast(nIndex);
		_pBestMatch =NULL;
	}
}

// ********************************************
//	Render
// ********************************************
void BallRecord::Render(const UInt32 nColor, const Bool bReverse, const Bool bBallVelocity)
{
	const Float32 rReverse =(bReverse)?-1.0f:1.0f;

	const BallFrameArray& aArray =_aArray;
	UInt32 nSize =aArray.GetSize();
	if (nSize>=2)
	{
		UInt32 nColors[] ={ nColor|0x00FFFFFF, nColor };
		const BallRecordFrame* pPrev =aArray.GetArray();
		for(UInt32 iFrame =1; iFrame<nSize; ++iFrame)
		{
			const BallRecordFrame* pNext =pPrev+1;

			hge->Gfx_RenderLine(rReverse * pPrev->_vBallPos.x, pPrev->_vBallPos.y, 
								rReverse * pNext->_vBallPos.x, pNext->_vBallPos.y,
								nColors[iFrame&1] );
			pPrev =pNext;
		}

		if (bBallVelocity)
		{
			hge->Gfx_RenderCircle(rReverse*_vInitialBallPos.x, _vInitialBallPos.y, 0.05f, nColors[1]);

			hgeVector vVelocity =_vInitialBallVelocity;
			hgeVector vEnd =_vInitialBallPos + vVelocity*0.15f;
			hge->Gfx_RenderLine(_vInitialBallPos.x, _vInitialBallPos.y,
								vEnd.x*rReverse, vEnd.y, nColors[0] );
		}

	}
}

// ********************************************
//	FindBest
// ********************************************
BallRecord*	BallRecordDB::FindBest(Ball& ball)
{
	hgeVector vVelocity =ball.GetVelocity();
	Int32 nBallSide =ball.GetSide();
	if (nBallSide==0)
	{
		vVelocity.x =-vVelocity.x;
	}

	const Float32 rBallPosY	=ball.GetPos().y;
// 	const Float32 rBallAngle=vVelocity.Angle();
 	const Float32 rBallSpeed=vVelocity.Length();

	BallRecord* pBest =NULL;
	Float32 rBestScore=999999;
	FOR_EACH(_lDatabase, it, BallRecordVector)
	{
		BallRecord& ballRec =*it;
		Float32 dY =TChangeRange(0.0f, 10.0f,		0.0f, 3.0f, TAbs(ballRec._vInitialBallPos.y-rBallPosY));
// 		Float32	dA =TChangeRange(-M_PI_2, M_PI_2,	0.0f, 2.0f, TAbs(ballRec._rInitialBallAngle-rBallAngle));
// 		Float32 dS =TChangeRange(0.0f, 10.0f,		0.0f, 1.0f, TAbs(ballRec._rInitialBallSpeed-rBallSpeed));
		hgeVector vDiff =ballRec._vInitialBallVelocity-vVelocity;
		Float32 dV =/*TChangeRange(0.0f, 10.0f,		0.0f, 1.0f,*/ vDiff.Length()/rBallSpeed;

		Float32 rRatio =Float32(ballRec._nSucced)/Float32(ballRec._nTry);

		Float32 rScore =(dY+dV)/rRatio;

		if (rScore<rBestScore)
		{
			rBestScore=rScore;
			pBest	  =&ballRec;
		}
	}
	
	return pBest;
}
