#include "GameObject.h"
#include <iostream>

GameObject::GameObject(std::string strName)
{
    this->strName = strName;
    bEnabled = true;
    pParent = NULL;
}

void GameObject::processInput(SDL_Event eEvent)
{

}

void GameObject::update()
{

}

void GameObject::draw(SDL_Window * pWindow)
{

}

void GameObject::attachChild(GameObject * pChild)
{
    this->vecChildren.push_back(pChild);
    pChild->setParent(this);
    pChild->initialize();
}

void GameObject::detachChild(GameObject * pChild)
{
    int nIndex = -1;
    for (int i = 0; i < this->vecChildren.size() && nIndex == -1; i++)
    {
        if (this->vecChildren[i] == pChild)
        {
            nIndex = i;
        }
    }

    if (nIndex != -1)
        this->vecChildren.erase(this->vecChildren.begin() + nIndex);
}

GameObject* GameObject::findChildByName(std::string strName)
{
    int nIndex = -1;
    for (int i = 0; i < this->vecChildren.size() && nIndex == -1; i++)
    {
        if (this->vecChildren[i]->getName() == strName)
        {
            nIndex = i;
        }
    }

    if (nIndex != -1)
        return this->vecChildren[nIndex];
    else
    {
        std::cout << "[ERROR] : Child [" + strName + "] NOT found." << std::endl;
        return NULL;
    }
}

void GameObject::attachComponent(Component* pComponent)
{
    this->vecComponent.push_back(pComponent);
    pComponent->attachOwner(this);
}

void GameObject::detachComponent(Component * pComponent)
{
    int nIndex = -1;
    for (int i = 0; i < this->vecComponent.size() && nIndex == -1; i++)
    {
        if (this->vecComponent[i] == pComponent)
        {
            nIndex = i;
        }
    }

    if (nIndex != -1)
    {
        this->vecComponent[nIndex]->detachOwner();
        this->vecComponent.erase(this->vecComponent.begin() + nIndex);
    }
}

Component* GameObject::findComponentByName(std::string strName)
{
    for (Component* pComponent : this->vecComponent)
    {
        if (pComponent->getName() == strName)
            return pComponent;
    }

    std::cout << "[ERROR] : Component [" << strName << "] NOT found." << std::endl;
    return NULL;
}

std::vector<Component*> GameObject::getComponents(ComponentType EType)
{
    std::vector<Component*> vecFound = {};

    for (Component* pComponent : this->vecComponent)
    {
        if (pComponent->getType() == EType)
            vecFound.push_back(pComponent);
    }

    return vecFound;
}

std::vector<Component*> GameObject::getComponentsRecursively(ComponentType EType, bool bInclusive)
{
    std::vector<Component*> vecFound = {};

    if (bInclusive)
        vecFound = this->getComponents(EType);

    for (GameObject* pChild : this->vecChildren)
    {
        for (Component* pComponent : pChild->getComponents(EType))
        {
            if (pComponent->getType() == EType)
                vecFound.push_back(pComponent);
        }
    }

    return vecFound;
}

bool GameObject::isEnabled()
{
    return this->bEnabled;
}

void GameObject::setEnabled(bool bEnabled)
{
    this->bEnabled = bEnabled;
}

std::string GameObject::getName()
{
    return this->strName;
}

GameObject* GameObject::getParent()
{
    return this->pParent;
}

void GameObject::setParent(GameObject* pParent)
{
    this->pParent = pParent;
}
