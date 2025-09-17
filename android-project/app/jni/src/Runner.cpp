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

    spdlog::info("Starting main game loop");

    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

    while (running)
    {
        spdlog::debug("--- Frame Start ---");

        EngineTime::getInstance()->logFrameStart();

        uint64_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        spdlog::debug("Delta time: {:.3f}", deltaTime);

        // Process events
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT) {
                spdlog::info("Quit event received");
                running = false;
            } else {
                this->processEvents(e);
            }
        }

        // Clear backbuffer
        SDL_RenderClear(pRenderer);

        // REMOVED: this->update(); // This was causing duplicate updates

        // Check for scene changes
        SceneManager::getInstance()->checkLoadScene();

        // Update scene (ONLY ONCE per frame)
        SceneManager::getInstance()->update(deltaTime);

        // REMOVED: GameObjectManager update - SceneManager should handle this

        // Render scene
        SceneManager::getInstance()->render(pRenderer);

        // Present renderer
        SDL_RenderPresent(pRenderer);

        // Frame timing
        Uint64 frameTime = SDL_GetTicks() - EngineTime::getInstance()->tStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }

        EngineTime::getInstance()->logFrameEnd();
        spdlog::debug("--- Frame End ---");
    }

    spdlog::info("Main game loop ended");
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
	//GameObjectManager::getInstance()->update();
	//EngineTime::getInstance()->logFrameEnd();
}

void Runner::render()
{
	SpriteRendererSystem::getInstance()->draw();
}
