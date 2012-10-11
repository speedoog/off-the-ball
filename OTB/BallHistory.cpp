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


#include "../hge/hgeColor.h"
#include "../hge/hgeSprite.h"

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
void BallHistory::Update(const hgeVector& vPosition, const Float32 rAngle, const Float32 rDeltaTime)
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
			BallFrame* pFrame =new BallFrame(vPosition, _bTeleport, _bBlink, _rCurrentTime, rAngle);
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
void BallHistory::Reset()
{
	_lFrames.DeleteAllElements();
}

//-----------------------------------------------------------------------------
void BallHistory::Draw(hgeSprite* pSpriteBallTrail)
{
	UInt32 nElementCount =_lFrames.GetNbElements();
	BallFrame* pPrev =NULL;

	hgeColorRGB clSrc(0.0f, 0.0f, 0.0f, 0.0f);
	hgeColorRGB clDst(0.0f, 0.7f, 0.0f, 0.1f);
	hgeColorRGB clDst2(1.0f, 1.0f, 1.0f, 0.2f);

	UInt32 nCurrentElement =0;

	Float32 rScale =0.1f/8.0f;

	FOR_EACH_ELEMENT(_lFrames, pFrame, BallFrame)
	{
		if (pPrev!=NULL)
		{
			if (!pFrame->_bTeleport)
			{
				Float32 rRatio =Float32(nCurrentElement)/Float32(nElementCount);
				hgeColorRGB clCurrent =TBlend(clSrc, clDst, rRatio);

				pSpriteBallTrail->SetColor(clCurrent.GetHWColor());
				pSpriteBallTrail->RenderEx(pPrev->_vPosition.x, pPrev->_vPosition.y, pPrev->_rAngle, rScale*rRatio, rScale*rRatio);

				hgeVector vDiff(pFrame->_vPosition-pPrev->_vPosition);
				hgeVector vNorm(vDiff.y, -vDiff.x);
				vNorm.Normalize();

				hgeColorRGB clCurrent2 =TBlend(clSrc, clDst2, rRatio);

				for(UInt32 i=0; i<4; ++i)
				{
					Float32 rOffsetAngle =Float32(i)*M_PI_2;
					Float32 rSin1 =sinf(pPrev->_rAngle+rOffsetAngle)*rRatio;
					Float32 rSin2 =sinf(pFrame->_rAngle+rOffsetAngle)*rRatio;

					hgeVector v0 =pPrev->_vPosition+vNorm*0.1f*0.9f*rSin1;
					hgeVector v1 =pFrame->_vPosition+vNorm*0.1f*0.9f*rSin2;

					hge->Gfx_RenderLine(v0.x, v0.y, v1.x, v1.y, clCurrent2.GetHWColor());
				}

// 				hge->Gfx_RenderLine(pPrev->_vPosition.x,	pPrev->_vPosition.y,
// 									pFrame->_vPosition.x,	pFrame->_vPosition.y, clCurrent.GetHWColor());

				++nCurrentElement;
			}
		}
		pPrev =pFrame;
	}
}
