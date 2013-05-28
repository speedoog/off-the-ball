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

#ifndef __BALL_HISTORY_H__
#define __BALL_HISTORY_H__
#pragma once

#include "../Base/Base.h"
#include "../Base/TQuickList.h"


//
// -------------------------------
//

template <class TType>
class HistoryFrame : public TQuickListElement
{
public:
	HistoryFrame(const Float32 rTime, const Bool bTeleport, const Bool bBlink, const TType data)
		: _rTime(rTime), _bTeleport(bTeleport), _bBlink(bBlink), _Data(data) { }

	Float32		_rTime;
	Bool		_bTeleport;
	Bool		_bBlink;
	TType		_Data;
};

//
// -------------------------------
//
class BallData
{
public:
	BallData(const hgeVector& vPosition, const Float32 rAngle)
		: _vPosition(vPosition), _rAngle(rAngle) { }

	hgeVector	_vPosition;
	Float32		_rAngle;
};

typedef HistoryFrame<BallData> BallHistoryFrame;

//
// -------------------------------
//
template <class TType>
class History
{
public:
	typedef HistoryFrame<TType> HistoryFrameLocal;

					History(const Float32 rMinTimeBetweenSamples, const Float32 rMaxRecordingTime, const UInt32 nMaxFrameCount)
						: _rMaxRecordingTime		(rMaxRecordingTime)
						, _nMaxFrameCount			(nMaxFrameCount)
						, _rMinTimeBetweenSamples	(rMinTimeBetweenSamples)
						, _rCurrentTime				(0.0f)
						, _bTeleport				(false)
						, _bBlink					(false)
					{
					}

					~History() { }

			 void	Update(const Float32 rTimeElapsed, const TType& Data)
			 {
				 if (rTimeElapsed>0.0f)
				 {
					 _rCurrentTime +=rTimeElapsed;

					 // CleanUp oldies
					 ClearOldFrames();

					 HistoryFrameLocal* pLastFrame =(HistoryFrameLocal*)_lFrames.GetQueue();
					 if ( !pLastFrame || _rCurrentTime>=(pLastFrame->_rTime+_rMinTimeBetweenSamples) || _bTeleport)
					 {
						 _bBlink =!_bBlink;

						 // Push new frame
						 HistoryFrameLocal* pFrame =new HistoryFrameLocal(_rCurrentTime, _bTeleport, _bBlink, Data);
						 _lFrames.InsertQueue(pFrame);
					 }
				 }

				 _bTeleport =false;
			 }

			 void	Reset()
			 {
				 _lFrames.DeleteAllElements();
			 }

protected:
			 void	ClearOldFrames()
			 {
				const Float32 rTimeOut =_rCurrentTime-_rMaxRecordingTime;

				HistoryFrameLocal* pFrame =(HistoryFrameLocal*)_lFrames.GetHead();
				while (pFrame!=NULL)
				{
					HistoryFrameLocal* pNextFrame =(HistoryFrameLocal*)pFrame->GetNext();
					if (pFrame->_rTime<rTimeOut)			// Too old ... drop Head
					{
						_lFrames.DeleteElement(pFrame);
						pFrame =pNextFrame;
					}
					else
					{
						break;
					}
				}

				// Max Frame Count limit (for TimeScale near 0)
				while (_lFrames.GetNbElements()>=_nMaxFrameCount)
				{
					_lFrames.DeleteElement(_lFrames.GetHead());
				}
			}


protected:
	Float32		_rMaxRecordingTime;
	UInt32		_nMaxFrameCount;
	Float32		_rMinTimeBetweenSamples;

	Float32		_rCurrentTime;
	TQuickList	_lFrames;

	Bool		_bTeleport;
	Bool		_bBlink;
};

//
// -------------------------------
//
class hgeSprite;
class BallHistory : public History<BallData>
{
public:
	BallHistory (const Float32 rMinTimeBetweenSamples, const Float32 rMaxRecordingTime, const UInt32 nMaxFrameCount)
		: History(rMinTimeBetweenSamples, rMaxRecordingTime, nMaxFrameCount)
	{

	}

	void	Draw(hgeSprite*	pSpriteBallTrail, const Float32 rScale);

};

#endif //__BALL_HISTORY_H__
