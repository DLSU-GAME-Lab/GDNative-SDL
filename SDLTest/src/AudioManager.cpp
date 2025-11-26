#include "AudioManager.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <cstdio>

void AudioManager::load(std::string strPath, std::string strName)
{
    // strPath should be relative to assets root (build.gradle(app). Do NOT prefix with "Assets/" (Case Sensitive).
    // Example usage: load("Audio/myclip.wav", "myclip")
    const std::string assetPath = strPath;

    // Diagnostic logs
    spdlog::info("[AudioManager] Attempting to load asset: '{}', name '{}'", assetPath, strName);

    const char* base = SDL_GetBasePath();     // native app base (may be useful)
    const char* pref = SDL_GetPrefPath("org.main", "babaylan_tales"); // user pref path
    spdlog::info("[AudioManager] SDL_GetBasePath(): {}", base ? base : "<null>");
    spdlog::info("[AudioManager] SDL_GetPrefPath(): {}", pref ? pref : "<null>");

    // Quick check: try fopen() (works only if asset is an actual file on filesystem)
    FILE* f = fopen(assetPath.c_str(), "rb");
    if (f) {
        fclose(f);
        spdlog::info("[AudioManager] fopen succeeded for '{}', trying SDL_LoadWAV", assetPath);

        Uint8* audioBuffer = nullptr;
        Uint32 audioLength = 0;

        // SDL3: SDL_LoadWAV returns bool (true on success)
        bool ok = SDL_LoadWAV(assetPath.c_str(), &this->mSpec, &audioBuffer, &audioLength);
        if (ok) {
            AudioClip* pAudioClip = new AudioClip(strName, audioBuffer, audioLength);
            this->vecAudioClip.push_back(pAudioClip);
            this->mapAudioClip[strName] = pAudioClip;
            spdlog::info("[AudioManager] Loaded audio asset '{}' as '{}' ({} bytes)", assetPath, strName, audioLength);
            return;
        } else {
            spdlog::error("[AudioManager] SDL_LoadWAV failed for '{}': {}", assetPath, SDL_GetError());
            return;
        }
    } else {
        spdlog::warn("[AudioManager] fopen failed for '{}'. Asset probably inside APK (not a regular file): {}",
                     assetPath, strerror(errno));
    }

    // FALLBACK: asset inside APK — must use RWops / LoadWAV_RW or Android AssetManager.
    spdlog::info("[AudioManager] Asset '{}' not accessible via fopen — use SDL_LoadWAV_RW with an SDL_RWops created from the APK asset.", assetPath);
    
    /*
    Uint8* audioBuffer = 0;
    Uint32 audioLength = 0;
	std::cout << "[DEBUG] Loading Audio Clip from: " << assetPath << std::endl;
    if (SDL_LoadWAV(assetPath.c_str(), &this->mSpec, &audioBuffer, &audioLength) != NULL)
    {
        AudioClip* pAudioClip = new AudioClip(strName, audioBuffer, audioLength);
        this->vecAudioClip.push_back(pAudioClip);
        this->mapAudioClip[strName] = pAudioClip;
    }
    else
    {
        std::cerr << "[ERROR] : Failed to create Audio Clip for [" << assetPath << "] "
            << "Error: " << SDL_GetError() << std::endl;
    }
     */
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
    if (!pPlayer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] play called with null player");
        return;
    }

    // Ensure the player actually has a clip
    if (!pPlayer->pClip) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] play called but player has no clip (null).");
        return;
    }

    // Ensure clip buffer exists
    Uint8* buf = pPlayer->pClip->getBuffer();
    Uint32 len = pPlayer->pClip->getLength();
    if (!buf || len == 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] clip '%s' has empty buffer or zero length", pPlayer->pClip->getName().c_str());
        return;
    }

    // Add to playing lists
    this->vecPlaying.push_back(pPlayer);
    if (!pPlayer->strKey.empty()) this->mapPlaying[pPlayer->strKey] = pPlayer;

    // Open stream
    SDL_AudioStream* pStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &mSpec, audioStreamCallback, pPlayer);
    if (!pStream) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
        // remove from vecPlaying/mapPlaying since we couldn't start playback
        this->vecPlaying.erase(std::remove(this->vecPlaying.begin(), this->vecPlaying.end(), pPlayer), this->vecPlaying.end());
        if (!pPlayer->strKey.empty()) this->mapPlaying.erase(pPlayer->strKey);
        return;
    }
    pPlayer->pStream = pStream;

    // Queue data
    int putResult = SDL_PutAudioStreamData(pPlayer->pStream, buf, len);
    if (putResult != 0) {
        // SDL_PutAudioStreamData return conventions may differ by SDL version; log robustly.
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] SDL_PutAudioStreamData returned %d; SDL_Error: %s", putResult, SDL_GetError());
    }

    // set gain if needed (guard pStream)
    if (pPlayer->ETag != AudioGroupTag::NONE && pPlayer->pStream) {
        SDL_SetAudioStreamGain(pPlayer->pStream, AudioManager::getInstance()->getVolume(pPlayer->ETag));
    }

    // resume stream (check return)
    if (SDL_ResumeAudioStreamDevice(pPlayer->pStream) != 0) {
        SDL_Log("[Audio Manager] LOG: Playing audio clip \"%s\"", pPlayer->pClip->getName().c_str());
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] SDL_ResumeAudioStreamDevice returned error: %s", SDL_GetError());
    }
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
    SDL_Log("[Audio Manager] LOG: Stopping all audio streams");
    for (int i = (int)this->vecPlaying.size() - 1; i >= 0; --i)
    {
        AudioPlayer* p = this->vecPlaying[i];
        if (!p) continue;

        p->bCleanUp = true;
        if (p->pStream) {
            SDL_DestroyAudioStream(p->pStream);
            p->pStream = nullptr;
        }
        delete p;
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
    for (int i = (int)this->vecToDestroy.size() - 1; i >= 0; --i)
    {
        AudioPlayer* p = this->vecToDestroy[i];
        if (!p) continue;

        if (!p->strKey.empty() && this->mapPlaying.contains(p->strKey))
            this->mapPlaying.erase(p->strKey);

        stopByData(p); // remove from vecPlaying

        if (p->pStream) {
            SDL_DestroyAudioStream(p->pStream);
            p->pStream = nullptr;
        } else {
            SDL_Log("[AudioManager] cleanUp: player had null pStream (clip=%s)", p->pClip ? p->pClip->getName().c_str() : "<no-clip>");
        }

        delete p;
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
