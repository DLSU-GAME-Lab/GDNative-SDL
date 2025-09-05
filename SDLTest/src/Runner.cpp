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
	this->lFPSTime = SDL_GetTicks();
	while (this->pMainEvent->type != SDL_EVENT_QUIT)
	{
		this->lFrameTime = SDL_GetTicks();
		SDL_PollEvent(this->pMainEvent);
		SDL_RenderClear(pRenderer);
		SDL_RenderFillRect(pRenderer, NULL);
		this->update();
		SceneManager::getInstance()->checkLoadScene();
		this->render();
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
