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
class BallRecord
{
public:
	static  const Float32 _rFrameTime;
	static  UInt32		GetFrameFromTime(const Float32 rTime)	{ return UInt32(rTime/_rFrameTime);		}
	static  Float32		GetTimeFromFrame(const UInt32 nFrame)	{ return Float32(nFrame)*_rFrameTime;	}

	BallRecord() : _nTry(1), _nSucced(1) {}

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

	void	Render(const UInt32 nColor, const Bool bReverse=false, const Bool bBallVelocity =true);
	void	FixSide()
	{
		if (_nBallSide==0)
		{
			RevertSide();
			_vInitialBallVelocity.x =-_vInitialBallVelocity.x;
			_nBallSide =1;
		}
	}

	// stream Write
	inline friend void	operator << (TStream& Stream, const BallRecord& ballRecord)
	{
		Stream << ballRecord._nTry;
		Stream << ballRecord._nSucced;
		Stream << ballRecord._vInitialBallVelocity;
		Stream << ballRecord._vInitialBallPos;
		Stream << ballRecord._aArray;
	}
	// Stream read
	inline friend void	operator >> (TStream& Stream, BallRecord& ballRecord)
	{
		Stream >> ballRecord._nTry;
		Stream >> ballRecord._nSucced;
		Stream >> ballRecord._vInitialBallVelocity;
		Stream >> ballRecord._vInitialBallPos;
		Stream >> ballRecord._aArray;
	}

	BallRecordFrame& GetFrame(const Float32 rTime)
	{
		return GetFrame(GetFrameFromTime(rTime));
	}

	BallRecordFrame& GetFrame(const UInt32 nFrame)
	{
		UInt32 nFrameClamped =TMin(nFrame, _aArray.GetSize()-1);
		return _aArray[nFrameClamped];
	}

	UInt32 GetBestMatch(const hgeVector& vBallPos, const UInt32 nStart, const UInt32 nWindowSize, Float32& rDiff)
	{
		UInt32  nBestMatch=nStart;
		Float32 rBestDistSq =99999;
		UInt32 nEnd =TMin(nStart+nWindowSize, _aArray.GetSize());

		for(UInt32 i=nStart; i<nEnd; ++i)
		{
			BallRecordFrame& frame =_aArray[i];
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

	hgeVector 		_vInitialBallPos;
	UInt32			_nBallSide;
	hgeVector		_vInitialBallVelocity;
	UInt32			_nTry, _nSucced;

	BallFrameArray	_aArray;
};

class Ball;

typedef	TVector<BallRecord>	BallRecordVector;
class BallRecordDB
{
public:
				BallRecordDB()	{ }
				~BallRecordDB()	{ }

	BallRecord*	FindBest(Ball& ball);

	// stream
	inline friend void	operator << (TStream& Stream, const BallRecordDB& ballRecordDB)
	{
		Stream << ballRecordDB._lDatabase;
	}
	inline friend void	operator >> (TStream& Stream, BallRecordDB& ballRecordDB)
	{
		Stream >> ballRecordDB._lDatabase;
		ballRecordDB.Cleanup();
	}

	inline void Cleanup()
	{
		for(BallRecordVector::Iterator it =_lDatabase.GetHead(); it!=_lDatabase.GetTail(); )
		{
			Bool bRemove =false;
			BallRecord& ballRec =*it;
			UInt32 nSize =ballRec._aArray.GetSize();

			if ((*ballRec._aArray.GetLast())._vBallPos.y<1.65f)
				bRemove =true;

			if (nSize>200 || nSize<50)
				bRemove =true;

			Float32 rRatio =Float32(ballRec._nSucced)/Float32(ballRec._nTry);
			if (rRatio<0.33f)
				bRemove =true;

			if (bRemove)
			{
				_lDatabase.Remove(it);
			}
			else
			{
				++it;
			}
		}

		// remove duplicate
		UInt32 nDup =0;
		for(BallRecordVector::Iterator it1 =_lDatabase.GetHead(); it1!=_lDatabase.GetTail(); ++it1)
		{
			BallRecord& br1 =*it1;
			for(BallRecordVector::Iterator it2 =it1+1; it2!=_lDatabase.GetTail(); )
			{
				BallRecord& br2 =*it2;
				if (	TEqual(br1._vInitialBallPos.y,br2._vInitialBallPos.y)
					&&	TEqual(br1._vInitialBallVelocity.x, br2._vInitialBallVelocity.x)
					&&	TEqual(br1._vInitialBallVelocity.y, br2._vInitialBallVelocity.y) )
				{
					_lDatabase.Remove(it2);
					++nDup;
				}
				else
				{
					++it2;
				}
			}
		}

		nDup ++;
	}

public:
	BallRecordVector	_lDatabase;
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

	Bool			_bDbgBest, _bDbgRecord, _bDbgHeat;

protected:
			void		InsertFrame();

protected:
	Game*			_pGame;
	Bool			_bRecording;
	BallRecord		_BallRecord;	
	Float32			_rLastFrameTime;
	Float32			_rCurrentTime;

	BallRecordDB	_Database;

	BallRecord*		_pBestMatch;

};

#endif	//__BALLRECORD_H__
