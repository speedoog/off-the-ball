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


#include "../../hge/hgeColor.h"
#include "../../hge/hgeSprite.h"


typedef HistoryFrame<BallData> BallHistoryFrame;
// ****************************************************************************************
//	Draw
// ****************************************************************************************
void BallHistory::Draw(hgeSprite* pSpriteBallTrail, const Float32 rScale)
{
	UInt32 nElementCount =_lFrames.GetNbElements();
	BallHistoryFrame* pPrev =NULL;

	hgeColorRGB clSrc(0.0f, 0.0f, 0.0f, 0.0f);
	hgeColorRGB clDst(0.0f, 0.7f, 0.0f, 0.1f);
	hgeColorRGB clDst2(0.5f, 0.5f, 0.5f, 0.2f);

	UInt32 nCurrentElement =0;

// 	Float32 rScale =0.1f/8.0f;
// 	Float32 rScale =_rRadius/(pSpriteBall->GetWidth()/2.0f);

	FOR_EACH_ELEMENT(_lFrames, pFrame, BallHistoryFrame)
	{
		if (pPrev!=NULL)
		{
			if (!pFrame->_bTeleport)
			{
				Float32 rRatio =Float32(nCurrentElement)/Float32(nElementCount);
				hgeColorRGB clCurrent =TBlend(clSrc, clDst, rRatio);

				pSpriteBallTrail->SetColor(clCurrent.GetHWColor());

				pSpriteBallTrail->RenderEx(pPrev->_Data._vPosition.x, pPrev->_Data._vPosition.y, pPrev->_Data._rAngle, rScale*rRatio, rScale*rRatio);

				hgeVector vDiff(pFrame->_Data._vPosition-pPrev->_Data._vPosition);
				hgeVector vNorm(vDiff.y, -vDiff.x);
				vNorm.Normalize();

				hgeColorRGB clCurrent2 =TBlend(clSrc, clDst2, rRatio);

				for(Float32 i=0; i<(2.0f*M_PI); i+=(2.0f*M_PI)/3.0f)
				{
					Float32 rOffsetAngle =Float32(i);//*M_PI_2;
					Float32 rSin1 =sinf(pPrev->_Data._rAngle+rOffsetAngle)*rRatio;
					Float32 rSin2 =sinf(pFrame->_Data._rAngle+rOffsetAngle)*rRatio;

					hgeVector v0 =pPrev->_Data._vPosition+vNorm*0.1f*0.9f*rSin1;
					hgeVector v1 =pFrame->_Data._vPosition+vNorm*0.1f*0.9f*rSin2;

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

typedef HistoryFrame<RacketData> RacketHistoryFrame;
// ****************************************************************************************
//	Draw
// ****************************************************************************************
void RacketHistory::Draw(hgeSprite* pSpriteRacketTrail, const hgeColorRGB& colBase)
{
	UInt32 nElementCount =_lFrames.GetNbElements();
	RacketHistoryFrame* pPrev =NULL;

	hgeColorRGB clSrc(0.0f, 0.0f, 0.0f, 0.0f);
	hgeColorRGB clDst=colBase;	//(0.0f, 0.7f, 0.0f, 0.1f);
	hgeColorRGB clDst2(0.5f, 0.5f, 0.5f, 0.2f);

	UInt32 nCurrentElement =0;

	FOR_EACH_ELEMENT(_lFrames, pFrame, RacketHistoryFrame)
	{
		if (pPrev!=NULL)
		{
			if (!pFrame->_bTeleport)
			{
				Float32 rRatio =Float32(nCurrentElement)/Float32(nElementCount);
				hgeColorRGB clCurrent =TBlend(clSrc, clDst, rRatio);

				pSpriteRacketTrail->SetColor(clCurrent.GetHWColor());

				pSpriteRacketTrail->Render4V(  pPrev->_Data._v1.x, pPrev->_Data._v1.y
											 , pPrev->_Data._v2.x, pPrev->_Data._v2.y
											 , pFrame->_Data._v2.x, pFrame->_Data._v2.y
											 , pFrame->_Data._v1.x, pFrame->_Data._v1.y);

				/*
				pSpriteRacketTrail->RenderEx(pPrev->_Data._vPosition.x, pPrev->_Data._vPosition.y, pPrev->_Data._rAngle, rScale*rRatio, rScale*rRatio);

				hgeVector vDiff(pFrame->_Data._vPosition-pPrev->_Data._vPosition);
				hgeVector vNorm(vDiff.y, -vDiff.x);
				vNorm.Normalize();

				hgeColorRGB clCurrent2 =TBlend(clSrc, clDst2, rRatio);

				for(Float32 i=0; i<(2.0f*M_PI); i+=(2.0f*M_PI)/3.0f)
				{
					Float32 rOffsetAngle =Float32(i);//*M_PI_2;
					Float32 rSin1 =sinf(pPrev->_Data._rAngle+rOffsetAngle)*rRatio;
					Float32 rSin2 =sinf(pFrame->_Data._rAngle+rOffsetAngle)*rRatio;

					hgeVector v0 =pPrev->_Data._vPosition+vNorm*0.1f*0.9f*rSin1;
					hgeVector v1 =pFrame->_Data._vPosition+vNorm*0.1f*0.9f*rSin2;

					hge->Gfx_RenderLine(v0.x, v0.y, v1.x, v1.y, clCurrent2.GetHWColor());
				}

// 				hge->Gfx_RenderLine(pPrev->_vPosition.x,	pPrev->_vPosition.y,
// 									pFrame->_vPosition.x,	pFrame->_vPosition.y, clCurrent.GetHWColor());
				*/

				++nCurrentElement;
			}
		}
		pPrev =pFrame;
	}
}
