#include "AGeneralInput.h"

AGeneralInput::AGeneralInput(std::string strName) : AComponent(strName, ComponentType::INPUT)
{
	this->eEvent = NULL;
}

AGeneralInput::~AGeneralInput()
{

}

void AGeneralInput::setEvent(SDL_Event* eEvent)
{
	this->eEvent = eEvent;
}
