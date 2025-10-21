#include "PuzzleToken.h"
#include "SpriteRenderer.h"
#include "ButtonInput.h"
#include "TokenController.h"

PuzzleToken::PuzzleToken(std::string strName, Vector2D position, Vector2D scale) : AGameObject(strName)
{
	this->fVecTranslate = position;
	this->fVecScale = scale;
}

void PuzzleToken::initialize()
{
	SpriteRenderer* pRenderer = new SpriteRenderer(strName);
	ButtonInput* pInput = new ButtonInput(pRenderer);
	TokenController* pController = new TokenController(pInput);

	this->attachComponent(pRenderer);
	this->attachComponent(pInput);
	this->attachComponent(pController);
}
