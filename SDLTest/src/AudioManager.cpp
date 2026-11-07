#include "AudioManager.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <fstream>
#include "ManifestLoader.h"

#if defined(__ANDROID__)
#include <spdlog/spdlog.h>
#endif

// clips shorter than this are decoded once at load time (cheap to mix, many at once),
// longer ones (music) are streamed from the file so they don't sit decoded in memory
static const Sint64 PREDECODE_MAX_MS = 8000;

// Helper function to log SDL errors
void logSDLError(const char* context)
{
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s: %s", context, SDL_GetError());
}

void AudioManager::load(std::string strPath, std::string strName)
{
    if (!this->pMixer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioManager] no mixer, can't load %s", strPath.c_str());
        return;
    }

    if (this->mapAudioClip.contains(strName))
    {
        SDL_Log("[AudioManager] '%s' is already loaded", strName.c_str());
        return;
    }

#if defined(__ANDROID__)
    const std::string assetPath = strPath;
#else
    const std::string assetPath = "Assets/" + strPath;
#endif
    SDL_Log("[AudioManager] Loading audio asset: %s", assetPath.c_str());

    // SDL_mixer opens the file itself (reads APK assets on Android too) and
    // decodes whatever format it supports (wav, ogg, ...).
    MIX_Audio* pAudio = MIX_LoadAudio(this->pMixer, assetPath.c_str(), false);
    if (!pAudio)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[AudioManager] MIX_LoadAudio failed for %s: %s",
            assetPath.c_str(), SDL_GetError());
        return;
    }

    // Short clips are loaded again fully decoded.
    Sint64 nFrames = MIX_GetAudioDuration(pAudio);
    if (nFrames > 0 && MIX_AudioFramesToMS(pAudio, nFrames) <= PREDECODE_MAX_MS)
    {
        MIX_Audio* pDecoded = MIX_LoadAudio(this->pMixer, assetPath.c_str(), true);
        if (pDecoded)
        {
            MIX_DestroyAudio(pAudio);
            pAudio = pDecoded;
        }
    }

    AudioClip* pAudioClip = new AudioClip(strName, pAudio);
    this->vecAudioClip.push_back(pAudioClip);
    this->mapAudioClip[strName] = pAudioClip;
    SDL_Log("[AudioManager] Loaded audio '%s' (%lld ms) as key '%s'",
        assetPath.c_str(), (long long)pAudioClip->getDurationMS(), strName.c_str());
}

void AudioManager::unload(std::string strName)
{
    if (!this->mapAudioClip.contains(strName))
        return;

    AudioClip* pClip = this->mapAudioClip[strName];

    // Players still using the clip are stopped first, so none is left with a dangling pointer.
    std::vector<AudioPlayer*> vecUsing;
    for (AudioPlayer* pPlayer : this->vecPlaying)
    {
        if (pPlayer && pPlayer->pClip == pClip)
            vecUsing.push_back(pPlayer);
    }
    for (AudioPlayer* pPlayer : vecUsing)
    {
        pPlayer->bCleanUp = true;
        pPlayer->stop();
        this->vecToDestroy.push_back(pPlayer);
    }
    this->cleanUp();

    this->vecAudioClip.erase(
        std::remove(this->vecAudioClip.begin(), this->vecAudioClip.end(), pClip),
        this->vecAudioClip.end());
    this->mapAudioClip.erase(strName);
    delete pClip;
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

    // a player that is played again must not be listed twice
    if (std::find(this->vecPlaying.begin(), this->vecPlaying.end(), pPlayer) == this->vecPlaying.end())
        this->vecPlaying.push_back(pPlayer);
    if (!pPlayer->strKey.empty())
        this->mapPlaying[pPlayer->strKey] = pPlayer;
}

void AudioManager::stop(std::string strKey)
{
    if (!this->mapPlaying.contains(strKey))
        return;

    AudioPlayer* player = this->mapPlaying[strKey];
    player->stop();

    this->vecPlaying.erase(std::remove(this->vecPlaying.begin(),
        this->vecPlaying.end(), player),
        this->vecPlaying.end());

    this->vecToDestroy.push_back(player);

    SDL_Log("[Audio Manager] LOG: Stopping player \"%s\" playing the clip \"%s\"",
        strKey.c_str(), player->pClip->getName().c_str());
}

void AudioManager::stopAll()
{
    SDL_Log("[Audio Manager] LOG: Stopping all audio streams");

    // Stop all active players and queue them for destruction.
    for (auto& player : this->vecPlaying)
    {
        if (player)
        {
            player->bCleanUp = true;
            player->stop();
            this->vecToDestroy.push_back(player);
        }
    }
    // Clear the live lists before cleanUp() so the erase-remove inside it is a
    // no-op, avoids an O(n^2) scan over an already-conceptually-empty list.
    this->vecPlaying.clear();
    this->mapPlaying.clear();

    this->cleanUp();  // deletes all queued players and clears vecToDestroy

    SDL_Log("[Audio Manager] LOG: All audio streams stopped");
}

void AudioManager::update()
{
    // Iterate a snapshot so that cleanUp() can safely mutate vecPlaying below.
    std::vector<AudioPlayer*> snapshot = this->vecPlaying;
    for (auto pPlayer : snapshot)
    {
        if (!pPlayer) continue;

        if (pPlayer->bCleanUp)
            this->vecToDestroy.push_back(pPlayer);
        else
            pPlayer->updateProgress();
    }

    cleanUp();
}

void AudioManager::cleanUp()
{
    // A player can be queued more than once (stopped, then finished), delete each only once.
    std::unordered_set<AudioPlayer*> setDeleted;

    for (int i = (int)this->vecToDestroy.size() - 1; i >= 0; --i)
    {
        AudioPlayer* p = this->vecToDestroy[i];
        if (!p || !setDeleted.insert(p).second) continue;

        // Remove from vecPlaying so no dangling pointer remains after delete.
        this->vecPlaying.erase(
            std::remove(this->vecPlaying.begin(), this->vecPlaying.end(), p),
            this->vecPlaying.end());

        // Only drop the key if it still points at this player (a newer player may own it).
        if (!p->strKey.empty() && this->mapPlaying.contains(p->strKey) && this->mapPlaying[p->strKey] == p)
            this->mapPlaying.erase(p->strKey);

        delete p;
    }
    this->vecToDestroy.clear();
}

void AudioManager::setVolume(std::string strKey, float fVolume)
{
    if (!this->mapPlaying.contains(strKey))
        return;

    this->mapPlaying[strKey]->setVolume(fVolume);
}

void AudioManager::setVolume(AudioGroupTag ETag, float fVolume)
{
    this->mapGroupVolumes[ETag] = fVolume;
    this->setGroupApplied.insert(ETag);

    // running players pick the new group volume up right away
    for (AudioPlayer* pPlayer : this->vecPlaying)
    {
        if (pPlayer) pPlayer->applyGain();
    }

    SDL_Log("[AudioManager] setVolume for tag %d to %.2f", (int)ETag, fVolume);
}

float AudioManager::getVolume(std::string strKey)
{
    if (this->mapPlaying.contains(strKey))
        return this->mapPlaying[strKey]->getVolume();
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
    return this->mapPlaying.contains(strKey) && !this->mapPlaying[strKey]->isFinished();
}

MIX_Mixer* AudioManager::getMixer() const
{
    return this->pMixer;
}

float AudioManager::getGroupGain(AudioGroupTag ETag) const
{
    float fGain = 1.0f;

    if (ETag != AudioGroupTag::NONE && ETag != AudioGroupTag::MASTER && this->setGroupApplied.contains(ETag))
        fGain *= this->mapGroupVolumes.at(ETag);

    if (this->setGroupApplied.contains(AudioGroupTag::MASTER))
        fGain *= this->mapGroupVolumes.at(AudioGroupTag::MASTER);

    return fGain;
}

/* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
AudioManager* AudioManager::P_SHARED_INSTANCE = NULL;

AudioManager::AudioManager()
{
    this->pMixer = nullptr;

    // Initialize volume levels
    for (int i = static_cast<int>(AudioGroupTag::MUSIC);
        i <= static_cast<int>(AudioGroupTag::MASTER); i++)
    {
        this->mapGroupVolumes[static_cast<AudioGroupTag>(i)] = 0.5f;
    }

#if defined(__ANDROID__)
    // a bigger device buffer keeps the output from underrunning (crackling) on phones
    SDL_SetHint(SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES, "1024");
#endif

    if (!MIX_Init())
    {
        logSDLError("[AudioManager] MIX_Init failed");
        return;
    }

    // One mixer on the default device for the whole game, in the device's own
    // format (NULL spec), so nothing is converted twice on the way out.
    this->pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!this->pMixer)
    {
        logSDLError("[AudioManager] MIX_CreateMixerDevice failed");
        MIX_Quit();
        return;
    }

    SDL_Log("[AudioManager] Initialized (SDL_mixer)");
}

AudioManager::~AudioManager()
{
    stopAll();

    // Clean up all audio clips
    for (auto& clip : this->vecAudioClip)
    {
        if (clip)
        {
            delete clip;
        }
    }
    this->vecAudioClip.clear();
    this->mapAudioClip.clear();

    if (this->pMixer)
    {
        MIX_DestroyMixer(this->pMixer);
        this->pMixer = nullptr;
        MIX_Quit();
    }
}

void AudioManager::initialize()
{
    if (!P_SHARED_INSTANCE)
    {
        P_SHARED_INSTANCE = new AudioManager();
    }
}

void AudioManager::destroy()
{
    if (P_SHARED_INSTANCE)
    {
        P_SHARED_INSTANCE->stopAll();
        delete P_SHARED_INSTANCE;
        P_SHARED_INSTANCE = NULL;
    }
}

AudioManager* AudioManager::getInstance()
{
    if (!P_SHARED_INSTANCE)
    {
        initialize();
    }
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */
