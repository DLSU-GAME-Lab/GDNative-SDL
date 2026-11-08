#pragma once

#include "AudioPlayer.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// ---------------------------------------------------------------------------
// AudioManager
// Owns the SDL_mixer mixer (one audio device for the whole game), the loaded
// clips and the players that are currently playing.
// ---------------------------------------------------------------------------
class AudioManager
{
private:
    MIX_Mixer* pMixer;

    std::vector<AudioClip*> vecAudioClip;
    std::unordered_map<std::string, AudioClip*> mapAudioClip;

    std::vector<AudioPlayer*> vecPlaying;
    std::unordered_map<std::string, AudioPlayer*> mapPlaying;
    std::unordered_map<AudioGroupTag, float> mapGroupVolumes;
    std::unordered_set<AudioGroupTag> setGroupApplied;   // groups whose volume was set explicitly

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

    // The shared mixer, nullptr when the audio device could not be opened.
    MIX_Mixer* getMixer() const;

    // Group volume x master volume for a player of this group. A group only
    // counts once its volume was set with setVolume(AudioGroupTag, ...).
    float getGroupGain(AudioGroupTag ETag) const;

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
    AudioManager& operator=(const AudioManager&) { return *this; }

public:
    static void initialize();
    static void destroy();
    static AudioManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};
