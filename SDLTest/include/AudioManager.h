#pragma once

#include "AudioClip.h"
#include "EnumAudioGroupTag.h"
#include <string>
#include <vector>
#include <unordered_map>

class AudioManager
{
private:
    struct AudioData
    {
        std::string strClipName;
        std::string strKey;
        AudioGroupTag ETag;
        SDL_AudioStream* pStream;
        float fProgress;
        bool bCleanUp;
    };

    SDL_AudioSpec mSpec;
    std::vector<AudioClip*> vecAudioClip;
    std::unordered_map<std::string, AudioClip*> mapAudioClip;

    std::vector<AudioData*> vecPlaying;
    std::unordered_map<std::string, AudioData*> mapPlaying;
    std::unordered_map<AudioGroupTag, float> mapGroupVolumes;

    std::vector<AudioData*> vecToDestroy;

    void stopByData(AudioData* pData);

public:
    void load(std::string strPath, std::string strName);
    void unload(std::string strName);

    void play(std::string strName, std::string streamKey, float fVolume);
    void play(std::string strName, AudioGroupTag ETag = AudioGroupTag::NONE, std::string streamKey = "", float fVolume = 1.0f);
	void stop(std::string streamKey);
	void stopAll();

    void cleanUp();

	void setVolume(std::string streamKey, float fVolume);
	void setVolume(AudioGroupTag ETag, float fVolume);
	float getVolume(std::string streamKey);
	float getVolume(AudioGroupTag ETag);

private:
    static void audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static AudioManager* P_SHARED_INSTANCE;

private:
    AudioManager();
    ~AudioManager();
    AudioManager(const AudioManager&) {};
    AudioManager& operator=(const AudioManager&) {};

public:
    static void initialize();
    static void destroy();

    static AudioManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

