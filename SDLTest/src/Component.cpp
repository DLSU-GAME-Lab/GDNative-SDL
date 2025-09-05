#pragma once

#include "Component.h"
#include "GameObject.h"

Component::Component(std::string strName, ComponentType EType) {
    this->strName = strName;
    this->pOwner = NULL;
    this->EType = EType;
}

Component::~Component() {
    this->pOwner = NULL;
    this->EType = ComponentType::NONE;
}

void Component::attachOwner(GameObject* pOwner) {
    this->pOwner = pOwner;
}

void Component::detachOwner() {
    delete this;
}

GameObject* Component::getOwner() {
    return this->pOwner;
}

ComponentType Component::getType() const {
    return this->EType;
}

std::string Component::getName() {
    return this->strName;
}
