#pragma once

#include "AudioClip.h"
#include "EnumAudioGroupTag.h"
#include "EnumOnAudioFinished.h"
#include "IAudioPlayerListener.h"
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// AudioPlayer
// Wraps an SDL_AudioStream for a single AudioClip. Supports play/stop,
// progress tracking, completion callbacks, and an observer listener list.
//
// Ownership model:
//   - AudioPlayer does NOT own the AudioClip; it borrows a pointer from
//     AudioManager (which is the clip's owner).
//   - AudioPlayer DOES own the SDL_AudioStream and the SDL_AudioDeviceID
//     it opens; both are destroyed in the destructor.
//   - AudioPlayer does NOT own its IAudioPlayerListener pointers.
// ---------------------------------------------------------------------------
class AudioPlayer
{
private:
    std::string         strKey;
    AudioClip* pClip;          // Borrowed; not freed by this class.
    SDL_AudioStream* pStream;        // Owned; freed in destructor.
    SDL_AudioDeviceID   nDeviceID;      // Owned; closed in destructor.

    AudioGroupTag       ETag;
    OnAudioFinished     EOnFinished;
    float               fVolume;        // [0.0, 1.0]
    float               fProgress;      // [0.0, 1.0], updated per callback.
    Uint32              nBytesQueued;   // Running total of bytes fed to stream.
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

    // Non-copyable; streams and device IDs are unique resources.
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

    // Sets the gain on the bound SDL_AudioStream ([0.0, 1.0]).
    void setVolume(float fVol);
    float getVolume() const { return fVolume; }

    // --- Observer list ---
    void addListener(IAudioPlayerListener* pListener);
    void removeListener(IAudioPlayerListener* pListener);

private:
    // Creates, configures, and returns a bound SDL_AudioStream.
    // Also opens and stores the audio device in nDeviceID.
    // Returns nullptr on any failure; errors are logged via SDL_LogError.
    SDL_AudioStream* createAudioStream(AudioClip* pClip);

    // Updates fProgress from the number of bytes left in the stream.
    void updateProgress();

    // Notifies all listeners and (if LOOP) restarts playback.
    // Called from the callback when fProgress reaches 1.0, and from stop().
    void onFinished();

    friend class AudioManager;
};
