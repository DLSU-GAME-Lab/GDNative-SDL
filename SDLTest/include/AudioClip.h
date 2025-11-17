#pragma once
#include <SDL3/SDL.h>

class AudioClip
{
private:
    Uint8* buffer;
    Uint32 length;

    AudioClip(Uint8* buffer, Uint32 length);
    ~AudioClip();

public:
    Uint8* getBuffer() const;
    Uint32 getLength() const;

    friend class AudioManager;

};


