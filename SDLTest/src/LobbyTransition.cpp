#include "LobbyTransition.h"
#include "LobbyPlayer.h"
#include "GameObjectManager.h"

LobbyTransition::LobbyTransition(bool bRight) : AComponent("LobbyTransition", ComponentType::SCRIPT)
{
	this->pInput = NULL;
	this->bRight = bRight;
}

void LobbyTransition::onAttach()
{
	this->pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	this->pPlayer = (LobbyPlayer*)GameObjectManager::getInstance()->findObjectByName("LobbyPlayer");
}

void LobbyTransition::perform()
{
	if (pInput && pInput->getClicked())
	{
		std::cout << "[Lobby Transition]: click!\n";
		pInput->setClicked(false);
		if (this->bRight)
		{
			this->pPlayer->goRight();
		}
		else
		{
			this->pPlayer->goLeft();
		}
	}
}
