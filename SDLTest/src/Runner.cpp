// ---------------------------------------------------------------------------
// Responsibilities: initialize SDL, systems, run main loop (event -> update -> render),
// and manage timing/frame limiting.
// Comments below state Big-O, dominating terms as n increases, and potential
// runtime spikes when certain operations are invoked.
// ---------------------------------------------------------------------------
#include "EditorMode.h"
#if EDITOR_MODE
#include "EditorModule.h"
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#include "Runner.h"
#include "algorithm"

#include "TextureManager.h"
#include "CameraManager.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "AGameObject.h"
#include "EnumSceneTag.h"
#include "Settings.h"
#include "UIManager.h"
#include "SceneTransitionManager.h" 
#include "FontManager.h"
#include "AudioManager.h"
#include "DataAssetManager.h"
#include "EventBroadcaster.h"

// metrics
#include "MetricsManager.h"
#include "imgui.h" 
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <iostream>
#include <streambuf>

// scenes
#include "LobbyScene.h"
#include "Title_Scene.h"
#include "RightRoomScene.h"
#include "LeftRoomScene.h"
#include "PlatformerLevel1Scene.h"
#include "PuzzleLevel1Scene.h"
#include "RendererContext.h"

// Constructor / Destructor: startup and teardown costs are not per-frame.
// - Startup work: O(1) for SDL init and renderer creation, plus O(S) for
// registering S scenes in registerScenes() (startup-time growth with S).
// - These do not affect steady-state per-frame complexity except by
// increasing sizes of G/R/M at runtime if more content is created.
Runner::Runner()
{
#if defined(__ANDROID__)
    class AndroidLogBuf : public std::streambuf {
    protected:
        int overflow(int c) override {
            if (c == '\n') {
                __android_log_write(ANDROID_LOG_INFO, "STDOUT", buffer.c_str());
                buffer.clear();
            } else {
                buffer += (char)c;
            }
            return c;
        }
    private:
        std::string buffer;
    };

    static AndroidLogBuf androidLogBuf;
    std::cout.rdbuf(&androidLogBuf);
#endif

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
	}

	std::cout << "|===========================================|\n";
	std::cout << "|----------| SDL VERSION: " <<
		SDL_MAJOR_VERSION << "." <<
		SDL_MINOR_VERSION <<"." <<
		SDL_MICRO_VERSION << " |----------|\n";
	std::cout << "|===========================================|\n";

    // --- window / renderer creation ---
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

    int windowWidth = (int)(gameWidth * scale);
    int windowHeight = (int)(gameHeight * scale);

    pWindow = SDL_CreateWindow(
            strWindowTitle.c_str(),
            windowWidth,
            windowHeight,
            SDL_WINDOW_FULLSCREEN
    );

    pRenderer = SDL_CreateRenderer(pWindow, NULL);
    if (!pRenderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Failed to create renderer: %s",
                     SDL_GetError());
    }

    SDL_SetRenderLogicalPresentation(
            pRenderer,
            (int)gameWidth,
            (int)gameHeight,
            SDL_LOGICAL_PRESENTATION_LETTERBOX
    );

	//initialize systems
	std::cout << "[Runner] Initializing systems..." << std::endl;
	GameObjectManager::initialize();
	SceneManager::initialize();
	InputManager::initialize();
	TextureManager::initialize(this->pRenderer);
	CameraManager::initialize();
	RendererContext::initialize(this->pRenderer);
	// setWindowSize likely O(1) or O(#render targets)
	CameraManager::getInstance()->setWindowSize(this->pWindow);
    CameraManager::getInstance()->setWindowScale(scaleX, scaleY);
    SceneTransitionManager::initialize();
	FontManager::initialize();
	AudioManager::initialize();
	DataAssetManager::initialize();
	EventBroadcaster::initialize();
	UIManager::initialize(this->pWindow, this->pRenderer);
	std::cout << "[Runner] Initializing MetricsManager..." << std::endl;
	MetricsManager::initialize();
	std::cout << "[Runner] MetricsManager initialized." << std::endl;
#if EDITOR_MODE
	Editor::EditorModule::initialize();
#endif

	this->registerScenes();
}

// ------- DESTRUCTOR -------
Runner::~Runner()
{
	SceneManager::getInstance()->unloadScene();

	//destroy systems
#if EDITOR_MODE
	Editor::EditorModule::destroy();
#endif
	DataAssetManager::destroy();
	AudioManager::destroy();
	UIManager::destroy();
	TextureManager::destroy();
	CameraManager::destroy();
	InputManager::destroy();
	SceneManager::destroy();
	GameObjectManager::destroy();
	SceneTransitionManager::destroy();
	MetricsManager::destroy();
	RendererContext::destroy();
	EventBroadcaster::destroy();
	SDL_DestroyRenderer(this->pRenderer);
	SDL_DestroyWindow(this->pWindow);
}


