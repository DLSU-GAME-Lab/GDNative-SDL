#pragma once
#include <SDL3/SDL.h>
#include <string>

class AudioClip
{
private:
	std::string strName;
    Uint8* buffer;
    Uint32 length;
    SDL_AudioSpec spec;

public:
    AudioClip(std::string strName, Uint8* buffer, Uint32 length, SDL_AudioSpec spec);
    ~AudioClip();

public:
	std::string getName() const;
    Uint8* getBuffer() const;
    Uint32 getLength() const;
	SDL_AudioSpec getSpec() const;
};


