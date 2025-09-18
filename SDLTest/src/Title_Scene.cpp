#include "Title_Scene.h"
#include "Background.h"
#include "UIButton.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "EnumSceneTag.h"
#include "Prop.h"
#include "AnimatedSprite.h"

Title_Scene::Title_Scene()
    : AScene(SceneTag::TITLE_SCENE)
{
}

void Title_Scene::onLoadResources()
{
    // naming convention (TextureManager prefixes "Assets/")
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Title_Background");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_GemHunterMatch_Logo.png", "Title_Logo");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Yellow.png", "Yellow");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Blue.png", "Fish");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_White.png", "White");
    TextureManager::getInstance()->load("title_screen_pngs/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "Button_Container");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_LevelEntry_Bg.png", "Level_Button");
}

void Title_Scene::onLoadObjects()
{
    
}

void Title_Scene::onUnloadResources()
{
    TextureManager::getInstance()->unload("Title_Background");
    TextureManager::getInstance()->unload("Title_Logo");
    TextureManager::getInstance()->unload("Yellow");
    TextureManager::getInstance()->unload("Fish");
    TextureManager::getInstance()->unload("Green");
    TextureManager::getInstance()->unload("Purple");
    TextureManager::getInstance()->unload("Red");
    TextureManager::getInstance()->unload("White");
    TextureManager::getInstance()->unload("Button_Container");
    TextureManager::getInstance()->unload("Level_Button");
}

void Title_Scene::onUnloadObjects()
{
    // base implementation to remove all objects
    AScene::onUnloadObjects();
}
