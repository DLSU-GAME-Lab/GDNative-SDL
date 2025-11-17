#include "AudioClip.h"

AudioClip::AudioClip(Uint8* buffer, Uint32 length)
{
	this->buffer = buffer;
	this->length = length;
}

AudioClip::~AudioClip()
{
	SDL_free(this->buffer);
}

Uint8* AudioClip::getBuffer() const
{
	return this->buffer;
}

Uint32 AudioClip::getLength() const
{
	return this->length;
}
