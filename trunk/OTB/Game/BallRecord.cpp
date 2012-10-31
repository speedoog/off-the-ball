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
, _bDbgBest		(false)
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
	_Database.Init(pGame);

	_bRecording =false;
	_pGame		=pGame;

	_BallRecord.InitNew();

	// Load DB
	TStream Stream;
	Bool bOk =Stream.OpenFile("Database.AI", TStream::SM_READ);
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
	Stream.OpenFile("Database.AI", TStream::SM_WRITE);
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
				_BallRecord.CleanupMove();
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
//	CleanupMove
// ********************************************
void BallRecord::CleanupMove()
{
	UInt32 nBreaks =0;
	UInt32 nEraseStart =0;

	BallRecordFrame* pvStart=&_aArray[0];
	BallRecordFrame* pvMid	=&_aArray[1];
	UInt32 nEnd =_aArray.GetSize()-1;
	for (UInt32 iCurrent=1; iCurrent<nEnd; ++iCurrent)
	{
		BallRecordFrame* pvEnd =pvMid+1;
		hgeVector vDiff1 =(pvMid->_vBallPos)-(pvStart->_vBallPos);	vDiff1.Normalize();
		hgeVector vDiff2 =(pvEnd->_vBallPos)-(pvMid->_vBallPos);	vDiff2.Normalize();

		Float32 rDot =vDiff1.Dot(vDiff2);
		if (rDot<0.9f)
		{
			for(UInt32 iErase=nEraseStart; iErase<iCurrent; ++iErase)
			{
				_aArray[iErase]._rPlayerPositionX =pvMid->_rPlayerPositionX;
			}
			nEraseStart =iCurrent+1;
			++nBreaks;
		}

		pvStart =pvMid;
		pvMid	=pvEnd;
	}

	++nBreaks;
}

// ********************************************
//	FindBest
// ********************************************
BallRecord*	BallRecordDB::FindBest(Ball& ball)
{
	Float32 rNetY =_pGame->GetLevel().GetFutureNetY()+0.1f;

	hgeVector vVelocity =ball.GetVelocity();
	Int32 nBallSide =ball.GetSide();
	if (nBallSide==0)
	{
		vVelocity.x =-vVelocity.x;
	}

//	Float32 rMinRatio =TRand(0.1f, 0.8f);

	const Float32 rBallPosY	=ball.GetPos().y;
// 	const Float32 rBallAngle=vVelocity.Angle();
 	const Float32 rBallSpeed=vVelocity.Length();

	BallRecord* pBest =NULL;
	Float32 rBestScore=999999;
	FOR_EACH(_lDatabase, it, BallRecordVector)
	{
		BallRecord& ballRec =*it;
//		Float32 dY =TChangeRange(0.0f, 10.0f,		0.0f, 10.0f, TAbs(ballRec._vInitialBallPos.y-rBallPosY));
// 		Float32	dA =TChangeRange(-M_PI_2, M_PI_2,	0.0f, 2.0f, TAbs(ballRec._rInitialBallAngle-rBallAngle));
// 		Float32 dS =TChangeRange(0.0f, 10.0f,		0.0f, 1.0f, TAbs(ballRec._rInitialBallSpeed-rBallSpeed));
//		hgeVector vDiff =ballRec._vInitialBallVelocity-vVelocity;
//		Float32 dV =TChangeRange(0.0f, 10.0f,		0.0f, 10.0f, vDiff.Length());

		if ( (*ballRec._aArray.GetLast())._vBallPos.y < rNetY )
			continue;

		Float32 rDiff =BallRecord::ComputeDiff(rBallPosY, vVelocity, ballRec._vInitialBallPos.y, ballRec._vInitialBallVelocity);

		Float32 rRatio =Float32(ballRec._nSucced)/Float32(ballRec._nTry+1);
//		if (rRatio>=rMinRatio)
		{
			Float32 rScore =rDiff/rRatio;

			if (rScore<rBestScore)
			{
				rBestScore=rScore;
				pBest	  =&ballRec;
			}
		}
	}

	return pBest;
}
