#include "AudioClip.h"

AudioClip::AudioClip(std::string strName, Uint8* buffer, Uint32 length)
{
	this->strName = strName;
	this->buffer = buffer;
	this->length = length;
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
