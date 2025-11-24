#pragma once

#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"
#include "AGameObject.h"
#include <SDL3/SDL.h>
#include "EventBroadcaster.h"
//#include "FontManager.h"
//#include "SFXManager.h"

class AScene
{
private:
    SceneTag ETag;

public:
    AScene(SceneTag ETag);
    virtual ~AScene() = default;

public:
    virtual void onLoadResources() = 0;
    virtual void onLoadObjects() = 0;
    virtual void onUnloadResources() = 0;
    virtual void onUnloadObjects();

protected:
    void registerObject(AGameObject* pGameObject);

public:
    SceneTag getTag() const;
};
