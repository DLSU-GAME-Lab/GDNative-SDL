#pragma once
#include <SDL3/SDL.h>
#include <string>

class AudioClip
{
private:
	std::string strName;
    Uint8* buffer;
    Uint32 length;

    AudioClip(Uint8* buffer, Uint32 length);
    ~AudioClip();

public:
	std::string getName() const;
    Uint8* getBuffer() const;
    Uint32 getLength() const;

    friend class AudioManager;

};


