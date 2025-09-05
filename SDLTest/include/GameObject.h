//#pragma once
//
//#include "Component.h"
//#include "GeneralInput.h"
//
//#include "AnimatedTexture.h"
//
//namespace models {
//    using namespace components;
//    
//    class GameObject {
//        protected:
//            bool bEnabled;
//            std::string strName;
//            sf::Sprite* pSprite;
//            AnimatedTexture* pTexture;
//
//            GameObject* pParent;
//            std::vector<GameObject*> vecChildren;
//            std::vector<Component*> vecComponent;
//
//        public:
//            GameObject(std::string strName, AnimatedTexture* pTexture = NULL);
//            virtual ~GameObject() = default;
//
//        public:
//            virtual void initialize() = 0;
//            virtual void processInput(sf::Event eEvent);
//            virtual void update(sf::Time tDeltaTime);
//            virtual void draw(sf::RenderWindow* pWindow, sf::RenderStates CRenderStates);
//
//        public:
//            void attachChild(GameObject* pChild);
//            void detachChild(GameObject* pChild);
//            GameObject* findChildByName(std::string strName);
//
//            void attachComponent(Component* pComponent);
//            void detachComponent(Component* pComponent);
//            Component* findComponentByName(std::string strName);
//            std::vector<Component*> getComponents(ComponentType EType);
//            std::vector<Component*> getComponentsRecursively(ComponentType EType, bool bInclusive = true);
//
//            void incrementFrame();
//            void centerSpriteOrigin();
//
//        public:
//            virtual sf::FloatRect getGlobalBounds();
//
//        public:
//            bool isEnabled();
//            void setEnabled(bool bEnabled);
//            std::string getName();
//            sf::Sprite* getSprite();
//            GameObject* getParent();
//            void setParent(GameObject* pParent);
//            void setFrame(int nFrame);
//            int getFrameSize();
//            int getCurrentFrame();
//    };
//}
