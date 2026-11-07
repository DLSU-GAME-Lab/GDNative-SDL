#pragma once

#include "AudioClip.h"
#include "EnumAudioGroupTag.h"
#include "EnumOnAudioFinished.h"
#include "IAudioPlayerListener.h"
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// AudioPlayer
// Plays a single AudioClip on its own SDL_mixer track. Supports play/stop,
// progress tracking, completion callbacks, and an observer listener list.
//
// Ownership model:
//   - AudioPlayer does NOT own the AudioClip; it borrows a pointer from
//     AudioManager (which is the clip's owner).
//   - AudioPlayer DOES own its MIX_Track, which it creates on the first
//     play() and destroys in the destructor. All tracks share the one mixer
//     (and the one audio device) owned by AudioManager.
//   - AudioPlayer does NOT own its IAudioPlayerListener pointers.
//
// Looping is done by the mixer itself (OnAudioFinished::LOOP plays the clip
// with infinite loops), so it is gapless and needs no polling.
// ---------------------------------------------------------------------------
class AudioPlayer
{
private:
    std::string         strKey;
    AudioClip* pClip;          // Borrowed; not freed by this class.
    MIX_Track* pTrack;         // Owned; created on first play, freed in destructor.

    AudioGroupTag       ETag;
    OnAudioFinished     EOnFinished;
    float               fVolume;        // [0.0, 1.0]
    float               fProgress;      // [0.0, 1.0], updated by AudioManager::update().
    bool                bFinished;
    bool                bCleanUp;

    std::vector<IAudioPlayerListener*> vecListener;

public:
    // Convenience constructors delegate to the primary constructor.
    AudioPlayer(const std::string& strClipName, AudioGroupTag ETag);
    AudioPlayer(const std::string& strClipName, float fVolume);

    // Primary constructor.
    AudioPlayer(const std::string& strClipName,
        const std::string& strKey,
        AudioGroupTag      ETag,
        OnAudioFinished    EOnFinished = OnAudioFinished::STOP,
        float              fVolume = 1.0f);

    ~AudioPlayer();

    // Non-copyable; a track is a unique resource.
    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;

    // --- Playback control ---
    void play();
    void stop();

    // --- Getters (used by AudioManager) ---
    bool               isFinished()  const { return bFinished; }
    bool               needsCleanUp()const { return bCleanUp; }
    float              getProgress() const { return fProgress; }
    const std::string& getKey()      const { return strKey; }
    AudioGroupTag      getTag()      const { return ETag; }

    // Sets this player's volume ([0.0, 1.0]). The group volumes set on
    // AudioManager are applied on top of it.
    void setVolume(float fVol);
    float getVolume() const { return fVolume; }

    // --- Observer list ---
    void addListener(IAudioPlayerListener* pListener);
    void removeListener(IAudioPlayerListener* pListener);

private:
    // Creates the track on the shared mixer if it doesn't exist yet.
    bool ensureTrack();

    // Pushes volume * group volume to the track.
    void applyGain();

    // Updates fProgress from the track position and handles the clip having
    // ended (stop / pause / func). Called by AudioManager::update().
    void updateProgress();

    // Notifies all listeners. Called when playback finishes and from stop().
    void onFinished();

    friend class AudioManager;
};
