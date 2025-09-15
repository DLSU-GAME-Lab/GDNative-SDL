#include "UIScreen.h"

UIScreen::UIScreen(std::string name)
{
    this->name = name;
}

UIScreen::~UIScreen()
{

}

std::string UIScreen::GetName()
{
    return this->name;
}
