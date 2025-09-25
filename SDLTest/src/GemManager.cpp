#include "GemManager.h"
#include "GameObjectManager.h"
#include "EmptyObject.h"
#include "Prop.h"
#include "TextureManager.h"
#include <ctime>

void GemManager::perform()
{

}

void GemManager::loadResources()
{
    TextureManager::getInstance()->load("gems/SP_Gem_Yellow.png", "Yellow");
    TextureManager::getInstance()->load("gems/SP_Gem_Blue.png", "Blue");
    TextureManager::getInstance()->load("gems/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("gems/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("gems/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("gems/SP_Gem_White.png", "White");
}

void GemManager::unloadResources()
{
    TextureManager::getInstance()->unload("Yellow");
    TextureManager::getInstance()->unload("Blue");
    TextureManager::getInstance()->unload("Green");
    TextureManager::getInstance()->unload("Purple");
    TextureManager::getInstance()->unload("Red");
    TextureManager::getInstance()->unload("White");
}

void GemManager::startLevel1()
{
    for (int i = 0; i < this->vecCell.size(); i++)
    {
        if ((this->vecCell[i].r == 0 && this->vecCell[i].c == 0) ||
            (this->vecCell[i].r == 0 && this->vecCell[i].c == 8) ||
            (this->vecCell[i].r == 9 && (this->vecCell[i].c > 2 && this->vecCell[i].c < 6)))
        {
            this->vecCell[i].blocked = true;
        }
    }

    this->createGems();
}

void GemManager::startLevel2()
{
    //TODO: create cell blocking for level 2
    for (int i = 0; i < this->vecCell.size(); i++)
    {
        if ((this->vecCell[i].r == 0 && this->vecCell[i].c == 0) ||
            (this->vecCell[i].r == 0 && this->vecCell[i].c == 8) ||
            (this->vecCell[i].r == 9 && (this->vecCell[i].c > 2 && this->vecCell[i].c < 6)))
        {
            this->vecCell[i].blocked = true;
        }
    }

    this->createGems();
}

void GemManager::startLevel3()
{
    //TODO: create cell blocking for level 3
    for (int i = 0; i < this->vecCell.size(); i++)
    {
        if ((this->vecCell[i].r == 0 && this->vecCell[i].c == 0) ||
            (this->vecCell[i].r == 0 && this->vecCell[i].c == 8) ||
            (this->vecCell[i].r == 9 && (this->vecCell[i].c > 2 && this->vecCell[i].c < 6)))
        {
            this->vecCell[i].blocked = true;
        }
    }

    this->createGems();
}

void GemManager::moveGems()
{
    Vector2D pos = vecGem[selected[0]]->getPos();
    vecGem[selected[0]]->setPos(vecGem[selected[1]]->getPos());
    vecGem[selected[1]]->setPos(pos);
    selected[0] = -1;
    selected[1] = -1;

    matchGems();
}

void GemManager::matchGems()
{

}

void GemManager::createGems()
{
    srand(time(0));

    std::string gemColors[]{ "Yellow", "Blue", "White", "Green", "Purple", "Red" };
    for (int i = 0; i < this->vecCell.size(); i++)
    {
        if (!this->vecCell[i].blocked)
        {
            std::string gemColor = gemColors[rand() % 6];
            //TODO: replace with functional gems
            Prop* pGem = new Prop(std::to_string(i), gemColor, this->vecCell[i].pos, Vector2D(0.2f), 0.0f, false);
            this->vecCell[i].obj = pGem;
            this->vecGem.push_back(pGem);
            GameObjectManager::getInstance()->addObject(pGem);
        }
    }
}

GemManager* GemManager::P_SHARED_INSTANCE = NULL;

GemManager::GemManager(Uint8 w, Uint8 h, float fCellSize) : Grid("GemManager", w, h, fCellSize)
{

}

void GemManager::initialize(Uint8 w, Uint8 h, float fCellSize, Vector2D offset)
{
    EmptyObject* pManagerObject = new EmptyObject("GUI Manager");
    P_SHARED_INSTANCE = new GemManager(w, h, fCellSize);
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    pManagerObject->setPos(offset);
    GameObjectManager::getInstance()->addObject(pManagerObject);
    P_SHARED_INSTANCE->setup();
}

void GemManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

GemManager* GemManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
