#include "ARenderer.h"

ARenderer::ARenderer(const std::string& strName) : AComponent(strName, ComponentType::RENDERER)
{
	this->pRenderer = NULL;
}

void ARenderer::setSDLRenderer(SDL_Renderer* pRenderer)
{
	this->pRenderer = pRenderer;
}
