#pragma once
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

// ---------------------------------------------------------------------------
// AudioClip: one loaded sound (wav, ogg, ...). Wraps a MIX_Audio, which is
// reference-counted by SDL_mixer, so a track that is still playing it stays
// valid. Owned by AudioManager.
// ---------------------------------------------------------------------------
class AudioClip
{
private:
    std::string strName;
    MIX_Audio* pAudio;

public:
    AudioClip(std::string strName, MIX_Audio* pAudio);
    ~AudioClip();

public:
    std::string getName() const;
    MIX_Audio* getAudio() const;
    Sint64 getDurationMS() const;   // -1 when unknown
};
