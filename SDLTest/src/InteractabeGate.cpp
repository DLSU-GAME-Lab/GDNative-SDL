#include "InteractabeGate.h"

InteractabeGate::InteractabeGate() : AInteractable("InteractableGate")
{

}

void InteractabeGate::onInteract()
{
	std::cout << "Opening Gate GUI" << std::endl;
}
