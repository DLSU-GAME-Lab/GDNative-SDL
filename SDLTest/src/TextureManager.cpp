// ---------------------------------------------------------------------------
// Responsibilities: load textures from disk, create cached textures, unload.
// ---------------------------------------------------------------------------

#include "TextureManager.h"
#include "FontManager.h"
#include <SDL3_image/SDL_image.h>
#include "SDL3_ttf/SDL_ttf.h"
#include "StringUtils.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "RendererContext.h"
#include "ManifestLoader.h"
#include <fstream>
#include <cstdio>

std::string fixAssetPathCase(const std::string& path, const std::vector<std::string>& manifest) {
    // Check if exact path exists in manifest
    for (const auto& entry : manifest) {
        if (entry == path) return path;
    }

    // Try lowercase extension
    std::string lowerPath = path;
    size_t dotPos = lowerPath.rfind('.');
    if (dotPos != std::string::npos) {
        for (size_t i = dotPos + 1; i < lowerPath.length(); i++) {
            lowerPath[i] = std::tolower(lowerPath[i]);
        }
        for (const auto& entry : manifest) {
            if (entry == lowerPath) return lowerPath;
        }
    }

    // Return original if not found
    return path;
}

void TextureManager::load(std::string assetPath, std::string strName)
{
    SDL_Log("[TextureManager] Loading texture via asset: %s with key: %s",
            assetPath.c_str(), strName.c_str());

    SDL_Texture* pTexture = nullptr;

#ifdef __ANDROID__
    // On Android, use SDL_LoadFile which works with APK assets
    SDL_Surface* surface = IMG_Load(assetPath.c_str());
    if (!surface) {
        // Try with different case for extension
        std::string altPath = assetPath;
        size_t dotPos = altPath.rfind('.');
        if (dotPos != std::string::npos) {
            // Try lowercase extension
            std::string lowerExt = altPath.substr(dotPos);
            std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
            altPath = altPath.substr(0, dotPos) + lowerExt;

            surface = IMG_Load(altPath.c_str());
            if (!surface) {
                // Try uppercase extension
                std::string upperExt = altPath.substr(dotPos);
                std::transform(upperExt.begin(), upperExt.end(), upperExt.begin(), ::toupper);
                altPath = altPath.substr(0, dotPos) + upperExt;
                surface = IMG_Load(altPath.c_str());
            }
        }
    }
#else
    // Desktop implementation
    SDL_Surface* surface = IMG_Load(assetPath.c_str());
#endif

    if (surface) {
        pTexture = SDL_CreateTextureFromSurface(this->pRenderer, surface);
        SDL_DestroySurface(surface);
        SDL_Log("[TextureManager] Successfully loaded: %s", assetPath.c_str());
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "IMG_Load failed for %s: %s", assetPath.c_str(), SDL_GetError());
        return;
    }

    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);
}

