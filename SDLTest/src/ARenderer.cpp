#include "ARenderer.h"
#include "Settings.h"

ARenderer::ARenderer(const std::string& strName) : AComponent(strName, ComponentType::RENDERER)
{
	this->pRenderer = NULL;
}

SDL_Renderer* ARenderer::getSDLRenderer()
{
	return this->pRenderer;
}

void ARenderer::setSDLRenderer(SDL_Renderer* pRenderer)
{
	this->pRenderer = pRenderer;
}

bool ARenderer::inCameraView(SDL_FRect spriteRect)
{
	SDL_FRect camView = { 0, 0, gameWidth, gameHeight };
	
	return SDL_HasRectIntersectionFloat(&camView, &spriteRect);
}

SDL_FRect ARenderer::getRect() const
{
	return { 0, 0, 0, 0 };
}
