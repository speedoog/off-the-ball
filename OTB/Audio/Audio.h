
#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma once

#include "../Base/Base.h"
#include "../../Bass/bass.h"

class Audio
{
public:
				Audio();
				~Audio();

	// music
	void		MusicLoad(const char* pFilename);
	void		MusicPlay();
	void		MusicStop();
	void		MusicSetVolume(const Float32 rVolume);

	// Samples
	HSAMPLE		SampleLoad(const char* pFilename);
	void		SampleFree(const HSAMPLE hsSample);
	HCHANNEL	SamplePlay(const HSAMPLE hsSample, const Float32 rVolume=1.0f, const Float32 rPan=0.0f, const Float32 rPitch=1.0f, const Bool bLoop =false);

	// Channel
	void		ChannelSetAttrib(const HCHANNEL	chan, const UInt32 nProp, const Float32 rValue);

protected:
//	HSTREAM _hsMusic;		// BG music
	HMUSIC	_hsMusic;		// BG music

	// Settings
	Float32	_rMusicVolume;
	Float32	_rFxVolume;
};

#endif //__AUDIO_H__
