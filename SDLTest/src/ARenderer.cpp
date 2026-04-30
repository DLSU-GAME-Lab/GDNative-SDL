#include "ARenderer.h"
#include "Settings.h"

ARenderer::ARenderer(const std::string& strName) : AComponent(strName, ComponentType::RENDERER)
{
	this->pRenderer = NULL;
	this->mColor = { 255, 255, 255, 255 };
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

void ARenderer::setColor(SDL_Color color)
{
	this->mColor = color;
}

SDL_Color ARenderer::getColor() const
{
	return this->mColor;
}
