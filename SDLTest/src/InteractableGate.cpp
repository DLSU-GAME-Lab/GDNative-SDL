#include "InteractableGate.h"
#include "EventBroadcaster.h"
InteractableGate::InteractableGate() : AInteractable("InteractableGate")
{

}

void InteractableGate::onInteract()
{
	std::cout << "Opening Gate GUI" << std::endl;
	EventBroadcaster::getInstance()->broadcast(EventKey::GEM_INPUT_SCREEN);
}
