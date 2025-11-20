#include "AudioManager.h"
#include <iostream>

void AudioManager::load(std::string strPath, std::string strName)
{
    std::string path = "Assets/" + strPath;
    Uint8* audioBuffer = 0;
    Uint32 audioLength = 0;
    if (SDL_LoadWAV(path.c_str(), &this->mSpec, &audioBuffer, &audioLength) != NULL)
    {
        AudioClip* pAudioClip = new AudioClip(audioBuffer, audioLength);
        this->vecAudioClip.push_back(pAudioClip);
        this->mapAudioClip[strName] = pAudioClip;
    }
    else
    {
        std::cerr << "[ERROR] : Failed to create Audio Clip for [" << path << "] "
            << "Error: " << SDL_GetError() << std::endl;
    }
}

void AudioManager::unload(std::string strName)
{
    int nIndex = -1;
    for (int i = 0; i < this->vecAudioClip.size() && nIndex == -1; i++)
    {
        if (this->vecAudioClip[i] == this->mapAudioClip[strName])
            nIndex = i;
    }

    if (nIndex != -1)
    {
        this->vecAudioClip.erase(this->vecAudioClip.begin() + nIndex);
        delete this->mapAudioClip[strName];
        this->mapAudioClip.erase(strName);
    }
}

AudioClip* AudioManager::getAudioClip(std::string strName)
{
    if (this->mapAudioClip.contains(strName))
        return this->mapAudioClip[strName];

	return NULL;
}

void AudioManager::play(std::string strName, std::string streamKey, float fVolume)
{
	play(strName, AudioGroupTag::NONE, streamKey, fVolume);
}

void AudioManager::play(std::string strName, AudioGroupTag ETag, std::string streamKey, float fVolume)
{
    if (!this->mapAudioClip.contains(strName)) return;

	AudioData* pPlayer = new AudioData();
	this->vecPlaying.push_back(pPlayer);
    this->mapPlaying[streamKey] = pPlayer;
    pPlayer->strClipName = strName;
    pPlayer->strKey = streamKey;
	pPlayer->fProgress = 0.0f;
	pPlayer->bCleanUp = false;
	pPlayer->ETag = ETag;

    SDL_AudioStream* pAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &this->mSpec, audioStreamCallback, pPlayer);
	pPlayer->pStream = pAudioStream;

    AudioClip* pClip = this->mapAudioClip[strName];
    SDL_PutAudioStreamData(pAudioStream, pClip->buffer, pClip->length);

    if (pPlayer->ETag != AudioGroupTag::NONE)
	    SDL_SetAudioStreamGain(pAudioStream, fVolume);
    else SDL_SetAudioStreamGain(pAudioStream, this->mapGroupVolumes[ETag]);
    SDL_ResumeAudioStreamDevice(pAudioStream);
}

void AudioManager::stop(std::string streamKey)
{
    if (!this->mapPlaying.contains(streamKey)) return;
	this->mapPlaying[streamKey]->bCleanUp = true;
	this->vecToDestroy.push_back(this->mapPlaying[streamKey]);
	SDL_ClearAudioStream(this->mapPlaying[streamKey]->pStream);
    std::cout << "[Audio Manager] LOG: Stopping stream \"" << streamKey << "\" playing the clip \"" << this->mapPlaying[streamKey]->strClipName << "\"" << std::endl;
}

void AudioManager::stopAll()
{
    std::cout << "[Audio Manager] LOG: Stopping all audio streams" << std::endl;
    for (auto pPlayer : this->vecPlaying)
    {
        pPlayer->bCleanUp = true;
        this->vecToDestroy.push_back(pPlayer);
        SDL_ClearAudioStream(pPlayer->pStream);
	}
    this->vecPlaying.clear();
    this->mapPlaying.clear();
}

void AudioManager::stopByData(AudioData* pData)
{
    int nIndex = -1;
    for (int i = 0; i < this->vecPlaying.size() && nIndex == -1; i++)
    {
        if (this->vecPlaying[i] == pData)
            nIndex = i;
    }

    if (nIndex != -1)
    {
        this->vecPlaying.erase(this->vecPlaying.begin() + nIndex);
    }
}

void AudioManager::cleanUp()
{
    for (int i = this->vecToDestroy.size() - 1; i >= 0; i--)
    {
        if (this->mapPlaying.contains(this->vecToDestroy[i]->strKey))
            this->mapPlaying.erase(this->vecToDestroy[i]->strKey);
		stopByData(this->vecToDestroy[i]);
		SDL_DestroyAudioStream(this->vecToDestroy[i]->pStream);
		delete this->vecToDestroy[i];
    }
    this->vecToDestroy.clear();
}

void AudioManager::setVolume(std::string streamKey, float fVolume)
{
    if (this->mapPlaying.contains(streamKey))
        SDL_SetAudioStreamGain(this->mapPlaying[streamKey]->pStream, fVolume);
}

void AudioManager::setVolume(AudioGroupTag ETag, float fVolume)
{
	this->mapGroupVolumes[ETag] = fVolume;
    for (auto pPlayer : this->vecPlaying)
    {
        if (pPlayer->ETag == ETag)
        {
            SDL_SetAudioStreamGain(pPlayer->pStream, fVolume);
        }
	}
}

float AudioManager::getVolume(std::string streamKey)
{
    if (this->mapPlaying.contains(streamKey))
        return SDL_GetAudioStreamGain(this->mapPlaying[streamKey]->pStream);
    else return -1.0f;
}

float AudioManager::getVolume(AudioGroupTag ETag)
{
    return this->mapGroupVolumes[ETag];
}

void AudioManager::audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal)
{
	AudioData* pPlayer = static_cast<AudioData*>(pData);
	pPlayer->fProgress = (float)nExtra / (float)nTotal;
    if (pPlayer->fProgress == 1.0f && !pPlayer->bCleanUp)
    {
		pPlayer->bCleanUp = true;
		P_SHARED_INSTANCE->vecToDestroy.push_back(pPlayer);
		std::cout << "Stream playing \"" << pPlayer->strClipName << "\" finished." << std::endl;
    }
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
AudioManager* AudioManager::P_SHARED_INSTANCE = NULL;

AudioManager::AudioManager()
{
    this->mSpec.freq = 44100; // Sample rate
    this->mSpec.format = SDL_AUDIO_F32; // Audio format
    this->mSpec.channels = 2; // Stereo

    for (int i = static_cast<int>(AudioGroupTag::MUSIC); i <= static_cast<int>(AudioGroupTag::MASTER); i++)
    {
        this->mapGroupVolumes[static_cast<AudioGroupTag>(i)] = 1.0f;
	}
}

AudioManager::~AudioManager()
{
    for (int i = this->vecAudioClip.size() - 1; i >= 0; i--)
    {
        delete this->vecAudioClip[i];
    }
}

void AudioManager::initialize()
{
    P_SHARED_INSTANCE = new AudioManager();
}

void AudioManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

AudioManager* AudioManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
