#include "AudioClip.h"

AudioClip::AudioClip(std::string strName, MIX_Audio* pAudio)
{
	this->strName = strName;
	this->pAudio = pAudio;
}

AudioClip::~AudioClip()
{
	MIX_DestroyAudio(this->pAudio);
}

std::string AudioClip::getName() const
{
	return this->strName;
}

MIX_Audio* AudioClip::getAudio() const
{
	return this->pAudio;
}

Sint64 AudioClip::getDurationMS() const
{
	Sint64 nFrames = MIX_GetAudioDuration(this->pAudio);
	if (nFrames < 0) return -1;

	return MIX_AudioFramesToMS(this->pAudio, nFrames);
}
