#pragma once

#include "Component.h"

using namespace components;

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

ComponentType Component::getType() {
    return this->EType;
}

std::string Component::getName() {
    return this->strName;
}
//
//void Component::setDeltaTime(sf::Time tDeltaTime) {
//    this->tDeltaTime = tDeltaTime;
//}
