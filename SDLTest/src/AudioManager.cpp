#include "AudioManager.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include "ManifestLoader.h"

#if defined(__ANDROID__)
#include <spdlog/spdlog.h>
#endif

// Helper function to log SDL errors
void logSDLError(const char* context) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s: %s", context, SDL_GetError());
}

void AudioManager::load(std::string strPath, std::string strName)
{
    const std::string assetPath = strPath;
    SDL_Log("[AudioManager] Loading audio asset: %s", assetPath.c_str());

    // Try to load raw bytes (works for APK assets)
    size_t size = 0;
    void* data = SDL_LoadFile(assetPath.c_str(), &size);
    if (!data || size <= 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] SDL_LoadFile failed for %s: %s",
                     assetPath.c_str(), SDL_GetError());
        if (data) SDL_free(data);
        return;
    }

    // For SDL3, we use SDL_IOStream
    SDL_IOStream* io = SDL_IOFromConstMem(data, (int)size);
    if (!io) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "[AudioManager] Failed to create IOStream for %s", assetPath.c_str());
        SDL_free(data);
        return;
    }

    SDL_AudioSpec loadedSpec;
    Uint8* audioBuffer = nullptr;
    Uint32 audioLength = 0;

    // Use SDL3's SDL_LoadWAV_IO with true for closeio parameter
    if (SDL_LoadWAV_IO(io, 1, &loadedSpec, &audioBuffer, &audioLength)) {

        AudioClip* pAudioClip = new AudioClip(strName, audioBuffer, audioLength, loadedSpec);
        this->vecAudioClip.push_back(pAudioClip);
        this->mapAudioClip[strName] = pAudioClip;
        SDL_Log("[AudioManager] Loaded audio '%s' (%u bytes) as key '%s'",
                assetPath.c_str(), audioLength, strName.c_str());
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "[AudioManager] SDL_LoadWAV_IO failed for %s: %s",
                     assetPath.c_str(), SDL_GetError());
    }

    // Note: Don't free audioBuffer here - it's owned by AudioClip
    // SDL_LoadWAV_IO will free the io stream automatically when closeio is true
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
    if (!pPlayer || !pPlayer->pClip) return;

	pPlayer->play();

    this->vecPlaying.push_back(pPlayer);
    if (!pPlayer->strKey.empty())
        this->mapPlaying[pPlayer->strKey] = pPlayer;
}

void AudioManager::stop(std::string strKey)
{
    if (!this->mapPlaying.contains(strKey))
        return;

    AudioPlayer* player = this->mapPlaying[strKey];
    player->bCleanUp = true;
    this->vecToDestroy.push_back(player);

    SDL_Log("[Audio Manager] LOG: Stopping player \"%s\" playing the clip \"%s\"",
            strKey.c_str(), player->pClip->getName().c_str());
}

void AudioManager::stopAll()
{
    SDL_Log("[Audio Manager] LOG: Stopping all audio streams");

    // First, stop all playing audio
    for (auto& player : this->vecPlaying) {
        if (player) {
            player->bCleanUp = true;
        }
    }

    // Then clean up in update()
    this->update();

    // Clear remaining references
    this->vecPlaying.clear();
    this->mapPlaying.clear();
    this->vecToDestroy.clear();

    SDL_Log("[Audio Manager] LOG: All audio streams stopped");
}

void AudioManager::stopByData(AudioPlayer* pPlayer)
{
	pPlayer->stop();
    this->vecPlaying.erase(std::remove(this->vecPlaying.begin(),
                                       this->vecPlaying.end(), pPlayer),
                           this->vecPlaying.end());
}

void AudioManager::audioStreamCallback(void* pData, SDL_AudioStream* pStream,
                                       int nExtra, int nTotal)
{
    // This callback is currently not used since we're not actually playing audio
    // Keep it as a stub for now
    AudioPlayer* pPlayer = static_cast<AudioPlayer*>(pData);
    if (pPlayer) {
        pPlayer->fProgress = (float)nExtra / (float)nTotal;
    }
}

void AudioManager::update()
{
    // Since we're not actually playing audio, just clean up any players marked for deletion
    for (auto pPlayer : this->vecPlaying) {
        if (pPlayer && pPlayer->bCleanUp) {
            this->vecToDestroy.push_back(pPlayer);
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

        delete p;
    }
    this->vecToDestroy.clear();
}

void AudioManager::setVolume(std::string strKey, float fVolume)
{
    SDL_Log("[AudioManager] setVolume for key %s (audio disabled)", strKey.c_str());
}

void AudioManager::setVolume(AudioGroupTag ETag, float fVolume)
{
    this->mapGroupVolumes[ETag] = fVolume;
    SDL_Log("[AudioManager] setVolume for tag %d to %.2f (audio disabled)",
            (int)ETag, fVolume);
}

float AudioManager::getVolume(std::string strKey)
{
    // Return default volume since audio is disabled
    return 0.5f;
}

float AudioManager::getVolume(AudioGroupTag ETag)
{
    if (this->mapGroupVolumes.contains(ETag))
        return this->mapGroupVolumes[ETag];
    return 0.5f;
}

bool AudioManager::isPlaying(std::string strKey)
{
    // Since we're not actually playing, check if it's in our map
    return this->mapPlaying.contains(strKey);
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
AudioManager* AudioManager::P_SHARED_INSTANCE = NULL;

AudioManager::AudioManager()
{
    // Initialize volume levels
    for (int i = static_cast<int>(AudioGroupTag::MUSIC);
         i <= static_cast<int>(AudioGroupTag::MASTER); i++)
    {
        this->mapGroupVolumes[static_cast<AudioGroupTag>(i)] = 0.5f;
    }

    SDL_Log("[AudioManager] Initialized (audio playback disabled for debugging)");
}

AudioManager::~AudioManager()
{
    stopAll();

    // Clean up all audio clips
    for (auto& clip : this->vecAudioClip) {
        if (clip) {
            delete clip;
        }
    }
    this->vecAudioClip.clear();
    this->mapAudioClip.clear();
}

void AudioManager::initialize()
{
    if (!P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE = new AudioManager();
    }
}

void AudioManager::destroy()
{
    if (P_SHARED_INSTANCE) {
        P_SHARED_INSTANCE->stopAll();
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = NULL;
    }
}

AudioManager* AudioManager::getInstance()
{
    if (!P_SHARED_INSTANCE) {
        initialize();
    }
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */