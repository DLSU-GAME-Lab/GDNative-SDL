// ---------------------------------------------------------------------------
// AudioPlayer.cpp
// ---------------------------------------------------------------------------

#include "AudioPlayer.h"
#include "AudioManager.h"
#include <algorithm>
#include <iostream>

// ---------------------------------------------------------------------------
// Constructors / Destructor
// ---------------------------------------------------------------------------

AudioPlayer::AudioPlayer(const std::string& strClipName, AudioGroupTag ETag)
    : AudioPlayer(strClipName, "", ETag, OnAudioFinished::STOP, 1.0f)
{}

AudioPlayer::AudioPlayer(const std::string& strClipName, float fVolume)
    : AudioPlayer(strClipName, "", AudioGroupTag::NONE, OnAudioFinished::STOP, fVolume)
{}

// Primary constructor - all members initialised here via member-initialiser
// list so every field has a defined value before the body runs. The track is
// created on the first play(), so a player that is never played holds nothing.
AudioPlayer::AudioPlayer(const std::string& strClipName,
    const std::string& strKey,
    AudioGroupTag      ETag,
    OnAudioFinished    EOnFinished,
    float              fVolume)
    : strKey(strKey)
    , pClip(AudioManager::getInstance()->getAudioClip(strClipName))
    , pTrack(nullptr)
    , ETag(ETag)
    , EOnFinished(EOnFinished)
    , fVolume(fVolume)
    , fProgress(0.0f)
    , bFinished(false)
    , bCleanUp(false)
{
    if (!pClip)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] clip '%s' is not loaded (key='%s')",
            strClipName.c_str(), strKey.c_str());
    }
}

AudioPlayer::~AudioPlayer()
{
    if (pTrack)
    {
        MIX_DestroyTrack(pTrack);   // also stops it
        pTrack = nullptr;
    }
}

// ---------------------------------------------------------------------------
// Track creation
// ---------------------------------------------------------------------------

bool AudioPlayer::ensureTrack()
{
    if (pTrack) return true;

    MIX_Mixer* pMixer = AudioManager::getInstance()->getMixer();
    if (!pMixer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] no mixer available (key='%s')", strKey.c_str());
        return false;
    }

    pTrack = MIX_CreateTrack(pMixer);
    if (!pTrack)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] MIX_CreateTrack failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

// ---------------------------------------------------------------------------
// Playback control
// ---------------------------------------------------------------------------

void AudioPlayer::play()
{
    if (!pClip || !pClip->getAudio())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] play: clip is null (key='%s')", strKey.c_str());
        return;
    }

    if (!ensureTrack()) return;

    // Reset state for fresh playback (supports replaying after stop).
    bFinished = false;
    bCleanUp = false;
    fProgress = 0.0f;

    if (!MIX_SetTrackAudio(pTrack, pClip->getAudio()))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] MIX_SetTrackAudio failed: %s", SDL_GetError());
        return;
    }

    this->applyGain();

    // LOOP loops inside the mixer (-1 = forever), everything else plays once.
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER,
        EOnFinished == OnAudioFinished::LOOP ? -1 : 0);

    if (!MIX_PlayTrack(pTrack, props))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] MIX_PlayTrack failed: %s", SDL_GetError());
    }
    SDL_DestroyProperties(props);
}

void AudioPlayer::stop()
{
    if (!pTrack)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] stop: track is null (key='%s')", strKey.c_str());
        return;
    }

    // Stop right away (0 fade-out frames).
    MIX_StopTrack(pTrack, 0);

    bFinished = true;
    onFinished();
}

// ---------------------------------------------------------------------------
// Volume
// ---------------------------------------------------------------------------

void AudioPlayer::setVolume(float fVol)
{
    fVolume = fVol;
    this->applyGain();
}

void AudioPlayer::applyGain()
{
    if (!pTrack) return;

    float fGain = fVolume * AudioManager::getInstance()->getGroupGain(ETag);
    if (fGain < 0.0f) fGain = 0.0f;
    MIX_SetTrackGain(pTrack, fGain);
}

// ---------------------------------------------------------------------------
// Progress tracking
// ---------------------------------------------------------------------------

// Playing or paused tracks only report their progress. Once the mixer has
// stopped the track by itself (the clip ran out), the finish behaviour runs.
// A looping track never stops by itself, so LOOP only gets here if the mixer
// could not loop the input, in which case it is simply restarted.
void AudioPlayer::updateProgress()
{
    if (!pTrack || !pClip || bFinished) return;

    if (MIX_TrackPlaying(pTrack) || MIX_TrackPaused(pTrack))
    {
        Sint64 nDuration = MIX_GetAudioDuration(pClip->getAudio());
        Sint64 nPosition = MIX_GetTrackPlaybackPosition(pTrack);
        if (nDuration > 0 && nPosition >= 0)
        {
            fProgress = static_cast<float>(nPosition % nDuration) / static_cast<float>(nDuration);
        }
        return;
    }

    fProgress = 1.0f;

    switch (EOnFinished)
    {
    case OnAudioFinished::STOP:
        bCleanUp = true;
        stop();
        break;

    case OnAudioFinished::PAUSE:
        stop();
        break;

    case OnAudioFinished::LOOP:
        play();
        break;

    case OnAudioFinished::FUNC:
        bCleanUp = true;
        stop();
        break;
    }
}

// ---------------------------------------------------------------------------
// Observer list
// ---------------------------------------------------------------------------

void AudioPlayer::addListener(IAudioPlayerListener* pListener)
{
    if (pListener)
        vecListener.push_back(pListener);
}

void AudioPlayer::removeListener(IAudioPlayerListener* pListener)
{
    vecListener.erase(
        std::remove(vecListener.begin(), vecListener.end(), pListener),
        vecListener.end());
}

void AudioPlayer::onFinished()
{
    std::vector<IAudioPlayerListener*> snapshot = vecListener;
    for (IAudioPlayerListener* pListener : snapshot)
        if (pListener) pListener->onAudioFinished();
}
