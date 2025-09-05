#include "Runner.h"

Runner::Runner()
{
	int screenWidth, screenHeight;
	SDL_DisplayID dispID = SDL_GetPrimaryDisplay();
	SDL_DisplayMode dispMode = *SDL_GetDesktopDisplayMode(dispID);
	screenWidth = dispMode.w;
	screenHeight = dispMode.h;
	//should have a game width and game height to scale the window
	// give them here ->
	int windowHeight = screenHeight / 2;
	int windowWidth = screenWidth / 2;

	window = SDL_CreateWindow("Babaylan Tales", windowWidth, windowHeight,SDL_WINDOW_RESIZABLE);

	this->renderer = SDL_CreateRenderer(this->window, "Babaylan Tales");
	SDL_SetRenderLogicalPresentation(this->renderer, windowHeight, screenHeight, SDL_LOGICAL_PRESENTATION_DISABLED);

	mainEvent = new SDL_Event();
}

Runner::~Runner()
{
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

}
