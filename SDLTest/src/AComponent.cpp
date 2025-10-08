#pragma once

#include "AComponent.h"

AComponent::AComponent(std::string strName, ComponentType EType)
{
    this->strName = strName;
    this->pOwner = NULL;
    this->EType = EType;
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
