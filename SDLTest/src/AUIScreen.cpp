#include "AUIScreen.h"

using namespace Editor;

Editor::AUIScreen::AUIScreen(std::string name)
{
    this->name = name;
}

Editor::AUIScreen::~AUIScreen()
{

}

std::string Editor::AUIScreen::GetName()
{
    return this->name;
}
