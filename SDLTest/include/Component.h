#pragma once

#include "EnumComponentType.h"
#include <SDL3/SDL.h>
#include <string>

namespace models {
    class GameObject;
}

namespace components {
    using namespace models;
    
    class Component {
        protected:
            GameObject* pOwner;
            ComponentType EType;
            std::string strName;
            //sf::Time tDeltaTime;

        public:
            Component(std::string strName, ComponentType EType);
            virtual ~Component();
            
        public:
            void attachOwner(GameObject* pOwner);
            void detachOwner();

        public:
            virtual void perform() = 0;
        
        public:
            GameObject* getOwner();
            ComponentType getType();
            std::string getName();
            //void setDeltaTime(sf::Time tDeltaTime);
    };
}
