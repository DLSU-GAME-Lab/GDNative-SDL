#define EDITOR_MODE __has_include("EditorModule.h") && 1

#if EDITOR_MODE
#include "EditorModule.h"
#endif

#include "Runner.h"
#include "algorithm"

#include "EngineTime.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "AGameObject.h"
#include "EnumSceneTag.h"
#include "Settings.h"
#include "UIManager.h"
#include "SceneTransitionManager.h" 

// scenes
#include "LobbyScene.h"
#include "Title_Scene.h"
#include "RightRoomScene.h"
#include "LeftRoomScene.h"

Runner::Runner()
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
	}

	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello World",
		"!! Your SDL project successfully runs on Android !!", NULL) == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_ShowSimpleMessageBox failed (%s)", SDL_GetError());
	}

	std::cout << "|===========================================|\n";
	std::cout << "|----------| SDL VERSION: " <<
		SDL_MAJOR_VERSION << "." <<
		SDL_MINOR_VERSION <<"." <<
		SDL_MICRO_VERSION << " |----------|\n";
	std::cout << "|===========================================|\n";

	int screenWidth, screenHeight;
	SDL_DisplayID dispID = SDL_GetPrimaryDisplay();
	SDL_DisplayMode dispMode = *SDL_GetDesktopDisplayMode(dispID);
	screenWidth = dispMode.w;
	screenHeight = dispMode.h;

	// debug
	std::cout << "Screen size: " << screenWidth << "x" << screenHeight << std::endl;

	this->strWindowTitle = "Babaylan Tales";
	float scaleX = (float)screenWidth / gameWidth;
	float scaleY = (float)screenHeight / gameHeight;
	float scale = std::min(scaleX, scaleY);

	int windowHeight = gameHeight * scale;
	int windowWidth = gameWidth * scale;

	pWindow = SDL_CreateWindow(strWindowTitle.c_str(), windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);

	this->pRenderer = SDL_CreateRenderer(this->pWindow, NULL);
	if (this->pRenderer == NULL)
	{
		printf("ERROR");
	}
	SDL_SetRenderLogicalPresentation(this->pRenderer, gameWidth, gameHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	//initialize systems
	EngineTime::initialize();
	GameObjectManager::initialize();
	SceneManager::initialize();
	TextureManager::initialize(this->pRenderer);
	RenderSystem::initialize();
	SceneTransitionManager::initialize();
	RenderSystem::getInstance()->updateWindowSize(this->pWindow);

#if EDITOR_MODE
	EditorModule::initialize(this->pWindow, this->pRenderer);
#endif

	this->registerScenes();
}

Runner::~Runner()
{
	SceneManager::getInstance()->unloadScene();

	//destroy systems
#if EDITOR_MODE
	EditorModule::destroy();
#endif

	TextureManager::destroy();
	RenderSystem::destroy();
	SceneManager::destroy();
	GameObjectManager::destroy();
	EngineTime::destroy();
	SceneTransitionManager::destroy();

	SDL_DestroyRenderer(this->pRenderer);
	SDL_DestroyWindow(this->pWindow);
}

void Runner::run()
{
	bool running = true;

	while (running)
	{
		EngineTime::getInstance()->logFrame();

		// process all pending events
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
#if EDITOR_MODE
			EditorModule::getInstance()->processEditorInput(&e);
#endif

			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				RenderSystem::getInstance()->updateWindowSize(this->pWindow);
				break;

			default:
#if EDITOR_MODE
#else
				this->processEvents(&e);
#endif
				break;
			}
		}

#if EDITOR_MODE
		EditorModule::getInstance()->updateGameObjects();
#else
		this->update();
#endif
		
		this->render();

		// scene loading now handled safely after transition
		if (!SceneTransitionManager::getInstance()->isTransitioning())
			SceneManager::getInstance()->checkLoadScene();

		Uint64 frameTime = SDL_GetTicks() - EngineTime::getInstance()->tStart;
		if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
}


void Runner::processEvents(SDL_Event* eEvent)
{
	GameObjectManager::getInstance()->processInput(eEvent);
}

void Runner::update()
{
	GameObjectManager::getInstance()->update();
	SceneTransitionManager::getInstance()->update();
}

void Runner::render()
{
	// pick a clear color once (optional)
	// clear the screen to black (RGB = 0,0,0, fully opaque) before drawing sprites.
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);

	RenderSystem::getInstance()->draw(this->pRenderer);

#if EDITOR_MODE
	EditorModule::getInstance()->drawEditor(this->pRenderer);
#endif

	// draw fade/transition overlay last
	SceneTransitionManager::getInstance()->draw(this->pRenderer);

	SDL_RenderPresent(this->pRenderer);
}

void Runner::registerScenes()
{
	auto titleScene = new Title_Scene();
	auto lobbyScene = new LobbyScene();
	auto rightRoomScene = new RightRoomScene();
	auto leftRoomScene = new LeftRoomScene();

	SceneManager::getInstance()->registerScene(titleScene);
	SceneManager::getInstance()->registerScene(lobbyScene);
	SceneManager::getInstance()->registerScene(rightRoomScene);
	SceneManager::getInstance()->registerScene(leftRoomScene);

	//load initial scene
	SceneManager::getInstance()->loadScene(SceneTag::TITLE_SCENE);

}
