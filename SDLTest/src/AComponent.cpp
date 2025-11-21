#pragma once

#include "AComponent.h"
#include "AGameObject.h"

AComponent::AComponent(std::string strName, ComponentType EType)
{
    this->strName = strName;
    this->pOwner = NULL;
    this->EType = EType;
    this->bEnabled = true;
}

AComponent::~AComponent()
{
    this->pOwner = NULL;
    this->EType = ComponentType::NONE;
}

void AComponent::attachOwner(AGameObject* pOwner)
{
    this->pOwner = pOwner;
}

void AComponent::detachOwner()
{
    delete this;
}

void AComponent::setDeltaTime(float fDeltaTime)
{
    this->fDeltaTime = fDeltaTime;
}

float AComponent::getDeltaTime() const
{
    return this->fDeltaTime;
}

void AComponent::setEnabled(bool bEnabled)
{
    this->bEnabled = bEnabled;
}

bool AComponent::getEnabled() const
{
    return this->bEnabled || this->pOwner->getEnabled();
}

AGameObject* AComponent::getOwner() const
{
    return this->pOwner;
}

ComponentType AComponent::getType() const
{
    return this->EType;
}

std::string AComponent::getName() const
{
    return this->strName;
}
