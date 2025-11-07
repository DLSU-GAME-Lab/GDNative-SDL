#include "StoryWindow.h"

StoryWindow::StoryWindow(std::string strName):AGameObject(strName)
{
}

StoryWindow::~StoryWindow()
{
}

void StoryWindow::initialize()
{
	
}

void StoryWindow::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
}

EventKey StoryWindow::getKey()
{
	return this->EKey;
}

bool StoryWindow::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void StoryWindow::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string StoryWindow::getListenerOwnerName()
{
	return this->getName();
}
