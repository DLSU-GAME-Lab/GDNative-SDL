#include "AUIScreen.h"

AUIScreen::AUIScreen(std::string name)
{
    this->name = name;
}

AUIScreen::~AUIScreen()
{

}

std::string AUIScreen::GetName()
{
    return this->name;
}