// loadFromFolder: O(F) where F = number of files processed. Total wall-time
// grows with sum of per-file I/O and decode costs.
void TextureManager::loadFromFolder(std::string strPath, std::string strName)
{
    SDL_Log("[TextureManager] Loading from folder: %s", strPath.c_str());

    // Remove trailing slash if present
    while (!strPath.empty() && strPath.back() == '/') {
        strPath.pop_back();
    }

    bool loadedAny = false;

    // Method 1: Try to use the manifest to discover files
    auto manifest = readAssetManifest("asset_manifest.txt");
    if (!manifest.empty()) {
        SDL_Log("[TextureManager] Using manifest to discover files in: %s", strPath.c_str());

        std::vector<std::pair<int, std::string>> numberedFiles; // Store (number, path) pairs

        for (const auto& entry : manifest) {
            // Check if entry is in the requested folder
            std::string folderPrefix = strPath + "/";
            if (entry.find(folderPrefix) == 0) {
                // Extract filename
                std::string filename = entry.substr(folderPrefix.length());

                // Try to extract a number from the filename
                int frameNumber = extractFrameNumber(filename);
                if (frameNumber >= 0) {
                    numberedFiles.emplace_back(frameNumber, entry);
                    SDL_Log("[TextureManager] Found numbered file: %s (frame %d)", entry.c_str(), frameNumber);
                }
            }
        }

        // Sort by frame number
        std::sort(numberedFiles.begin(), numberedFiles.end(),
                  [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
                      return a.first < b.first;
                  });

        // Load in sorted order
        for (const auto& [frameNum, filePath] : numberedFiles) {
            this->load(filePath, strName);
            loadedAny = true;
        }
    }

    // Method 2: If manifest didn't work, try pattern-based loading
    if (!loadedAny) {
        SDL_Log("[TextureManager] Trying pattern-based loading for: %s", strPath.c_str());

        // Try common patterns
        std::vector<std::string> patterns = {
                "Set", "set", "Frame", "frame",
                "SET", "FRAME",  // Uppercase variations
                "img", "Img", "IMG",
                "image", "Image", "IMAGE"
        };

        // Try loading up to 100 frames for each pattern
        for (const auto& pattern : patterns) {
            for (int i = 0; i < 100; i++) {
                std::string pathUpper = strPath + "/" + pattern + std::to_string(i) + ".PNG";
                std::string pathLower = strPath + "/" + pattern + std::to_string(i) + ".png";

                if (tryLoadFile(pathUpper, strName) || tryLoadFile(pathLower, strName)) {
                    loadedAny = true;
                } else {
                    // If we haven't found any files with this pattern after 10 attempts, break
                    if (i > 10 && !loadedAny) {
                        break;
                    }
                }
            }

            // If we found files with this pattern, don't try other patterns
            if (loadedAny) {
                SDL_Log("[TextureManager] Found files using pattern: %s", pattern.c_str());
                break;
            }
        }
    }

    // Method 3: Try loading files with parentheses (e.g., "frame (01).PNG")
    if (!loadedAny) {
        SDL_Log("[TextureManager] Trying parenthesized pattern for: %s", strPath.c_str());

        for (int i = 0; i < 100; i++) {
            // Format with leading zeros
            std::string number = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);

            std::string path1 = strPath + "/frame (" + number + ").PNG";
            std::string path2 = strPath + "/frame (" + number + ").png";
            std::string path3 = strPath + "/Frame (" + number + ").PNG";
            std::string path4 = strPath + "/Frame (" + number + ").png";

            if (tryLoadFile(path1) || tryLoadFile(path2) ||
                tryLoadFile(path3) || tryLoadFile(path4)) {
                loadedAny = true;
            }
        }
    }

    if (!loadedAny) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "[TextureManager] Could not load any files from: %s", strPath.c_str());
    }
}

// Helper function to extract frame number from filename
int TextureManager::extractFrameNumber(const std::string& filename) {
    // Remove extension
    size_t dotPos = filename.rfind('.');
    std::string basename = (dotPos != std::string::npos) ? filename.substr(0, dotPos) : filename;

    // Common patterns and their prefixes
    static const std::vector<std::string> prefixes = {
            "Set", "set", "Frame", "frame", "SET", "FRAME",
            "img", "Img", "IMG", "image", "Image", "IMAGE"
    };

    // Try each prefix
    for (const auto& prefix : prefixes) {
        if (basename.find(prefix) == 0) {
            std::string numberStr = basename.substr(prefix.length());

            // Handle parenthesized numbers like "(01)"
            if (!numberStr.empty() && numberStr[0] == ' ') {
                numberStr = numberStr.substr(1); // Remove space
            }
            if (!numberStr.empty() && numberStr[0] == '(' && numberStr.back() == ')') {
                numberStr = numberStr.substr(1, numberStr.length() - 2);
            }

            // Try to convert to number
            try {
                return std::stoi(numberStr);
            } catch (...) {
                // Not a valid number, try next prefix
                continue;
            }
        }
    }

    return -1; // Not a numbered file
}

