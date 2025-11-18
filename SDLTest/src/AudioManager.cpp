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

void AudioManager::play(std::string strName, std::string streamKey)
{
    if (!this->mapAudioClip.contains(strName)) return;

	AudioData* pAudioData = new AudioData();
    pAudioData->strClipName = strName;
	pAudioData->fProgress = 0.0f;
	pAudioData->bCleanUp = false;

    SDL_AudioStream* pAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &this->mSpec, audioStreamCallback, pAudioData);
	pAudioData->pStream = pAudioStream;
	if (!streamKey.empty()) this->mapPlaying[streamKey] = pAudioData;

    AudioClip* pClip = this->mapAudioClip[strName];
    SDL_PutAudioStreamData(pAudioStream, pClip->buffer, pClip->length);
    SDL_ResumeAudioStreamDevice(pAudioStream);
}

void AudioManager::stop(std::string streamKey)
{
	this->mapPlaying[streamKey]->bCleanUp = true;
}

void AudioManager::cleanUp()
{
    for (int i = this->vecToDestroy.size() - 1; i >= 0; i--)
    {
		SDL_DestroyAudioStream(this->vecToDestroy[i]->pStream);
		delete this->vecToDestroy[i];
    }
    this->vecToDestroy.clear();
}

void AudioManager::audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal)
{
	AudioData* pAudioData = static_cast<AudioData*>(pData);
	pAudioData->fProgress = (float)nExtra / (float)nTotal;
    if (pAudioData->fProgress == 1.0f && !pAudioData->bCleanUp)
    {
		pAudioData->bCleanUp = true;
        P_SHARED_INSTANCE->vecToDestroy.push_back(pAudioData);
		std::cout << "Stream playing \"" << pAudioData->strClipName << "\" finished." << std::endl;
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
