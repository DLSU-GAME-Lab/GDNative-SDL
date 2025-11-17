#pragma once

#include "AudioClip.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <vector>
#include <unordered_map>

class AudioManager
{
private:
    SDL_AudioSpec mSpec;
    std::vector<AudioClip*> vecAudioClip;
    std::unordered_map<std::string, AudioClip*> mapAudioClip;
    std::vector<SDL_AudioStream*> vecFinishedStream;
    SDL_Mutex* pMutex;

public:
    void load(std::string strPath, std::string strName);
    void unload(std::string strName);
    void play(std::string strName);
    void cleanUp();

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

