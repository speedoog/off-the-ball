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

#ifndef __BALLRECORD_H__
#define __BALLRECORD_H__
#pragma once

#include "../Base/Base.h"
#include "../Base/TVector.h"
#include "../Base/TString.h"
#include "../Base/TList.h"

class BallRecordFrame
{
public:
				BallRecordFrame()	{ }
				~BallRecordFrame()	{ }
	void		RevertSide()
	{
		_vBallPos.x			=-_vBallPos.x;
		_vInputRaquet.x 	=-_vInputRaquet.x;
		_vInputMove.x		=-_vInputMove.x;
		_rPlayerPositionX	=-_rPlayerPositionX;
	}

	hgeVector 	_vBallPos;
	hgeVector 	_vInputRaquet;
	hgeVector 	_vInputMove;
	Float32		_rPlayerPositionX;
};

typedef TVector<BallRecordFrame>	BallFrameArray;
class BallRecordData
{
public:
	void				InitNew()		{ _aArray.Reserve(512); Clear(); }
	BallRecordFrame&	NewFrame()		{ _aArray.Resize(_aArray.GetSize()+1); return *_aArray.GetLast(); }
	void				Clear()			{ _aArray.Clear(); }
	void				RevertSide()
	{
		FOR_EACH(_aArray, it, BallFrameArray)
		{
			(*it).RevertSide();
		}
	}

	BallFrameArray	_aArray;
};

class BallRecord
{
public:
	static  const Float32 _rFrameTime;
	static  UInt32		GetFrameFromTime(const Float32 rTime)	{ return UInt32(rTime/_rFrameTime);		}
	static  Float32		GetTimeFromFrame(const UInt32 nFrame)	{ return Float32(nFrame)*_rFrameTime;	}

	BallRecord() {}

	void	Clear()		{ _Data.Clear(); }
	void	Render(const UInt32 nColor);
	void	FixSide()
	{
		if (_nBallSide==0)
		{
			_Data.RevertSide();
			if (_rInitialBallAngle>0)
			{
				_rInitialBallAngle =M_PI-_rInitialBallAngle;
			}
			else
			{
				_rInitialBallAngle =-M_PI-_rInitialBallAngle;
			}
			_nBallSide =1;
		}
	}

	// stream
	inline friend void	operator << (TStream& Stream, const BallRecord& ballRecord)
	{
		Stream << ballRecord._rInitialBallAngle;
		Stream << ballRecord._rInitialBallSpeed;
		Stream << ballRecord._vInitialBallPos;
		Stream << ballRecord._Data._aArray;
	}
	inline friend void	operator >> (TStream& Stream, BallRecord& ballRecord)
	{
		Stream >> ballRecord._rInitialBallAngle;
		Stream >> ballRecord._rInitialBallSpeed;
		Stream >> ballRecord._vInitialBallPos;
		Stream >> ballRecord._Data._aArray;
	}

	BallRecordFrame& GetFrame(const Float32 rTime)
	{
		return GetFrame(GetFrameFromTime(rTime));
	}

	BallRecordFrame& GetFrame(const UInt32 nFrame)
	{
		UInt32 nFrameClamped =TMin(nFrame, _Data._aArray.GetSize()-1);
		return _Data._aArray[nFrameClamped];
	}

	UInt32 GetBestMatch(const hgeVector& vBallPos, const UInt32 nStart, const UInt32 nWindowSize, Float32& rDiff)
	{
		UInt32  nBestMatch=nStart;
		Float32 rBestDistSq =99999;
		UInt32 nEnd =TMin(nStart+nWindowSize, _Data._aArray.GetSize());

		for(UInt32 i=nStart; i<nEnd; ++i)
		{
			BallRecordFrame& frame =_Data._aArray[i];
 			hgeVector vDiff =frame._vBallPos-vBallPos;
 			Float32 rDistSq =vDiff.Length();
//			Float32 rDistSq =TAbs(frame._vBallPos.y-vBallPos.y);
			if (rDistSq<rBestDistSq)
			{
				nBestMatch  =i;
				rBestDistSq =rDistSq;
			}
		}
		rDiff =rBestDistSq;
		return nBestMatch;
	}

	hgeVector 			_vInitialBallPos;
	UInt32				_nBallSide;
	Float32				_rInitialBallAngle;
	Float32				_rInitialBallSpeed;

	BallRecordData		_Data;
};

class Ball;
class BallRecordDB
{
public:
				BallRecordDB()	{ }
				~BallRecordDB()	{ }

	BallRecord*	FindBest(Ball& ball);

	// stream
	inline friend void	operator << (TStream& Stream, const BallRecordDB& ballRecordDb)
	{
		Stream << ballRecordDb._lDatabase;
	}
	inline friend void	operator >> (TStream& Stream, BallRecordDB& ballRecordDB)
	{
		Stream >> ballRecordDB._lDatabase;
		FOR_EACH_ELEMENT_OF_DLIST_SAFE(ballRecordDB._lDatabase, it, TList<BallRecord>)
		{
			BallRecord& ballRec =*it;
			UInt32 nSize =ballRec._Data._aArray.GetSize();
			if (nSize>100 || nSize<25)
			{
				ballRecordDB._lDatabase.Remove(it);
			}
		}
	}

public:
	TList<BallRecord>	_lDatabase;

};

class Game;
class BallRecorder
{
public:

						BallRecorder();
						~BallRecorder();
			void		Init(Game* pGame);
			void		Kill();

			void		StartRecord();
			void		StopRecord(const Bool bSuccess);
			void		Update(const Float32 rDeltaTime);
			void		Render();

			void		DeleteCurrentRecord();
	inline	BallRecord*	GetBestMatch()	{ return _pBestMatch; }
protected:
			void		InsertFrame();

protected:
	Game*				_pGame;
	Bool				_bRecording;
	BallRecord			_BallRecord;	
	Float32				_rLastFrameTime;
	Float32				_rCurrentTime;

	BallRecordDB		_Database;

	BallRecord*			_pBestMatch;
};

#endif	//__BALLRECORD_H__
