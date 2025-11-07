#include "GUIToggle.h"
#include "GameObjectManager.h"
#include "ButtonInput.h"


GUIToggle::GUIToggle(EventKey EKey): AComponent("GUIToggle", ComponentType::SCRIPT)
{
	this->EKey = EKey;
}

GUIToggle::~GUIToggle()
{

}

void GUIToggle::perform()
{
	ButtonInput* pInput = (ButtonInput*)pOwner->findComponentByName("ButtonInput");
	if (pInput && pInput->getClicked())
	{
		std::cout << "Click" << std::endl;

		pInput->setClicked(false);
		std::unordered_map <std::string, void*> mapParam;
		std::string strName = pOwner->getName();
		mapParam["Sender"] = static_cast<void*>(&strName);
		EventBroadcaster::getInstance()->broadcast(this->EKey, mapParam);
	}
}
