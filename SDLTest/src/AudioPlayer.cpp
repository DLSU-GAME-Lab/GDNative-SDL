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
	this->pStream = NULL;
}

void AudioPlayer::addListener(IAudioPlayerListener* pListener)
{
	this->vecListener.push_back(pListener);
}

void AudioPlayer::removeListener(IAudioPlayerListener* pListener)
{
	this->vecListener.erase(std::remove(this->vecListener.begin(), this->vecListener.end(), pListener), this->vecListener.end());
}

void AudioPlayer::onFinished()
{
	for (auto listener : this->vecListener)
	{
		listener->onAudioFinished();
	}
}
