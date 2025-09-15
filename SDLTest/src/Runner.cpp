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
#include "UIManager.h"

// scenes
#include "LobbyScene.h"
#include "Title_Scene.h"

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
	TextureManager::initialize(this->pRenderer);
	SpriteRendererSystem::initialize(this->pRenderer);
	GameObjectManager::initialize();
	SceneManager::initialize();
	UIManager::initialize(this->pWindow, this->pRenderer);

	this->registerScenes();
}

Runner::~Runner()
{
	SceneManager::getInstance()->unloadScene();

	//destroy systems
	UIManager::destroy();
	SceneManager::destroy();
	GameObjectManager::destroy();
	SpriteRendererSystem::destroy();
	TextureManager::destroy();
	EngineTime::destroy();

	SDL_DestroyRenderer(this->pRenderer);
	SDL_DestroyWindow(this->pWindow);
}

void Runner::run()
{
	bool running = true;
	uint64_t lastTime = SDL_GetTicks();

	while (running)
	{
		EngineTime::getInstance()->logFrameStart();

		//TODO: Remove additional time stuff. We have an EngineTime class for that.
		uint64_t currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		// process all pending events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			UIManager::getInstance()->processEvent(&e);
			if (e.type == SDL_EVENT_QUIT)
				running = false;
			else this->processEvents(e);
		}

		// per-frame logic
		this->update();
		SceneManager::getInstance()->checkLoadScene();

		this->render();

		Uint64 frameTime = SDL_GetTicks() - EngineTime::getInstance()->tStart;
		if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
		EngineTime::getInstance()->logFrameEnd();
	}
}


void Runner::processEvents(SDL_Event eEvent)
{
	GameObjectManager::getInstance()->processInput(eEvent);
}

void Runner::update()
{
	GameObjectManager::getInstance()->update();
}

void Runner::render()
{
	// pick a clear color once (optional)
	// clear the screen to black (RGB = 0,0,0, fully opaque) before drawing sprites.
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);

	SpriteRendererSystem::getInstance()->draw();

	UIManager::getInstance()->newFrame();
	UIManager::getInstance()->drawAllUI(pRenderer);

	ImGuiIO io = ImGui::GetIO();
	SDL_SetRenderScale(this->pRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

	SDL_RenderPresent(this->pRenderer);
}

void Runner::registerScenes()
{
	auto titleScene = new Title_Scene();
	auto lobbyScene = new LobbyScene();

	SceneManager::getInstance()->registerScene(titleScene);
	SceneManager::getInstance()->registerScene(lobbyScene);

	//load initial scene
	SceneManager::getInstance()->loadScene(SceneTag::TITLE_SCENE);

}
