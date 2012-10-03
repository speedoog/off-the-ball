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

#include "BallHistory.h"

//-----------------------------------------------------------------------------
BallHistory::BallHistory(const Float32 rMinTimeBetweenSamples, const Float32 rMaxRecordingTime, const UInt32 nMaxFrameCount)
: _rMaxRecordingTime		(rMaxRecordingTime)
, _nMaxFrameCount			(nMaxFrameCount)
, _rMinTimeBetweenSamples	(rMinTimeBetweenSamples)
, _rCurrentTime				(0.0f)
, _bTeleport				(false)
, _bBlink					(false)
{
}

//-----------------------------------------------------------------------------
BallHistory::~BallHistory()
{
}

//-----------------------------------------------------------------------------
void BallHistory::Update(const hgeVector& vPosition, const Float32 rDeltaTime)
{
	if (rDeltaTime>0.0f)
	{
		_rCurrentTime +=rDeltaTime;

		// CleanUp oldies
		ClearOldFrames();

		BallFrame* pLastFrame =(BallFrame*)_lFrames.GetQueue();
		if ( !pLastFrame || _rCurrentTime>=(pLastFrame->_rTime+_rMinTimeBetweenSamples) || _bTeleport)
		{
			_bBlink =!_bBlink;

			// Push new frame
			BallFrame* pFrame =new BallFrame(vPosition, _bTeleport, _bBlink, _rCurrentTime);
			_lFrames.InsertQueue(pFrame);
		}
	}

	_bTeleport =false;
}

//-----------------------------------------------------------------------------
void BallHistory::ClearOldFrames()
{
	const Float32 rTimeOut =_rCurrentTime-_rMaxRecordingTime;

	BallFrame* pFrame =(BallFrame*)_lFrames.GetHead();
	while (pFrame!=NULL)
	{
		BallFrame* pNextFrame =(BallFrame*)pFrame->GetNext();
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

//-----------------------------------------------------------------------------
void BallHistory::Draw()
{
	BallFrame* pPrev =NULL;
	FOR_EACH_ELEMENT(_lFrames, pFrame, BallFrame)
	{
		if (pPrev!=NULL)
		{
			if (!pFrame->_bTeleport)
			{
				hge->Gfx_RenderLine(pPrev->_vPosition.x,	pPrev->_vPosition.y,
									pFrame->_vPosition.x,	pFrame->_vPosition.y, 0x50FFFF00);
			}
		}
		pPrev =pFrame;
	}
}
