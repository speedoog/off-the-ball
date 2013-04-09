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

#ifndef __BALL_HISTORY_H__
#define __BALL_HISTORY_H__
#pragma once

#include "../Base/Base.h"
#include "../Base/TQuickList.h"

class hgeSprite;

class BallHistoryFrame : public TQuickListElement
{
public:
	BallHistoryFrame(const hgeVector& vPosition, const Bool bTeleport, const Bool bBlink, const Float32 rTime, const Float32 rAngle)
		: _vPosition(vPosition), _bTeleport(bTeleport), _bBlink(bBlink), _rTime(rTime), _rAngle(rAngle) { }

	hgeVector	_vPosition;
	Bool		_bTeleport;
	Bool		_bBlink;
	Float32		_rTime;
	Float32		_rAngle;
};

class BallHistory
{
public:
					BallHistory(const Float32 rMinTimeBetweenSamples, const Float32 rMaxRecordingTime, const UInt32 nMaxFrameCount);
					~BallHistory();

			 void	Update(const hgeVector& vBallPos, const Float32 rAngle, const Float32 rTimeElapsed);
			 void	Draw(hgeSprite*	pSpriteBallTrail);
			 void	Reset();

protected:
			 void	ClearOldFrames();

protected:
	Float32		_rMaxRecordingTime;
	UInt32		_nMaxFrameCount;
	Float32		_rMinTimeBetweenSamples;

	Float32		_rCurrentTime;
	TQuickList	_lFrames;

	Bool		_bTeleport;
	Bool		_bBlink;
};

#endif //__BALL_HISTORY_H__