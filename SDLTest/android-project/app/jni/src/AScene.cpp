#include "AScene.h"
#include "SpriteRendererSystem.h"

AScene::AScene(SceneTag ETag) : ETag(ETag) {}

void AScene::onUnloadObjects() {
    GameObjectManager::getInstance()->deleteAllObjects();
}

void AScene::update(float deltaTime) {
    GameObjectManager::getInstance()->update();
}

void AScene::render(SDL_Renderer* pRenderer) {
    // GameObject drawing is handled by SpriteRendererSystem
    SpriteRendererSystem::getInstance()->draw();
}

void AScene::registerObject(AGameObject* pGameObject) {
    GameObjectManager::getInstance()->addObject(pGameObject);
}

SceneTag AScene::getTag() const {
    return this->ETag;
}