#include "AGameObject.h"
#include "AGeneralInput.h"
#include <iostream>
#include "Collider.h"
AGameObject::AGameObject(std::string strName)
{
    this->strName = strName;
    bEnabled = true;
    bFollowParent = true;
    pParent = NULL;

    // defaults:
    this->fRot = 0.0f;
    this->fVecScale = Vector2D(1, 1);
    this->bIsScreenObject = false;
}

AGameObject::~AGameObject()
{
    // Delete components
    for (int i = 0; i < this->vecComponent.size(); i++)
    {
        if (this->vecComponent[i])
        {
            delete this->vecComponent[i];
        }
    }

    // Delete children
    for (int i = 0; i < this->vecChildren.size(); i++)
    {
        if (this->vecChildren[i])
        {
            delete this->vecChildren[i];
        }
    }
    vecChildren.clear();
}

void AGameObject::processInput(SDL_Event* eEvent)
{
    auto vecInput = this->getComponentsRecursively(ComponentType::INPUT);
    for (AComponent* pComponent : vecInput)
    {
        AGeneralInput* input = (AGeneralInput*)pComponent;
        input->setEvent(eEvent);
        input->perform();
    }

}

void AGameObject::update(float fDeltaTime)
{
    auto vecScript = this->getComponentsRecursively(ComponentType::SCRIPT);
    for (AComponent* pComponent : vecScript)
    {
        pComponent->setDeltaTime(fDeltaTime);
        pComponent->perform();
    }
}

void AGameObject::draw(SDL_Renderer* pRenderer)
{

    auto vecRenderer = this->getComponentsRecursively(ComponentType::RENDERER);
    for (AComponent* pComponent : vecRenderer)
    {
        AGameObject* pOwner = pComponent->getOwner(); // Assuming each component knows its owner
        if (!pOwner || !pOwner->isGloballyEnabled())
            continue;

        ARenderer* renderer = (ARenderer*)pComponent;
        renderer->setSDLRenderer(pRenderer);
        pComponent->perform();
    }

}
SDL_FRect  AGameObject::getGlobalBounds()
{
    SpriteRenderer* renderer = (SpriteRenderer*)this->findComponentByName("SpriteRenderer");
    SDL_FRect CTransform = renderer->getRect();
    return CTransform;
}
void AGameObject::attachChild(AGameObject * pChild)
{
    this->vecChildren.push_back(pChild);
    pChild->setParent(this);
    pChild->initialize();
}

void AGameObject::detachChild(AGameObject * pChild)
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

AGameObject* AGameObject::findChildByName(std::string strName)
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

void AGameObject::attachComponent(AComponent* pComponent)
{
    this->vecComponent.push_back(pComponent);
    pComponent->attachOwner(this);
    pComponent->onAttach();
}

void AGameObject::detachComponent(AComponent * pComponent)
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

AComponent* AGameObject::findComponentByName(std::string strName)
{
    for (AComponent* pComponent : this->vecComponent)
    {
        if (pComponent->getName() == strName)
            return pComponent;
    }

    std::cout << "[ERROR] : Component [" << strName << "] NOT found." << std::endl;
    return NULL;
}

std::vector<AComponent*> AGameObject::getComponents(ComponentType EType)
{
    std::vector<AComponent*> vecFound = {};

    for (AComponent* pComponent : this->vecComponent)
    {
        if (pComponent->getType() == EType)
            vecFound.push_back(pComponent);
    }

    return vecFound;
}

std::vector<AComponent*> AGameObject::getComponentsRecursively(ComponentType EType, bool bInclusive)
{
    std::vector<AComponent*> vecFound = {};

    if (bInclusive)
        vecFound = this->getComponents(EType);

    for (AGameObject* pChild : this->vecChildren)
    {
        std::vector<AComponent*> childComponents = pChild->getComponentsRecursively(EType, bInclusive);
        vecFound.insert(vecFound.end(), childComponents.begin(), childComponents.end());
    }
    

    return vecFound;
}

bool AGameObject::componentExists(std::string strName)
{
    for (AComponent* pComponent : this->vecComponent)
    {
        if (pComponent->getName() == strName)
            return true;
    }
    return false;
}

bool AGameObject::getEnabled() const
{
    return this->bEnabled;
}

void AGameObject::setEnabled(bool bEnabled)
{
    this->bEnabled = bEnabled;
    if (!this->vecChildren.empty())
    {
        for (AGameObject* pObject:vecChildren)
        {
            if(pObject->getFollowParent())
            {
                pObject->setEnabled(bEnabled);
            }
            
        }
    }
}

std::string AGameObject::getName() const
{
    return this->strName;
}

AGameObject* AGameObject::getParent() const
{
    return this->pParent;
}

void AGameObject::setParent(AGameObject* pParent)
{
    this->pParent = pParent;
}

void AGameObject::setPos(Vector2D fVecTranslate)
{
    this->fVecTranslate = fVecTranslate;
    if (!this->vecChildren.empty())
    {
        for(AGameObject* child: vecChildren)
        {
            if (child->pParent->getParent() != nullptr)
            {
                child->setPos(child->getParent()->getPos());
            }
            else
            {
                Vector2D fVecNewPos(child->getPos().x + child->getParent()->getPos().x, child->getPos().y + child->getParent()->getPos().y);
                child->setPos(fVecNewPos);
            }
  
        }
    }

}

void AGameObject::setScale(Vector2D fVecScale)
{
    this->fVecScale = fVecScale;
}

Vector2D AGameObject::getPos()
{
    return this->fVecTranslate;
}

Vector2D AGameObject::getScale()
{
    return this->fVecScale;
}


void AGameObject::setRot(float fRot)
{
    this->fRot = fRot;
}

float AGameObject::getRot()
{
    return this->fRot;
}

bool AGameObject::getIsScreenObject() const
{
    return this->bIsScreenObject;
}

void AGameObject::setIsScreenObject(bool bIsScreenObject)
{
    this->bIsScreenObject = bIsScreenObject;
}

bool AGameObject::getFollowParent()
{
    return this->bFollowParent;
}

void AGameObject::setFollowParent(bool bFollowParent)
{
    this->bFollowParent = bFollowParent;
}

bool AGameObject::isGloballyEnabled() const
{
    if (!this->bEnabled)
        return false;

    if (this->pParent)
        return this->pParent->isGloballyEnabled();

    return true;
}



