// Detect EditorModule.h
#if defined(__has_include)
	#if __has_include("EditorModule.h")
		#define EDITOR_MODULE_AVAILABLE 1
	#else
		#define EDITOR_MODULE_AVAILABLE 0
	#endif
#else
	#define EDITOR_MODULE_AVAILABLE 0
#endif

// Toggle editor mode (set to 1 to enable, 0 to disable)
#define EDITOR_MODE (EDITOR_MODULE_AVAILABLE && 0)   // <- put 1 or 0 here

#if EDITOR_MODE
#include "EditorModule.h"
#endif

#include "Runner.h"
#include "algorithm"

#include "TextureManager.h"
#include "RenderSystem.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "AGameObject.h"
#include "EnumSceneTag.h"
#include "Settings.h"
#include "UIManager.h"
#include "SceneTransitionManager.h" 
#include "FontManager.h"

#ifdef __ANDROID__
// android logging
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"
#endif

// metrics
#include "MetricsManager.h"
#include "imgui.h" 
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

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
	float scale = (std::min)(scaleX, scaleY);

	int windowHeight = gameHeight * scale;
	int windowWidth = gameWidth * scale;

    pWindow = SDL_CreateWindow(
            strWindowTitle.c_str(),
            0, 0,
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE
    );

	this->pRenderer = SDL_CreateRenderer(this->pWindow, NULL);
	if (this->pRenderer == NULL)
	{
		printf("ERROR");
	}
	SDL_SetRenderLogicalPresentation(this->pRenderer, gameWidth, gameHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

#ifdef __ANDROID__
	auto android_logger = spdlog::android_logger_mt("android", "BabaylanLog");
	spdlog::set_default_logger(android_logger);
	spdlog::set_level(spdlog::level::debug); // show debug+ logs
	spdlog::debug("spdlog initialized on Android");
#else
	// just use SDL_Log or std::cout on desktop
	SDL_Log("Logging initialized on desktop (no spdlog)");
#endif

	//initialize systems
	std::cout << "[Runner] Initializing systems..." << std::endl;
	GameObjectManager::initialize();
	SceneManager::initialize();
	TextureManager::initialize(this->pRenderer);
	RenderSystem::initialize();
	SceneTransitionManager::initialize();
	FontManager::initialize();
	RenderSystem::getInstance()->updateWindowSize(this->pWindow, this->pRenderer);
	std::cout << "[Runner] Initializing MetricsManager..." << std::endl;
	MetricsManager::initialize();
	std::cout << "[Runner] MetricsManager initialized." << std::endl;

#if !EDITOR_MODE
	std::cout << "[Runner] Initializing ImGui (runner-owned)..." << std::endl;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(this->pWindow, this->pRenderer);
	ImGui_ImplSDLRenderer3_Init(this->pRenderer);
#else
	std::cout << "[Runner] Editor mode active: UIManager should own ImGui." << std::endl;
#endif

#if EDITOR_MODE
	Editor::EditorModule::initialize(this->pWindow, this->pRenderer);
#endif

	this->registerScenes();
}

Runner::~Runner()
{
	SceneManager::getInstance()->unloadScene();

#if !EDITOR_MODE
	// runner only shuts down what it initialized.
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
#endif

	//destroy systems
#if EDITOR_MODE
	Editor::EditorModule::destroy();
#endif

	TextureManager::destroy();
	RenderSystem::destroy();
	SceneManager::destroy();
	GameObjectManager::destroy();
	SceneTransitionManager::destroy();
	MetricsManager::destroy();

	SDL_DestroyRenderer(this->pRenderer);
	SDL_DestroyWindow(this->pWindow);
}

void Runner::run()
{
	bool running = true;
	Uint64 lastTime = 0;
	Uint64 currentTime = 0;

	while (running)
	{
		// process all pending events
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
            // 1) Convert event coordinates to renderer/logical coordinates first
            if (this->pRenderer) {
                // if available this converts motion/touch coords to the renderer's logical space
                if (SDL_ConvertEventToRenderCoordinates(this->pRenderer, &e) == 0) {
                    // success - e now contains render-space coords for mouse/touch
                } else {
                    // debug log if conversion fails (not fatal)
                    SDL_Log("SDL_ConvertEventToRenderCoordinates returned error: %s", SDL_GetError());
                }
            }

            // 2) Now forward the (converted) event to ImGui so it receives render/logical coords
            if (ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().BackendPlatformUserData != nullptr) {
                ImGui_ImplSDL3_ProcessEvent(&e);
            }

#if EDITOR_MODE
			Editor::EditorModule::getInstance()->processEditorInput(&e);
#endif

			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				RenderSystem::getInstance()->updateWindowSize(this->pWindow, this->pRenderer);
				break;

			default:
#if EDITOR_MODE
#else
				this->processEvents(&e);
#endif
				break;
			}
		}

		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		float fDeltaTime = (currentTime - lastTime) / 1000.0f;

#if EDITOR_MODE
		Editor::EditorModule::getInstance()->updateGameObjects(fDeltaTime);
#else
		this->update(fDeltaTime);
#endif

		MetricsManager::getInstance()->update();

		this->render();

		// scene loading now handled safely after transition
		if (!SceneTransitionManager::getInstance()->isTransitioning())
			SceneManager::getInstance()->checkLoadScene();

		Uint64 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
}


void Runner::processEvents(SDL_Event* eEvent)
{
    // If ImGui wants the mouse/keyboard, skip game input for those events
    if (ImGui::GetCurrentContext() != nullptr)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureMouse)
        {
            if (eEvent->type == SDL_EVENT_MOUSE_MOTION ||
                eEvent->type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                eEvent->type == SDL_EVENT_MOUSE_BUTTON_UP ||
                eEvent->type == SDL_EVENT_FINGER_DOWN ||
                eEvent->type == SDL_EVENT_FINGER_UP ||
                eEvent->type == SDL_EVENT_FINGER_MOTION)
            {
                return;
            }
        }
        if (io.WantCaptureKeyboard)
        {
            if (eEvent->type == SDL_EVENT_KEY_DOWN || eEvent->type == SDL_EVENT_KEY_UP || eEvent->type == SDL_EVENT_TEXT_INPUT)
            {
                return;
            }
        }
    }

    GameObjectManager::getInstance()->processInput(eEvent);
}

void Runner::update(float fDeltaTime)
{
	GameObjectManager::getInstance()->update(fDeltaTime);
	SceneTransitionManager::getInstance()->update();
}

void Runner::render()
{
	// pick a clear color once (optional)
	// clear the screen to black (RGB = 0,0,0, fully opaque) before drawing sprites.
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(pRenderer);

	GameObjectManager::getInstance()->draw(this->pRenderer);

    // before starting ImGui frame
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)gameWidth, (float)gameHeight);
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f); // logical->framebuffer scale is 1 after conversion
    }

#if !EDITOR_MODE
	// Runner owns ImGui: start frame, draw metrics, render
	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui::NewFrame();

	MetricsManager::getInstance()->drawGUI();

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->pRenderer);
#else
	#if EDITOR_MODE
		// Only compile this call when EditorModule.h was detected and included.
		Editor::EditorModule::getInstance()->drawEditor(this->pRenderer);
	#endif
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
