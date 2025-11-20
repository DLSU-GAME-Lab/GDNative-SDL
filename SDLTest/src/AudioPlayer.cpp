#include "AudioPlayer.h"
#include "AudioManager.h"
#include <iostream>

AudioPlayer::AudioPlayer(std::string strClipName, AudioGroupTag ETag)
	: AudioPlayer(strClipName, "", ETag, OnAudioFinished::STOP)
{
}

AudioPlayer::AudioPlayer(std::string strClipName, float fVolume)
	: AudioPlayer(strClipName, "", AudioGroupTag::NONE, OnAudioFinished::STOP, fVolume)
{
}

AudioPlayer::AudioPlayer(std::string strClipName, std::string strKey, AudioGroupTag ETag, OnAudioFinished EOnFinished, float fVolume)
{
	this->strKey = strKey;
	this->pClip = AudioManager::getInstance()->getAudioClip(strClipName);
	this->ETag = ETag;
	this->EOnFinished = EOnFinished;
	this->fProgress = 0.0f;
	this->bFinished = false;
	this->bCleanUp = false;

	SDL_AudioSpec spec = {};
	spec.freq = 44100;
	spec.format = SDL_AUDIO_F32;
	spec.channels = 2;
	this->pStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, audioStreamCallback, this);
	SDL_PutAudioStreamData(this->pStream, this->pClip->getBuffer(), this->pClip->getLength());

	if (this->ETag != AudioGroupTag::NONE)
		SDL_SetAudioStreamGain(this->pStream, fVolume);
	else SDL_SetAudioStreamGain(this->pStream, AudioManager::getInstance()->getVolume(this->ETag));
	SDL_ResumeAudioStreamDevice(this->pStream);
}

void AudioPlayer::onFinished()
{
	this->bCleanUp = true;
	// TODO: Add custom functionality here
}

void AudioPlayer::audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal)
{
	AudioPlayer* pPlayer = static_cast<AudioPlayer*>(pData);
	pPlayer->fProgress = (float)nExtra / (float)nTotal;
	if (pPlayer->fProgress == 1.0f && !pPlayer->bFinished)
	{
		pPlayer->bFinished = true;
		switch (pPlayer->EOnFinished)
		{
		case OnAudioFinished::STOP:
			pPlayer->bCleanUp = true;
			break;
		case OnAudioFinished::PAUSE:
			SDL_PauseAudioStreamDevice(pPlayer->pStream);
			break;
		case OnAudioFinished::LOOP:
			pPlayer->bFinished = false;
			SDL_PutAudioStreamData(pPlayer->pStream, pPlayer->pClip->getBuffer(), pPlayer->pClip->getLength());
			break;
		case OnAudioFinished::FUNC:
			pPlayer->onFinished();
			break;
		default:
			break;
		}
		std::cout << "Audio Player playing \"" << pPlayer->pClip->getName() << "\" has finished." << std::endl;
	}
}
