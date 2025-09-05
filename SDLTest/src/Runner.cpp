#include "Runner.h"
#include "algorithm"

#include "TextureManager.h"
#include "SpriteRendererSystem.h"
#include "GameObjectManager.h"
#include "SceneManager.h"

#include "LobbyScene.h"

Runner::Runner()
{
	int screenWidth, screenHeight;
	SDL_DisplayID dispID = SDL_GetPrimaryDisplay();
	SDL_DisplayMode dispMode = *SDL_GetDesktopDisplayMode(dispID);
	screenWidth = dispMode.w;
	screenHeight = dispMode.h;

	float scaleX = (float)screenWidth / gameWidth;
	float scaleY = (float)screenHeight / gameHeight;
	float scale = std::min(scaleX, scaleY);

	int windowHeight = gameHeight * scale;
	int windowWidth = gameWidth * scale;

	pWindow = SDL_CreateWindow("Babaylan Tales", windowWidth, windowHeight,SDL_WINDOW_RESIZABLE);

	this->pRenderer = SDL_CreateRenderer(this->pWindow, NULL);
	if (this->pRenderer == NULL)
	{
		printf("ERROR");
	}
	SDL_SetRenderLogicalPresentation(this->pRenderer, windowHeight, screenHeight, SDL_LOGICAL_PRESENTATION_DISABLED);

	pMainEvent = new SDL_Event();

	//initialize systems
	TextureManager::initialize(this->pRenderer);
	SpriteRendererSystem::initialize(this->pRenderer);
	GameObjectManager::initialize();
	SceneManager::initialize();

	//load scene
	LobbyScene* pLobbyScene = new LobbyScene();
	SceneManager::getInstance()->registerScene((AScene*)pLobbyScene);
	SceneManager::getInstance()->loadScene(SceneTag::LOBBY_SCENE);

	//GameObjectManager::getInstance()->addObject(NULL);
}

Runner::~Runner()
{
	SceneManager::getInstance()->unloadScene();

	//destroy systems
	SceneManager::destroy();
	GameObjectManager::destroy();
	SpriteRendererSystem::destroy();
	TextureManager::destroy();

	delete this->pMainEvent;
	SDL_DestroyRenderer(this->pRenderer);
	SDL_DestroyWindow(this->pWindow);
}

void Runner::run()
{
	bool running = true;

	// pick a clear color once (optional)
	// clear the screen to black (RGB = 0,0,0, fully opaque) before drawing sprites.
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

	while (running)
	{
		// process all pending events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_QUIT)
				running = false;
		}

		// clear backbuffer
		SDL_RenderClear(pRenderer);

		// per-frame logic
		this->update();
		SceneManager::getInstance()->checkLoadScene();

		// draw all sprites
		this->render();

		SDL_RenderPresent(pRenderer);
	}
}


void Runner::processEvents()
{
	//TODO: create input system
}

void Runner::update()
{
	GameObjectManager::getInstance()->update();
}

void Runner::render()
{
	SpriteRendererSystem::getInstance()->draw();
}
