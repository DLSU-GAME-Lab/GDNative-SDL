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

// Primary constructor — all members initialised here via member-initialiser
// list so every field has a defined value before the body runs.
AudioPlayer::AudioPlayer(const std::string& strClipName,
    const std::string& strKey,
    AudioGroupTag      ETag,
    OnAudioFinished    EOnFinished,
    float              fVolume)
    : strKey(strKey)
    , pClip(AudioManager::getInstance()->getAudioClip(strClipName))
    , pStream(nullptr)
    , nDeviceID(0)
    , ETag(ETag)
    , EOnFinished(EOnFinished)
    , fVolume(fVolume)
    , fProgress(0.0f)
    , nBytesQueued(0)
    , bFinished(false)
    , bCleanUp(false)
{
    // createAudioStream sets nDeviceID as a side-effect.
    pStream = createAudioStream(pClip);

    // Apply initial volume to the stream if it was created successfully.
    if (pStream)
        SDL_SetAudioStreamGain(pStream, fVolume);
}

// FIX: destructor now also closes the audio device that was opened in
// createAudioStream. The original only destroyed the stream, leaking the
// SDL_AudioDeviceID handle.
AudioPlayer::~AudioPlayer()
{
    if (pStream)
    {
        SDL_DestroyAudioStream(pStream);
        pStream = nullptr;
    }
    if (nDeviceID != 0)
    {
        SDL_CloseAudioDevice(nDeviceID);
        nDeviceID = 0;
    }
}

// ---------------------------------------------------------------------------
// Stream creation
// ---------------------------------------------------------------------------

// FIX: the original opened a new SDL_AudioDeviceID but never stored it,
// making it impossible to close later — a resource leak on every AudioPlayer.
// nDeviceID is now stored as a member so the destructor can close it.
SDL_AudioStream* AudioPlayer::createAudioStream(AudioClip* pClip)
{
    if (!pClip)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] createAudioStream: AudioClip is null (key='%s')",
            strKey.c_str());
        return nullptr;
    }

    Uint8* buf = pClip->getBuffer();
    Uint32         len = pClip->getLength();
    SDL_AudioSpec  spec = pClip->getSpec();

    if (!buf || len == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] createAudioStream: AudioClip has empty buffer (key='%s')",
            strKey.c_str());
        return nullptr;
    }

    // Open the default playback device.
    nDeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (nDeviceID == 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] SDL_OpenAudioDevice failed: %s", SDL_GetError());
        return nullptr;
    }

    // Query the device's native format so the stream can resample/convert.
    SDL_AudioSpec deviceSpec{};
    SDL_GetAudioDeviceFormat(nDeviceID, &deviceSpec, nullptr);

    SDL_AudioStream* pNewStream = SDL_CreateAudioStream(&spec, &deviceSpec);
    if (!pNewStream)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] SDL_CreateAudioStream failed: %s", SDL_GetError());
        SDL_CloseAudioDevice(nDeviceID);
        nDeviceID = 0;
        return nullptr;
    }

    SDL_BindAudioStream(nDeviceID, pNewStream);
    return pNewStream;
}

// ---------------------------------------------------------------------------
// Playback control
// ---------------------------------------------------------------------------

void AudioPlayer::play()
{
    if (!pStream || !pClip)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] play: stream or clip is null (key='%s')", strKey.c_str());
        return;
    }

    // Reset state for fresh playback (supports replaying after stop).
    bFinished = false;
    bCleanUp = false;
    fProgress = 0.0f;
    nBytesQueued = 0;

    SDL_PutAudioStreamData(this->pStream, this->pClip->getBuffer(), this->pClip->getLength());
}

void AudioPlayer::stop()
{
    if (!pStream)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "[AudioPlayer] stop: stream is null (key='%s')", strKey.c_str());
        return;
    }

    // Drain any data still queued in the stream so playback stops immediately.
    SDL_ClearAudioStream(pStream);

    bFinished = true;
    onFinished();
}

// ---------------------------------------------------------------------------
// Volume
// ---------------------------------------------------------------------------

void AudioPlayer::setVolume(float fVol)
{
    fVolume = fVol;
    if (pStream)
        SDL_SetAudioStreamGain(pStream, fVolume);
}

// ---------------------------------------------------------------------------
// Progress tracking
// ---------------------------------------------------------------------------

// FIX: updateProgress was entirely commented out in the original, leaving
// fProgress permanently at 0.0f. The callback check (fProgress < 1.0f)
// therefore never triggered completion logic. Progress is now computed from
// bytes still queued in the stream vs. the total clip length.
void AudioPlayer::updateProgress()
{
    if (!pClip || pClip->getLength() == 0) return;

    int nQueued = SDL_GetAudioStreamQueued(pStream);
    if (nQueued < 0) nQueued = 0;

    Uint32 totalBytes = pClip->getLength();

    // fProgress = fraction of the clip that has been *consumed*.
    float fConsumed = static_cast<float>(totalBytes - static_cast<Uint32>(nQueued));
    fProgress = fConsumed / static_cast<float>(totalBytes);
    if (fProgress < 0.0f) fProgress = 0.0f;
    if (fProgress > 1.0f) fProgress = 1.0f;

    if (fProgress < 1.0f) return;

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
        // Reset state and queue the clip again for seamless looping.
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
