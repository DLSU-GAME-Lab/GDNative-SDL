#pragma once

#include "AudioPlayer.h"
#include <string>
#include <vector>
#include <unordered_map>

class AudioManager
{
private:
    SDL_AudioSpec mSpec;
    std::vector<AudioClip*> vecAudioClip;
    std::unordered_map<std::string, AudioClip*> mapAudioClip;

    std::vector<AudioPlayer*> vecPlaying;
    std::unordered_map<std::string, AudioPlayer*> mapPlaying;
    std::unordered_map<AudioGroupTag, float> mapGroupVolumes;

    std::vector<AudioPlayer*> vecToDestroy;

public:
    void load(std::string strPath, std::string strName);
    void unload(std::string strName);
	AudioClip* getAudioClip(std::string strName);

	void play(AudioPlayer* pPlayer);
	void stop(std::string strKey);
	void stopAll();

	void update();
    void cleanUp();

	void setVolume(std::string strKey, float fVolume);
	void setVolume(AudioGroupTag ETag, float fVolume);
	float getVolume(std::string strKey);
	float getVolume(AudioGroupTag ETag);
    bool isPlaying(std::string strKey);

private:
    void stopByData(AudioPlayer* pPlayer);
    static void audioStreamCallback(void* pData, SDL_AudioStream* pStream, int nExtra, int nTotal);

	friend class AudioPlayer;

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

