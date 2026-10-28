#include "ManifestLoader.h"
#include <SDL3/SDL.h>
#include <sstream>
#include <string>
#include <vector>

// Reads a text manifest (one asset path per line).
//
// SDL_LoadFile goes through SDL_IOFromFile, which on Android reads straight
// from the APK's assets folder and on desktop reads from the working
// directory. So no JNI / AAssetManager is needed on either platform.
std::vector<std::string> readAssetManifest(const std::string& manifestFilename)
{
    std::vector<std::string> out;

    size_t size = 0;
    void* data = SDL_LoadFile(manifestFilename.c_str(), &size);
    if (!data) {
        SDL_Log("readAssetManifest: can't open manifest '%s' : %s", manifestFilename.c_str(), SDL_GetError());
        return out;
    }

    std::string buf(static_cast<char*>(data), size);
    SDL_free(data);

    std::istringstream iss(buf);
    std::string line;
    while (std::getline(iss, line)) {
        // Manifests generated on Windows use CRLF; drop trailing CR/whitespace
        // so entries are valid asset paths.
        while (!line.empty() && (line.back() == '\r' || line.back() == ' ' || line.back() == '\t'))
            line.pop_back();
        if (!line.empty()) {
            out.push_back(line);
        }
    }

    return out;
}
