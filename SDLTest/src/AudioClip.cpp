#include "AudioClip.h"

AudioClip::AudioClip(std::string strName, Uint8* buffer, Uint32 length, SDL_AudioSpec spec)
{
	this->strName = strName;
	this->buffer = buffer;
	this->length = length;
	this->spec = spec;
}

AudioClip::~AudioClip()
{
	SDL_free(this->buffer);
}

std::string AudioClip::getName() const
{
	return this->strName;
}

Uint8* AudioClip::getBuffer() const
{
	return this->buffer;
}

Uint32 AudioClip::getLength() const
{
	return this->length;
}

SDL_AudioSpec AudioClip::getSpec() const
{
	return this->spec;
}
