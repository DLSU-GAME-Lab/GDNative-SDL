#pragma once
#include <SDL3/SDL.h>

class AudioClip
{
private:
    Uint8* buffer;
    Uint32 length;

    AudioClip(Uint8* buffer, Uint32 length);
    ~AudioClip();

    friend class AudioManager;
};


