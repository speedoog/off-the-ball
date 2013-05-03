
#include "Audio.h"

// ****************************************************************************************
//	Ctor
// ****************************************************************************************
Audio::Audio()
: _hsMusic		(0)
, _rMusicVolume	(1.0f)
, _rFxVolume	(1.0f)
{
	int bInit =BASS_Init(-1, 44100, 0, 0, NULL);
}

// ****************************************************************************************
//	Dtor
// ****************************************************************************************
Audio::~Audio()
{
	BASS_Stop();
	BASS_Free();
}

// ****************************************************************************************
//	MusicLoad
// ****************************************************************************************
void Audio::MusicLoad(const char* pFilename)
{
	_hsMusic =BASS_StreamCreateFile(false, "data/music/freezepop_Lazy.mp3", 0, 0, 0);
	MusicSetVolume(_rMusicVolume);
}

// ****************************************************************************************
//	MusicPlay
// ****************************************************************************************
void Audio::MusicPlay()
{
	if (_hsMusic)
	{
		BASS_ChannelPlay(_hsMusic, false);
	}
}

// ****************************************************************************************
//	MusicStop
// ****************************************************************************************
void Audio::MusicStop()
{
	if (_hsMusic)
	{
		BASS_ChannelStop(_hsMusic);
	}
}

// ****************************************************************************************
//	MusicSetVolume
// ****************************************************************************************
void Audio::MusicSetVolume(const Float32 rVolume)
{
	_rMusicVolume =rVolume;
	if (_hsMusic)
	{
		BASS_ChannelSetAttribute(_hsMusic, BASS_ATTRIB_VOL, _rMusicVolume);
	}
}

// ****************************************************************************************
//	SampleLoad
// ****************************************************************************************
HSAMPLE Audio::SampleLoad(const char* pFilename)
{
	return BASS_SampleLoad(false, pFilename, 0, 0, 1, BASS_SAMPLE_OVER_VOL);
}

// ****************************************************************************************
//	SampleFree
// ****************************************************************************************
void Audio::SampleFree(const HSAMPLE hsSample)
{
	if (hsSample)
	{
		BASS_SampleFree(hsSample);
	}
}

// ****************************************************************************************
//	SamplePlay
// ****************************************************************************************
HCHANNEL Audio::SamplePlay(const HSAMPLE hsSample, const Float32 rVolume, const Float32 rPan, const Float32 rPitch, const Bool bLoop)
{
	if (hsSample==0)
	{
		return 0;
	}

	BASS_SAMPLE info;
	HCHANNEL chn;
	BASS_SampleGetInfo(hsSample, &info);

	chn =BASS_SampleGetChannel(hsSample, false);

	BASS_ChannelSetAttribute(chn, BASS_ATTRIB_FREQ, rPitch*Float32(info.freq));
	BASS_ChannelSetAttribute(chn, BASS_ATTRIB_VOL, rVolume*_rFxVolume);
	BASS_ChannelSetAttribute(chn, BASS_ATTRIB_PAN, rPan);

	info.flags &= ~BASS_SAMPLE_LOOP;
	if(bLoop)
	{
		info.flags |= BASS_SAMPLE_LOOP;
	}

	BASS_ChannelFlags(chn, info.flags, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(chn, TRUE);
	return chn;
}
