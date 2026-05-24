#pragma once

#include "AudioClip.h"
#include "EnumAudioGroupTag.h"
#include "EnumOnAudioFinished.h"
#include "IAudioPlayerListener.h"
#include <string>
#include <vector>

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

	std::vector<IAudioPlayerListener*> vecListener;

public:
    AudioPlayer(std::string strClipName, AudioGroupTag ETag);
    AudioPlayer(std::string strClipName, float fVolume);
    AudioPlayer(std::string strClipName, std::string strKey, AudioGroupTag ETag, OnAudioFinished EOnFinished = OnAudioFinished::STOP, float fVolume = 1.0f);
	~AudioPlayer();

    void addListener(IAudioPlayerListener* pListener);
    void removeListener(IAudioPlayerListener* pListener);

private:
    SDL_AudioStream* createAudioStream(AudioClip* pClip);
    void play();
    void stop();

    void onFinished();

	friend class AudioManager;
};

