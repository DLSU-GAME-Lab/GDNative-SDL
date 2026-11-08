#include "BabaylanTayles.h"

#include "TextureManager.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "SceneTransitionManager.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "DataAssetManager.h"
#include "EventBroadcaster.h"
#include "RendererContext.h"
#include "UIManager.h"
#include "MetricsManager.h"
#include "Settings.h"
#include <algorithm>
#include <iostream>

// scenes
#include "LobbyScene.h"
#include "Title_Scene.h"
#include "RightRoomScene.h"
#include "LeftRoomScene.h"
#include "PlatformerLevel1Scene.h"
#include "PuzzleLevel1Scene.h"

IMPLEMENT_GAME(BabaylanTayles)

SDL_Window* BabaylanTayles::createWindow()
{
	// --- window / renderer creation ---
	int screenWidth, screenHeight;
	SDL_DisplayID dispID = SDL_GetPrimaryDisplay();
	SDL_DisplayMode dispMode = *SDL_GetDesktopDisplayMode(dispID);
	screenWidth = dispMode.w;
	screenHeight = dispMode.h;

	// debug
	std::cout << "Screen size: " << screenWidth << "x" << screenHeight << std::endl;

	this->strWindowTitle = "Babaylan Tayles";
	this->scaleX = (float)screenWidth / gameWidth;
	this->scaleY = (float)screenHeight / gameHeight;
	float scale = (std::min)(scaleX, scaleY);

	int windowWidth = (int)(gameWidth * scale);
	int windowHeight = (int)(gameHeight * scale);

	this->pWindow = SDL_CreateWindow(
		strWindowTitle.c_str(),
		windowWidth,
		windowHeight,
		SDL_WINDOW_FULLSCREEN
	);

	return this->pWindow;
}

SDL_Renderer* BabaylanTayles::createRenderer(SDL_Window* pWindow)
{
	return SDL_CreateRenderer(pWindow, NULL);
}

void BabaylanTayles::setupPresentation(SDL_Window* pWindow, SDL_Renderer* pRenderer)
{
	SDL_SetRenderLogicalPresentation(
		pRenderer,
		(int)gameWidth,
		(int)gameHeight,
		SDL_LOGICAL_PRESENTATION_LETTERBOX
	);
}

void BabaylanTayles::initializeSystems(SDL_Window* pWindow, SDL_Renderer* pRenderer)
{
	GameObjectManager::initialize();
	SceneManager::initialize();
	InputManager::initialize();
	TextureManager::initialize(pRenderer);
	ShaderManager::initialize(pRenderer);
	CameraManager::initialize();
	RendererContext::initialize(pRenderer);
	// setWindowSize likely O(1) or O(#render targets)
	CameraManager::getInstance()->setWindowSize(pWindow);
	CameraManager::getInstance()->setWindowScale(this->scaleX, this->scaleY);
	SceneTransitionManager::initialize();
	FontManager::initialize();
	AudioManager::initialize();
	DataAssetManager::initialize();
	EventBroadcaster::initialize();
	UIManager::initialize(pWindow, pRenderer);
	this->initializeMetrics();
}

// registerScenes: startup cost O(S) where S = number of scenes registered
void BabaylanTayles::registerScenes()
{
	auto titleScene = new Title_Scene();
	auto lobbyScene = new LobbyScene();
	auto rightRoomScene = new RightRoomScene();
	auto leftRoomScene = new LeftRoomScene();

	auto platformerL1 = new PlatformerLevel1Scene();
	auto puzzleL1 = new PuzzleLevel1Scene();

	SceneManager::getInstance()->registerScene(titleScene);
	SceneManager::getInstance()->registerScene(lobbyScene);
	SceneManager::getInstance()->registerScene(rightRoomScene);
	SceneManager::getInstance()->registerScene(leftRoomScene);

	SceneManager::getInstance()->registerScene(platformerL1);
	SceneManager::getInstance()->registerScene(puzzleL1);
}

void BabaylanTayles::destroySystems()
{
	DataAssetManager::destroy();
	AudioManager::destroy();
	UIManager::destroy();
	ShaderManager::destroy();
	TextureManager::destroy();
	CameraManager::destroy();
	InputManager::destroy();
	SceneManager::destroy();
	GameObjectManager::destroy();
	SceneTransitionManager::destroy();
	MetricsManager::destroy();
	RendererContext::destroy();
	EventBroadcaster::destroy();
}

void BabaylanTayles::onEvent(SDL_Event* pEvent)
{
	if (pEvent->type == SDL_EVENT_WINDOW_RESIZED)
	{
		// setWindowSize likely O(1) or O(#render targets)
		CameraManager::getInstance()->setWindowSize(this->pWindow);
	}
}

void BabaylanTayles::processEvent(SDL_Event* pEvent)
{
	InputManager::getInstance()->processEvents(pEvent);
	AGame::processEvent(pEvent);
}

void BabaylanTayles::update(float fDeltaTime)
{
	AudioManager::getInstance()->update();
	GameObjectManager::getInstance()->update(fDeltaTime);
	SceneTransitionManager::getInstance()->update();
	GameObjectManager::getInstance()->cleanUpDeletedObjects();
}
