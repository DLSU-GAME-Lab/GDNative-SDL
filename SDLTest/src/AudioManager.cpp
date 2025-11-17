#include "AudioManager.h"
#include <iostream>
#include <mutex>

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

void AudioManager::play(std::string strName)
{
    if (!this->mapAudioClip.contains(strName)) return;
    SDL_AudioStream* pAudioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &this->mSpec, audioStreamCallback, NULL);
    AudioClip* pClip = this->mapAudioClip[strName];
    SDL_PutAudioStreamData(pAudioStream, pClip->buffer, pClip->length);
    SDL_ResumeAudioStreamDevice(pAudioStream);
}

void AudioManager::cleanUp()
{
    SDL_LockMutex(this->pMutex);
    for (int i = 0; i < this->vecFinishedStream.size(); i++)
    {
		SDL_DestroyAudioStream(this->vecFinishedStream[i]);
    }
    this->vecFinishedStream.clear();
    SDL_UnlockMutex(this->pMutex);
}

void AudioManager::audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal)
{
    //std::cout << "Stream: " << pStream << " Playing: " << nExtra << " Total: " << nTotal << std::endl;
    if (nExtra == nTotal)
    {
        SDL_LockMutex(P_SHARED_INSTANCE->pMutex);
        P_SHARED_INSTANCE->vecFinishedStream.push_back(pStream);
        SDL_UnlockMutex(P_SHARED_INSTANCE->pMutex);
		//std::cout << "destroying stream." << std::endl;
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
	this->pMutex = SDL_CreateMutex();

	//MIX_Mixer* pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &this->mSpec);
 //   if (pMixer == NULL)
 //   {
 //       std::cerr << "[ERROR] : Failed to create Audio Mixer! "
 //           << "Error: " << SDL_GetError() << std::endl;
	//}
	//else SDL_free(pMixer);
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
