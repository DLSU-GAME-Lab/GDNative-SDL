#include "AudioManager.h"
#include <iostream>

void AudioManager::load(std::string strPath, std::string strName)
{
    std::string path = "Assets/" + strPath;
    Uint8* audioBuffer = 0;
    Uint32 audioLength = 0;
	std::cout << "[DEBUG] Loading Audio Clip from: " << path << std::endl;
    if (SDL_LoadWAV(path.c_str(), &this->mSpec, &audioBuffer, &audioLength) != NULL)
    {
        AudioClip* pAudioClip = new AudioClip(strName, audioBuffer, audioLength);
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

void AudioManager::play(AudioPlayer* pPlayer)
{
	this->vecPlaying.push_back(pPlayer);
    if (!pPlayer->strKey.empty()) this->mapPlaying[pPlayer->strKey] = pPlayer;
    SDL_AudioStream* pStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &mSpec, audioStreamCallback, pPlayer);
	pPlayer->pStream = pStream;

    if (!SDL_PutAudioStreamData(pPlayer->pStream, pPlayer->pClip->getBuffer(), pPlayer->pClip->getLength()))
    {
        std::cout << SDL_GetError() << std::endl;
    }

    if (pPlayer->ETag != AudioGroupTag::NONE)
        SDL_SetAudioStreamGain(pPlayer->pStream, AudioManager::getInstance()->getVolume(pPlayer->ETag));

    if (SDL_ResumeAudioStreamDevice(pPlayer->pStream))
	    std::cout << "[Audio Manager] LOG: Playing audio clip \"" << pPlayer->pClip->getName() << "\"" << std::endl;
}

void AudioManager::stop(std::string strKey)
{
    if (!this->mapPlaying.contains(strKey)) return;
    this->mapPlaying[strKey]->bCleanUp = true;
    this->vecToDestroy.push_back(this->mapPlaying[strKey]);
    std::cout << "[Audio Manager] LOG: Stopping player \"" << strKey << "\" playing the clip \"" << this->mapPlaying[strKey]->pClip->getName() << "\"" << std::endl;
}

void AudioManager::stopAll()
{
    std::cout << "[Audio Manager] LOG: Stopping all audio streams" << std::endl;
    for (int i = this->vecPlaying.size() - 1; i >= 0; i--)
    {
        this->vecPlaying[i]->bCleanUp = true;
        SDL_DestroyAudioStream(this->vecPlaying[i]->pStream);
        delete this->vecPlaying[i];
	}
    this->vecPlaying.clear();
    this->mapPlaying.clear();
}

void AudioManager::stopByData(AudioPlayer* pPlayer)
{
	this->vecPlaying.erase(std::remove(this->vecPlaying.begin(), this->vecPlaying.end(), pPlayer), this->vecPlaying.end());
}

void AudioManager::audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal)
{
    AudioPlayer* pPlayer = static_cast<AudioPlayer*>(pData);
    pPlayer->fProgress = (float)nExtra / (float)nTotal;
    if (pPlayer->bCleanUp) return;

    if (pPlayer->fProgress == 1.0f && !pPlayer->bFinished)
    {
        pPlayer->bFinished = true;
    }
}

void AudioManager::update()
{
    for (auto pPlayer : this->vecPlaying)
    {
		if (!pPlayer->bFinished) continue;

        switch (pPlayer->EOnFinished)
        {
        case OnAudioFinished::STOP:
			pPlayer->bCleanUp = true;
            this->vecToDestroy.push_back(pPlayer);
            break;
        case OnAudioFinished::PAUSE:
            SDL_PauseAudioStreamDevice(pPlayer->pStream);
            break;
        case OnAudioFinished::LOOP:
            pPlayer->bFinished = false;
            pPlayer->fProgress = 0.0f;
            SDL_PutAudioStreamData(pPlayer->pStream, pPlayer->pClip->getBuffer(), pPlayer->pClip->getLength());
            //SDL_ResumeAudioStreamDevice(pPlayer->pStream);
            break;
        case OnAudioFinished::FUNC:
            pPlayer->onFinished();
            break;
        default:
            break;
        }
	}

	cleanUp();
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

void AudioManager::setVolume(std::string strKey, float fVolume)
{
    if (this->mapPlaying.contains(strKey))
        SDL_SetAudioStreamGain(this->mapPlaying[strKey]->pStream, fVolume);
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

float AudioManager::getVolume(std::string strKey)
{
    if (this->mapPlaying.contains(strKey))
        return SDL_GetAudioStreamGain(this->mapPlaying[strKey]->pStream);
    else return -1.0f;
}

float AudioManager::getVolume(AudioGroupTag ETag)
{
    return this->mapGroupVolumes[ETag];
}

bool AudioManager::isPlaying(std::string strKey)
{
    return this->mapPlaying.contains(strKey);
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
        this->mapGroupVolumes[static_cast<AudioGroupTag>(i)] = 0.5f;
	}
}

AudioManager::~AudioManager()
{
    
}

void AudioManager::initialize()
{
    P_SHARED_INSTANCE = new AudioManager();
}

void AudioManager::destroy()
{
    P_SHARED_INSTANCE->stopAll();
    delete P_SHARED_INSTANCE;
}

AudioManager* AudioManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
