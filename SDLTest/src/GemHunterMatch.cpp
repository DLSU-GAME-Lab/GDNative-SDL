#include "GemHunterMatch.h"

#include "TextureManager.h"
#include "ShaderManager.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "CameraManager.h"
#include "RendererContext.h"
#include "UIManager.h"
#include "MetricsManager.h"
#include "Settings.h"

// scenes
#include "Title_Scene.h"
#include "Level_1_Scene.h"
#include "Level_2_Scene.h"
#include "Level_3_Scene.h"

IMPLEMENT_GAME(GemHunterMatch)

SDL_Window* GemHunterMatch::createWindow()
{
	this->strWindowTitle = "Gem Hunter Match";

	int windowHeight = gameHeight;
	int windowWidth = gameWidth;

	// SDL3 on Android typically runs in fullscreen
	Uint32 windowFlags = SDL_WINDOW_RESIZABLE;
	CameraManager::initialize();
	// Use logical size
	CameraManager::getInstance()->setWindowSize(gameWidth, gameHeight);
#ifdef __ANDROID__
	windowFlags |= SDL_WINDOW_FULLSCREEN;
	windowWidth = 0;  // Let SDL choose appropriate size
	windowHeight = 0; // Let SDL choose appropriate size
#endif

	return SDL_CreateWindow(
		strWindowTitle.c_str(),
		windowWidth,
		windowHeight,
		windowFlags
	);
}

// GPU renderer = required for custom fragment shaders (SDL 3.4+). If it
// can't be created (no Vulkan, e.g. Android < 7.0), fall back to SDL's
// normal renderer: the game runs as before and sprites are drawn plain.
SDL_Renderer* GemHunterMatch::createRenderer(SDL_Window* pWindow)
{
	return ShaderManager::createRenderer(pWindow, useGPURenderer);
}

void GemHunterMatch::setupPresentation(SDL_Window* pWindow, SDL_Renderer* pRenderer)
{
	// --- Get actual window size (important for Android) ---
	int actualWidth, actualHeight;
	SDL_GetWindowSize(pWindow, &actualWidth, &actualHeight);
	SDL_Log("Actual window size: %dx%d", actualWidth, actualHeight);

	// --- Logical presentation ---
	if (!SDL_SetRenderLogicalPresentation(
		pRenderer,
		gameWidth,
		gameHeight,
		SDL_LOGICAL_PRESENTATION_LETTERBOX))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_SetRenderLogicalPresentation failed: %s",
			SDL_GetError());
	}
	else
	{

		int actualPixelW, actualPixelH;
		SDL_GetWindowSizeInPixels(pWindow, &actualPixelW, &actualPixelH);
		SDL_Log("Actual pixel size: %d x %d", actualPixelW, actualPixelH);
		CameraManager::getInstance()->setActualWindowSize(actualPixelW, actualPixelH);
	}
	SDL_FRect letterboxRect;

	SDL_GetRenderLogicalPresentationRect(pRenderer, &letterboxRect);
	CameraManager::getInstance()->setLetterboxRect(letterboxRect.x, letterboxRect.y, letterboxRect.w, letterboxRect.h);
}

void GemHunterMatch::initializeSystems(SDL_Window* pWindow, SDL_Renderer* pRenderer)
{
	GameObjectManager::initialize();
	SceneManager::initialize();
	TextureManager::initialize(pRenderer);
	ShaderManager::initialize(pRenderer);

	RendererContext::initialize(pRenderer);
	SceneTransitionManager::initialize();
	FontManager::initialize();
	AudioManager::initialize();
	UIManager::initialize(pWindow, pRenderer);
	this->initializeMetrics();
}

void GemHunterMatch::registerScenes()
{
	auto titleScene = new Title_Scene();
	auto level1Scene = new Level_1_Scene();
	auto level2Scene = new Level_2_Scene();
	auto level3Scene = new Level_3_Scene();

	SceneManager::getInstance()->registerScene(titleScene);
	SceneManager::getInstance()->registerScene(level1Scene);
	SceneManager::getInstance()->registerScene(level2Scene);
	SceneManager::getInstance()->registerScene(level3Scene);
}

void GemHunterMatch::destroySystems()
{
	AudioManager::destroy();
	UIManager::destroy();
	ShaderManager::destroy();   // before TextureManager: materials reference textures
	TextureManager::destroy();
	CameraManager::destroy();
	SceneManager::destroy();
	GameObjectManager::destroy();
	SceneTransitionManager::destroy();
	MetricsManager::destroy();
	RendererContext::destroy();
}

void GemHunterMatch::onEvent(SDL_Event* pEvent)
{
	if (pEvent->type == SDL_EVENT_FINGER_DOWN || pEvent->type == SDL_EVENT_FINGER_UP ||
		pEvent->type == SDL_EVENT_FINGER_MOTION)
	{
		SDL_Log("Touch event: type=%d, x=%.2f, y=%.2f",
			pEvent->type, pEvent->tfinger.x, pEvent->tfinger.y);
	}
}

void GemHunterMatch::update(float fDeltaTime)
{
	GameObjectManager::getInstance()->update(fDeltaTime);
	AudioManager::getInstance()->update();
	SceneTransitionManager::getInstance()->update();
	GameObjectManager::getInstance()->cleanUpDeletedObjects();
}
