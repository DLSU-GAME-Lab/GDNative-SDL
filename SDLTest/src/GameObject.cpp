#include "GameObject.h"

GameObject::GameObject(std::string strName)
{
    this->strName = strName;
    bEnabled = true;
    pParent = NULL;
}

void GameObject::initialize()
{

}

void GameObject::processInput(SDL_Event eEvent)
{

}

void GameObject::update(SDL_Time tDeltaTime)
{

}

void GameObject::draw(SDL_Window * pWindow)
{

}

void GameObject::attachChild(GameObject * pChild)
{

}

void GameObject::detachChild(GameObject * pChild)
{

}

GameObject* GameObject::findChildByName(std::string strName)
{
    return nullptr;
}

void GameObject::attachComponent(Component* pComponent)
{

}

void GameObject::detachComponent(Component * pComponent)
{

}

Component* GameObject::findComponentByName(std::string strName)
{
    return nullptr;
}

std::vector<Component*> GameObject::getComponents(ComponentType EType)
{
    return std::vector<Component*>();
}

std::vector<Component*> GameObject::getComponentsRecursively(ComponentType EType, bool bInclusive)
{
    return std::vector<Component*>();
}
