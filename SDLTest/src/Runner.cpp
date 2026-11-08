// ---------------------------------------------------------------------------
// Responsibilities: initialize SDL, run the main loop (event -> update -> render),
// and manage timing/frame limiting. Identical in Babaylan Tayles and Gem Hunter
// Match: the window, the systems, the scenes and the input/update routing come
// from the game's AGame subclass.
// Comments below state Big-O, dominating terms as n increases, and potential
// runtime spikes when certain operations are invoked.
// ---------------------------------------------------------------------------

// Editor mode: the game's EditorMode.h when it has one, otherwise detected here.
#if defined(__has_include)
#if __has_include("EditorMode.h")
#include "EditorMode.h"
#endif
#endif

#ifndef EDITOR_MODE
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
#endif

#if EDITOR_MODE
#include "EditorModule.h"
#endif

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#include "Runner.h"
#include "AGame.h"
#include "Settings.h"
#include "SceneManager.h"
#include "SceneTransitionManager.h"
#include "UIManager.h"
#include "MetricsManager.h"
#include "GameObjectManager.h"
#include <iostream>
#include <streambuf>

// Constructor / Destructor: startup and teardown costs are not per-frame.
// - Startup work: O(1) for SDL init and renderer creation, plus whatever the
// game's setup does (systems, O(S) for registering S scenes).
// - These do not affect steady-state per-frame complexity except by
// increasing sizes of G/R/M at runtime if more content is created.
Runner::Runner()
{
#if defined(__ANDROID__)
	// send std::cout to logcat
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
		SDL_MINOR_VERSION << "." <<
		SDL_MICRO_VERSION << " |----------|\n";
	std::cout << "|===========================================|\n";

	{
		int rv = SDL_GetVersion();
		SDL_Log("SDL runtime: %d.%d.%d", SDL_VERSIONNUM_MAJOR(rv), SDL_VERSIONNUM_MINOR(rv), SDL_VERSIONNUM_MICRO(rv));
	}

	this->pGame = AGame::create();

	// --- Create Window ---
	pWindow = this->pGame->createWindow();
	if (!pWindow)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow failed: %s", SDL_GetError());
	}
	else
	{
		int windowWidth, windowHeight;
		SDL_GetWindowSize(pWindow, &windowWidth, &windowHeight);
		SDL_Log("Window created: %dx%d", windowWidth, windowHeight);
	}

	// --- Create Renderer ---
	pRenderer = this->pGame->createRenderer(pWindow);
	if (!pRenderer)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateRenderer failed: %s", SDL_GetError());
	}
	else
	{
		SDL_Log("Renderer created successfully");
	}

	// --- Logical presentation / camera ---
	this->pGame->setupPresentation(pWindow, pRenderer);

	//initialize systems
	std::cout << "[Runner] Initializing systems..." << std::endl;
	this->pGame->initializeSystems(pWindow, pRenderer);
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
	this->pGame->destroySystems();
	delete this->pGame;
	this->pGame = nullptr;

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
		// and this->processEvents(&e) -> the game's input routing (GameObjectManager::processInput).
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

			// the game sees every event first (resize, touch logging, ...)
			this->pGame->onEvent(&e);

			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				running = false; // constant-time action
				break;

			case SDL_EVENT_WINDOW_RESIZED:
				// handled by the game's onEvent, not passed on as game input
				break;

			default:
#if EDITOR_MODE
#else
				// processEvents delegates to the game's input routing which is O(G)
				// This line is where per-event scanning becomes expensive if events are many.
				this->processEvents(&e);
#endif
				break;
			}

			// Metrics: record input event -> O(1)
			// --- input lag tracking ---
			// record input lag when user presses a key or clicks mouse
			if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			{
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

// processEvents: delegates to the game's input routing
// - Complexity per call: O(G) (GameObjectManager::processInput iterates over all game objects).
// - If called for each event, event-processing cost per frame scales as O(E*G).
void Runner::processEvents(SDL_Event* eEvent)
{
	this->pGame->processEvent(eEvent);
}

// update: delegates to the game's update (GameObjectManager::update, audio,
// SceneTransitionManager::update, clean up of deleted objects)
// - GameObjectManager::update is O(G) per call (one update per object).
// - SceneTransitionManager::update is O(1) per call.
// - If updates include pairwise operations (e.g., naive collisions), update
// can become O(G^2). Thus as G grows, update cost may transition from
// linear to quadratic depending on object internals.
void Runner::update(float fDeltaTime)
{
	this->pGame->update(fDeltaTime);
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

	MetricsManager::getInstance()->beginGPUFrame();
	// present the frame. This call may block until GPU / vsync flush depending
	// on renderer configuration - it's expensive in time but constant in ops.
	SDL_RenderPresent(this->pRenderer); // O(1) but expensive in time
	MetricsManager::getInstance()->endGPUFrame();
}

// registerScenes: startup cost O(S) where S = number of scenes registered
void Runner::registerScenes()
{
	this->pGame->registerScenes();

	// --- load time tracking --- 
	// // start timer before load, end after load completes 
	MetricsManager::getInstance()->startLoadTimer();

	//load initial scene
	SceneManager::getInstance()->loadScene(this->pGame->getStartScene());

	MetricsManager::getInstance()->endLoadTimer();
}
