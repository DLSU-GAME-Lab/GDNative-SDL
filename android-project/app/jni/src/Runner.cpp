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

	// debug
	std::cout << "Screen size: " << screenWidth << "x" << screenHeight << std::endl;

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
    spdlog::info("Entered Constructor");

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
    spdlog::info("Exit Constructor");

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
        spdlog::info("Entered Run");

		EngineTime::getInstance()->logFrameStart();
        spdlog::info("after engine time");

		//TODO: Remove additional time stuff. We have an EngineTime class for that.
		uint64_t currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;
		
		// process all pending events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_EVENT_QUIT)
				running = false;
			else this->processEvents(e);
		}

		// clear backbuffer
		SDL_RenderClear(pRenderer);

		// per-frame logic
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
        spdlog::info("dead end");

	}
}


void Runner::processEvents(SDL_Event eEvent)
{
	if (eEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
		eEvent.type == SDL_EVENT_MOUSE_BUTTON_UP ||
		eEvent.type == SDL_EVENT_MOUSE_MOTION)
	{
		int rawX = eEvent.button.x;
		int rawY = eEvent.button.y;

		// convert window coordinates to logical coordinates
		float logicalX, logicalY;
		SDL_RenderCoordinatesFromWindow(pRenderer, rawX, rawY, &logicalX, &logicalY);

		// update the event with logical coordinates
		if (eEvent.type == SDL_EVENT_MOUSE_MOTION) {
			eEvent.motion.x = logicalX;
			eEvent.motion.y = logicalY;
		}
		else {
			eEvent.button.x = logicalX;
			eEvent.button.y = logicalY;
		}

		// log converted mouse input
		std::cout << "[Converted Mouse] Logical: (" << logicalX << ", " << logicalY << ")"
			<< " Type: " << eEvent.type
			<< std::endl;
	}

	// pass input to current scene
	GameObjectManager::getInstance()->processInput(eEvent);
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
