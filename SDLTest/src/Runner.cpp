#include "Runner.h"
#include "algorithm"

#include "EngineTime.h"
#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "AGameObject.h"
#include "EnumSceneTag.h"
#include "Settings.h"

// scenes
#include "LobbyScene.h"
#include "Title_Scene.h"

Runner::Runner()
{
	int screenWidth, screenHeight;
	SDL_DisplayID dispID = SDL_GetPrimaryDisplay();
	SDL_DisplayMode dispMode = *SDL_GetDesktopDisplayMode(dispID);
	screenWidth = dispMode.w;
	screenHeight = dispMode.h;

	this->fWindowScale = 0.75f;
	this->strWindowTitle = "Babaylan Tales";
	float scaleX = (float)screenWidth / gameWidth;
	float scaleY = (float)screenHeight / gameHeight;
	float scale = std::min(scaleX, scaleY);

	int windowHeight = gameHeight * scale * this->fWindowScale;
	int windowWidth = gameWidth * scale * this->fWindowScale;

	pWindow = SDL_CreateWindow(strWindowTitle.c_str(), windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);

	this->pRenderer = SDL_CreateRenderer(this->pWindow, NULL);
	if (this->pRenderer == NULL)
	{
		printf("ERROR");
	}
	SDL_SetRenderLogicalPresentation(this->pRenderer, gameWidth, gameHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	pMainEvent = new SDL_Event();

	//initialize systems
	EngineTime::initialize();
	TextureManager::initialize(this->pRenderer);
	SpriteRendererSystem::initialize(this->pRenderer);
	GameObjectManager::initialize();
	SceneManager::initialize();

	//register scene/s
	auto titleScene = std::make_unique<Title_Scene>();
	auto lobbyScene = std::make_unique<LobbyScene>();

	SceneManager::getInstance()->registerScene(SceneTag::TITLE_SCENE, std::move(titleScene));
	SceneManager::getInstance()->registerScene(SceneTag::LOBBY_SCENE, std::move(lobbyScene));

	//load initial scene
	SceneManager::getInstance()->loadScene(SceneTag::TITLE_SCENE);

	//GameObjectManager::getInstance()->addObject(NULL);
}

Runner::~Runner()
{
	SceneManager::getInstance()->unloadCurrentScene();

	//destroy systems
	SceneManager::destroy();
	GameObjectManager::destroy();
	SpriteRendererSystem::destroy();
	TextureManager::destroy();
	EngineTime::destroy();

	delete this->pMainEvent;
	SDL_DestroyRenderer(this->pRenderer);
	SDL_DestroyWindow(this->pWindow);
}

void Runner::run()
{
	bool running = true;
	uint64_t lastTime = SDL_GetTicks();

	// pick a clear color once (optional)
	// clear the screen to black (RGB = 0,0,0, fully opaque) before drawing sprites.
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

	while (running)
	{
		uint64_t currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;
		
		// process all pending events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_QUIT)
				running = false;

			// pass input to current scene
			GameObjectManager::getInstance()->processInput(e);
		}

		// clear backbuffer
		SDL_RenderClear(pRenderer);

		// per-frame logic
		EngineTime::getInstance()->logFrameStart();
		this->update();
		SceneManager::getInstance()->checkLoadScene();

		// update scene
		SceneManager::getInstance()->update(deltaTime);

		// render scene
		SceneManager::getInstance()->render(pRenderer);

		SDL_RenderPresent(pRenderer);
		Uint64 frameTime = SDL_GetTicks() - EngineTime::getInstance()->tStart;
		if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
		EngineTime::getInstance()->logFrameEnd();
	}
}


void Runner::processEvents()
{
	//TODO: create input system
}

void Runner::update()
{
	GameObjectManager::getInstance()->update();
	EngineTime::getInstance()->logFrameEnd();
}

void Runner::render()
{
	SpriteRendererSystem::getInstance()->draw();
}