// -----------------------
// MAIN LOOP: run()
// -----------------------
// Notation used below:
// - E = number of SDL events per frame
// - G = number of game objects
// - R = number of renderable objects (<= G)
// - M = number of GUI/metrics items drawn per frame
// - S = number of registered sprite renderers
// - V = number of textures in global texture vector
// - T = number of textures associated with a single name
// - load_work = cost of scene resource loading when triggered
// Overall per-frame complexity expressed as:
// cost(frame) = O(E * costPerEvent + G * costUpdate + R * costDraw + M + C)
// where C represents any heavy subsystem costs (collision checks, pathfinding,
// or blocking resource loads). As n increases, dominant terms will be those
// that scale with G, R, S, V, or E depending on system behavior.
void Runner::run()
{
	bool running = true;
	Uint64 lastTime = 0;
	Uint64 currentTime = 0;

	while (running)
	{
		// ---------- EVENT PROCESSING LOOP ----------
		// Complexity: O(E * costPerEvent)
		// - SDL_PollEvent yields E events this frame
		// - costPerEvent includes: forwarding to ImGui (O(1)), Editor processing (O(1) or more),
		// and this->processEvents(&e) -> GameObjectManager::processInput(e).
		// - If processInput scans all game objects to find listeners, costPerEvent = O(G)
		// so entire event loop can become O(E * G).
		
		// process all pending events
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			UIManager::getInstance()->processEvent(&e);
// Editor mode input may have its own costs (dependent on editor internals)
#if EDITOR_MODE
			Editor::EditorModule::getInstance()->processEditorInput(&e);
#endif

			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				running = false; // constant-time action
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				// setWindowSize likely O(1) or O(#render targets)
				CameraManager::getInstance()->setWindowSize(this->pWindow);
				break;

			default:
#if EDITOR_MODE
#else
				// processEvents delegates to GameObjectManager::processInput which is O(G)
				// This line is where per-event scanning becomes expensive if events are many.
				this->processEvents(&e);
#endif
				break;
			}

			// Metrics: record input event -> O(1)
			// --- input lag tracking ---
			// record input lag when user presses a key or clicks mouse
			if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				MetricsManager::getInstance()->recordInputEvent();
			}
		}

		// ---------- TIMING ----------
		// constant-time arithmetic
		lastTime = currentTime;
		currentTime = SDL_GetTicks();
		float fDeltaTime = (currentTime - lastTime) / 1000.0f;

		// ---------- UPDATE PHASE ----------
		// GameObjectManager::update -> typically O(G)
		// SceneTransitionManager::update -> O(1)
		// If update contains collision checks, pathfinding, or nested loops, costs escalate
#if EDITOR_MODE
		Editor::EditorModule::getInstance()->updateGameObjects(fDeltaTime);
#else
		this->update(fDeltaTime);
#endif

		// Metrics: mark input handled and update internal metrics (O(1) to O(M))
		// mark input as visually handled this frame 
		MetricsManager::getInstance()->markInputHandled();
		MetricsManager::getInstance()->update();

		// ---------- RENDER PHASE ----------
		// GameObjectManager::draw -> loops over R renderable objects => O(R)
		// ImGui rendering: NewFrame() O(1), Metrics draw O(M), RenderDrawData O(I)
		this->render();

		// scene loading check -> O(1)
		// scene loading now handled safely after transition
		if (!SceneTransitionManager::getInstance()->isTransitioning())
			SceneManager::getInstance()->checkLoadScene();

		// frame delay / sleep -> O(1) (affects timing, not algorithmic complexity)
		Uint64 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < frameDelay) SDL_Delay(frameDelay - frameTime);
	}
}

// processEvents: delegates to GameObjectManager::processInput
// - Complexity per call: O(G) (it iterates over all game objects).
// - If called for each event, event-processing cost per frame scales as O(E*G).
void Runner::processEvents(SDL_Event* eEvent)
{
	InputManager::getInstance()->processEvents(eEvent);
	GameObjectManager::getInstance()->processInput(eEvent);
}

// update: delegates to GameObjectManager::update and SceneTransitionManager::update
// - GameObjectManager::update is O(G) per call (one update per object).
// - SceneTransitionManager::update is O(1) per call.
// - If updates include pairwise operations (e.g., naive collisions), update
// can become O(G^2). Thus as G grows, update cost may transition from
// linear to quadratic depending on object internals.
void Runner::update(float fDeltaTime)
{
	GameObjectManager::getInstance()->update(fDeltaTime);
	AudioManager::getInstance()->update();
	SceneTransitionManager::getInstance()->update();
	GameObjectManager::getInstance()->cleanUpDeletedObjects();
}

// render: clears, draws, and presents
// - GameObjectManager::draw -> O(R) per frame (draw each renderable once).
// - ImGui/metrics work -> O(M) per frame.
// - SDL_RenderPresent and texture draw calls are O(1) algorithmically but
// can dominate wall-clock time (expensive constants) especially as R grows.
void Runner::render()
{
	// pick a clear color once (optional)
	// clear the screen to black (RGB = 0,0,0, fully opaque) before drawing sprites.
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255); // O(1)
	SDL_RenderClear(pRenderer); // O(1)

	UIManager::getInstance()->newFrame();
	GameObjectManager::getInstance()->draw(this->pRenderer); // O(R)

#if EDITOR_MODE
		// Only compile this call when EditorModule.h was detected and included.
		Editor::EditorModule::getInstance()->drawEditor(this->pRenderer); // editor draw cost
#endif

	// draw fade/transition overlay last
	SceneTransitionManager::getInstance()->draw(this->pRenderer); // O(1)

	MetricsManager::getInstance()->drawGUI();
	UIManager::getInstance()->drawAllUI(this->pRenderer);

	// present the frame. This call may block until GPU / vsync flush depending
	// on renderer configuration - it's expensive in time but constant in ops.
	SDL_RenderPresent(this->pRenderer); // O(1) but expensive in time
}

// registerScenes: startup cost O(S) where S = number of scenes registered
void Runner::registerScenes()
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

	// --- load time tracking --- 
	// // start timer before load, end after load completes 
	MetricsManager::getInstance()->startLoadTimer(); 

	//load initial scene
	SceneManager::getInstance()->loadScene(SceneTag::TITLE_SCENE); 
	
	MetricsManager::getInstance()->endLoadTimer();
}