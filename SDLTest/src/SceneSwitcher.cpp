#include "SceneSwitcher.h"
#include "AGameObject.h"
#include "SceneTransitionManager.h"

SceneSwitcher::SceneSwitcher(SceneTag ETag) : AComponent("SceneSwitcher", ComponentType::SCRIPT)
{
	this->ETag = ETag;
	this->pInput = NULL;
}

SceneSwitcher::~SceneSwitcher()
{

}

void SceneSwitcher::onAttach()
{
	this->pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
}

void SceneSwitcher::perform()
{
	if (pInput->getClicked())
	{
		pInput->setClicked(false);
		SceneTransitionManager::getInstance()->requestTransition(this->ETag, TransitionType::FADE);
	}
}
