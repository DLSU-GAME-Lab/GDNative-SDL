#pragma once

#include "GameObjectManager.h"
#include "TextureManager.h"
//#include "FontManager.h"
//#include "SFXManager.h"

#include "EnumSceneTag.h"
#include "AGameObject.h"

class AScene
{
private:
    SceneTag ETag;

public:
    AScene(SceneTag ETag);
    ~AScene();

public:
    virtual void onLoadResources() = 0;
    virtual void onLoadObjects() = 0;
    virtual void onUnloadResources() = 0;
    virtual void onUnloadObjects();

protected:
    void registerObject(AGameObject* pGameObject);

public:
    SceneTag getTag();
};
