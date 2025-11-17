#include "AudioPlayer.h"
#include <iostream>

AudioPlayer::AudioPlayer(std::string strName) : AComponent(strName, ComponentType::SCRIPT)
{
	this->mSpec.freq = 44100;
	this->mSpec.format = SDL_AUDIO_F32;
	this->mSpec.channels = 2;
	this->pStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &this->mSpec, audioStreamCallback, NULL);
	this->pClip = NULL;
	this->bReset = false;
	this->bFlush = false;
}

void AudioPlayer::onAttach()
{

}

void AudioPlayer::perform()
{
	if (this->bReset)
	{
		this->bReset = false;
		SDL_ClearAudioStream(this->pStream);
	}

	if (this->bFlush)
	{
		this->bFlush = false;
		SDL_FlushAudioStream(this->pStream);
	}
}

void AudioPlayer::play()
{
	if (this->pClip == NULL)
	{
		std::cerr << "[Audio Player] ERROR: No Audio Clip assigned." << std::endl;
		return;
	}
	SDL_PutAudioStreamData(this->pStream, this->pClip->getBuffer(), this->pClip->getLength());
	SDL_ResumeAudioStreamDevice(this->pStream);
}

void AudioPlayer::audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nCurrent, int nTotal)
{
	if (nCurrent == nTotal) ((AudioPlayer*)pData)->bReset = true;
}
