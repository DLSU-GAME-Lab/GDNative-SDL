#include "Runner.h"
#include "algorithm"
#include "SpriteRendererSystem.h"

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

	window = SDL_CreateWindow("Babaylan Tales", windowWidth, windowHeight,SDL_WINDOW_RESIZABLE);

	this->renderer = SDL_CreateRenderer(this->window, "Babaylan Tales");
	SDL_SetRenderLogicalPresentation(this->renderer, windowHeight, screenHeight, SDL_LOGICAL_PRESENTATION_DISABLED);

	mainEvent = new SDL_Event();

	//initialize systems
	SpriteRendererSystem::initialize();
}

Runner::~Runner()
{
	//destroy systems
	SpriteRendererSystem::destroy();

	delete this->mainEvent;
	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void Runner::run()
{
	this->lFPSTime = SDL_GetTicks();
	while (this->mainEvent->type != SDL_EVENT_QUIT)
	{
		this->lFrameTime = SDL_GetTicks();
		SDL_PollEvent(this->mainEvent);
		SDL_RenderClear(renderer);
		SDL_RenderFillRect(renderer, NULL);
		this->update();
		this->render();
	}
}

void Runner::processEvents()
{

}

void Runner::update()
{

}

void Runner::render()
{
	SpriteRendererSystem::getInstance()->draw();
}