// Helper function to try loading a file
bool TextureManager::tryLoadFile(const std::string& path, const std::string& name) {
    size_t size = 0;
    void* data = SDL_LoadFile(path.c_str(), &size);
    if (data && size > 0) {
        SDL_free(data);

        // Extract actual name to use (either provided or extract from path)
        std::string actualName = name;
        if (actualName.empty()) {
            // Extract name from last part of path
            size_t lastSlash = path.find_last_of('/');
            if (lastSlash != std::string::npos) {
                std::string filename = path.substr(lastSlash + 1);
                size_t dotPos = filename.rfind('.');
                if (dotPos != std::string::npos) {
                    actualName = filename.substr(0, dotPos);
                }
            }
        }

        this->load(path, actualName);
        return true;
    }
    return false;
}

// loadFromText: O(1) algorithmically; TTF rendering cost is an expensive
// constant that affects wall-time.
void TextureManager::loadFromText(std::string strName, std::string fontType, int nFontSize, std::string textureText, SDL_Color textColor)
{
    // O(1): creates texture from text; I/O and render cost from SDL_ttf.
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(FontManager::getInstance()->getFont(fontType, nFontSize), textureText.c_str(), 0, textColor, 0);
    if (textSurface == nullptr)
    {
        std::cout << "[ERROR]: Could not render text." << SDL_GetError() << std::endl;
        return;
    }
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(this->pRenderer, textSurface);
    SDL_DestroySurface(textSurface);
    if (!pTexture) {
        std::cerr << "[ERROR] : Failed to create texture for [" << strName << "] "
            << "Error: " << SDL_GetError() << std::endl;
        return;
    }
    this->mapTexture[strName].push_back(pTexture);
    this->vecTexture.push_back(pTexture);
}

// unload: current implementation destroys textures for a name and removes
// them from a global vector. Worst-case complexity can be proportional to
// the size of the global texture vector V times the number of textures T
// being removed (O(V * T)) depending on implementation details of removal.
// As V grows large, unload operations may become increasingly expensive.
void TextureManager::unload(std::string strName)
{
    // O(T): destroys all textures associated with name.
    // Erase + vector filtering also O(T) with small constants.
    // destroy each texture associated with this name
    for (SDL_Texture* tex : this->mapTexture[strName]) {
        SDL_DestroyTexture(tex);
    }

    // remove them from the global vector
    this->vecTexture.erase(
        std::remove_if(this->vecTexture.begin(), this->vecTexture.end(),
            [&](SDL_Texture* tex) {
                return std::find(this->mapTexture[strName].begin(),
                    this->mapTexture[strName].end(),
                    tex) != this->mapTexture[strName].end();
            }),
        this->vecTexture.end()
    );

    this->mapTexture[strName].clear();
    
}

// getTexture: returning copies of vectors is O(T) (copy cost). As T increases
// this creates overhead for callers that only needed read access.
std::vector<SDL_Texture*> TextureManager::getTexture(std::string strName, int nStart, int nEnd)
{
    // O(T): copies subset of textures if range given.
    if (mapTexture.find(strName) == mapTexture.end())
        return {};

    if (nStart == -1)
        return mapTexture[strName];

    std::vector<SDL_Texture*> vecTexture = mapTexture[strName];
    if (nEnd == -1)
        nEnd = nStart + 1;

    if (nStart >= 0 && nEnd <= (int)vecTexture.size())
        return std::vector<SDL_Texture*>(vecTexture.begin() + nStart, vecTexture.begin() + nEnd);

    return {};
}

// get: O(1) fetch of first texture pointer by name assuming map lookup is O(1).
// get a single texture by name (first one in the list)
SDL_Texture* TextureManager::get(const std::string& strName)
{
    // O(1): fetches first texture by key.
    if (mapTexture.find(strName) != mapTexture.end() && !mapTexture[strName].empty()) {
        return mapTexture[strName][0];
    }
    return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * 
 *       SINGLETON-RELATED CONTENT       * 
 * * * * * * * * * * * * * * * * * * * * */
TextureManager* TextureManager::P_SHARED_INSTANCE = NULL;


void TextureManager::initialize(SDL_Renderer* pRenderer)
{
    P_SHARED_INSTANCE = new TextureManager();
    P_SHARED_INSTANCE->pRenderer = pRenderer;
}

void TextureManager::destroy()
{
    delete P_SHARED_INSTANCE;
}

TextureManager* TextureManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
/* * * * * * * * * * * * * * * * * * * * */