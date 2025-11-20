#pragma once

#include "AudioClip.h"
#include "EnumAudioGroupTag.h"
#include "EnumOnAudioFinished.h"
#include <string>

class AudioPlayer
{
private:
    std::string strKey;
	AudioClip* pClip;
    SDL_AudioStream* pStream;

    AudioGroupTag ETag;
    OnAudioFinished EOnFinished;
    float fProgress;
    bool bFinished;
    bool bCleanUp;

public:
    AudioPlayer(std::string strClipName, AudioGroupTag ETag);
    AudioPlayer(std::string strClipName, float fVolume);
    AudioPlayer(std::string strClipName, std::string strKey, AudioGroupTag ETag, OnAudioFinished EOnFinished = OnAudioFinished::STOP, float fVolume = 1.0f);

private:
	void onFinished();
    static void audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal);

	friend class AudioManager;
};

