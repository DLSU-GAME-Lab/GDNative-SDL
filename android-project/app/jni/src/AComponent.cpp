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

AGameObject* AComponent::getOwner()
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
