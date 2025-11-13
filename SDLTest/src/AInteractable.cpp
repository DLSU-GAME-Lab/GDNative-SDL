#include "AInteractable.h"

AInteractable::AInteractable(std::string strName) : BoxCollider(strName)
{
	this->bIsTrigger = true;
}
