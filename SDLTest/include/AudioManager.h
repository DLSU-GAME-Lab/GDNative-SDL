#pragma once

#include "AudioClip.h"
#include <string>
#include <vector>
#include <unordered_map>

class AudioManager
{
private:
    SDL_AudioSpec mSpec;
    std::vector<AudioClip*> vecAudioClip;
    std::unordered_map<std::string, AudioClip*> mapAudioClip;
    std::unordered_map<std::string, SDL_AudioStream*> mapAudioStream;

public:
    void load(std::string strPath, std::string strName);
    void unload(std::string strName);
    void play(std::string strName);

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

