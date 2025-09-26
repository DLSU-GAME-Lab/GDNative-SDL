#include "GemSwapper.h"
#include "GemManager.h"

GemSwapper::GemSwapper(ButtonInput* pInput) : AComponent("GemSwapper", ComponentType::SCRIPT)
{
	this->pInput = pInput;
}

GemSwapper::~GemSwapper()
{

}

void GemSwapper::onAttach()
{

}

void GemSwapper::perform()
{
	if (pInput->getClicked())
	{
		pInput->setClicked(false);
		GemManager::getInstance()->setSelected((Gem*)this->pOwner);
	}
}
