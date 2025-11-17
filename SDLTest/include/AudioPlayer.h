#pragma once
#include "AComponent.h"
#include "AudioClip.h"
#include <string>

class AudioPlayer : public AComponent
{
private:
	SDL_AudioSpec mSpec;
	SDL_AudioStream* pStream;
	AudioClip* pClip;
	bool bReset;
	bool bFlush;

public:
	AudioPlayer(std::string strName);

	void onAttach() override;
	void perform() override;

	void play();

	void setAudioClip();

private:
	static void audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nCurrent, int nTotal);
};

