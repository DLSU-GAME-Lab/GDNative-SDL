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
	this->pStream = createAudioStream(this->pClip);
}

AudioPlayer::~AudioPlayer()
{
	if (this->pStream) SDL_DestroyAudioStream(this->pStream);
}

SDL_AudioStream* AudioPlayer::createAudioStream(AudioClip* pClip)
{
    if (pClip == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] ERROR: AudioClip is null for player with key '%s'", this->strKey.c_str());
        return nullptr;
	}

    Uint8* buf = pClip->getBuffer();
    Uint32 len = pClip->getLength();
	SDL_AudioSpec spec = pClip->getSpec();
    if (!buf || len == 0) return nullptr;

    // Open the default audio device
    SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

    if (deviceID == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioManager] SDL_OpenAudioDevice failed: %s", SDL_GetError());
        return nullptr;
    }

    // Create a stream that converts from the clip's loaded format
    // to whatever the device actually wants
    SDL_AudioSpec deviceSpec;
    SDL_GetAudioDeviceFormat(deviceID, &deviceSpec, NULL);

    SDL_AudioStream* pStream = SDL_CreateAudioStream(&spec, &deviceSpec);
    if (!pStream)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioManager] SDL_CreateAudioStream failed: %s", SDL_GetError());
        SDL_CloseAudioDevice(deviceID);
        return nullptr;
    }

    // Bind the stream to the device
    SDL_BindAudioStream(deviceID, pStream);

	return pStream;
}

void AudioPlayer::play()
{
    if (this->pStream == nullptr || this->pClip == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] Cannot play audio for player with key '%s': stream or clip is null", this->strKey.c_str());
        return;
	}
    // Push the audio data into the stream
    SDL_PutAudioStreamData(this->pStream, this->pClip->getBuffer(), this->pClip->getLength());
    SDL_FlushAudioStream(this->pStream);
}

void AudioPlayer::stop()
{
    if (this->pStream == nullptr || this->pClip == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] Cannot stop audio for player with key '%s': stream or clip is null", this->strKey.c_str());
        return;
    }
    this->bFinished = true;
    onFinished();
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
	for (auto listener : this->vecListener) if (listener != nullptr) listener->onAudioFinished();
}
