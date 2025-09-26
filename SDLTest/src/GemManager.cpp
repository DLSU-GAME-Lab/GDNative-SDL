#include "GemManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include <ctime>

void GemManager::onAttach()
{
    Grid::onAttach();
}

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
    for (Uint8 r = 0; r < this->gridCells.size(); r++)
    {
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++)
        {
            if ((r == 0 && (c == 0 || c == 8)) ||
                (r == 9 && (c > 2 && c < 6)))
            {
                this->gridCells[r][c].blocked = true;
            }
        }
    }

    this->createGems(0.2f);
}

void GemManager::startLevel2()
{
    for (Uint8 r = 0; r < this->gridCells.size(); r++)
    {
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++)
        {
            if (((r == 0 || r == 6) && (c == 0 || c == 1 || c == 8 || c == 9)) ||
                (r == 0 && (c == 4 || c == 5)) ||
                ((r == 1 || r == 5) && (c == 0 || c == 9)) ||
                (r == 7 && (c == 0 || c == 1 || c == 2 || c == 7 || c == 8 || c == 9)))
            {
                this->gridCells[r][c].blocked = true;
            }
        }
    }

    this->createGems(0.2f);
}

void GemManager::startLevel3()
{
    for (Uint8 r = 0; r < this->gridCells.size(); r++)
    {
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++)
        {
            if ((r == 0 && !(c == 3 || c == 4 || c == 7 || c == 8)) ||
                (r == 1 && (c == 0 || c == 5 || c == 6 || c == 11)) ||
                (r == 2 && !((c >= 0 && c <= 2) || (c >= 9 && c <= 11))) ||
                (r == 3 && !(c == 0 || c == 1 || c == 10 || c == 11)) ||
                (r == 6 && (c == 2 || c == 3 || c == 8 || c == 9)) ||
                (r == 7 && ((c >= 2 && c <= 4) || (c >= 7 && c <= 9))) ||
                (r == 8 && ((c >= 1 && c <= 4) || (c >= 7 && c <= 10))) ||
                (r == 9 && !(c == 1 || c == 5 || c == 6 || c == 10)) ||
                (r == 10 && (c == 4 || c == 7)) ||
                (r == 11 && (c == 0 || c == 11)) ||
                (r == 12 && !(c >= 3 && c <= 8)) ||
                (r == 13 && !(c == 5 || c == 6)))
            {
                this->gridCells[r][c].blocked = true;
            }
        }
    }

    this->createGems(0.15f);
}

void GemManager::setSelected(Gem* pSelected)
{
    if (this->pSelected[0] == NULL)
        this->pSelected[0] = pSelected;
    else if (this->pSelected[1] != pSelected)
    {
        this->pSelected[1] = pSelected;

        Vector2D pos0 = this->pSelected[0]->getPos();
        Vector2D pos1 = this->pSelected[1]->getPos();
        if ((pos0 - pos1).SqrMagnitude() <= this->fCellSize * this->fCellSize)
            this->moveGems();

        else
        {
            this->pSelected[0] = NULL;
            this->pSelected[1] = NULL;
        }
    }
    else this->pSelected[0] = NULL;
}

void GemManager::moveGems()
{
    Vector2D pos = pSelected[0]->getPos();
    pSelected[0]->setPos(pSelected[1]->getPos());
    pSelected[1]->setPos(pos);
    std::cout << "Swapped " << pSelected[0]->getName() << " with " << pSelected[1]->getName() << "\n";

    pSelected[0] = NULL;
    pSelected[1] = NULL;

    this->matchGems();
}

void GemManager::matchGems()
{
    std::vector<AGameObject*> toRemove;
    for (Uint8 r = 0; r < this->nHeight; r++)
    {
        GemType type = GemType::WHITE;
        int count = 0;

        for (Uint8 c = 0; c < this->nWidth; c++)
        {
            if (this->gridCells[r][c].blocked || this->gridCells[r][c].obj == NULL) continue;
            GemType currentType = ((Gem*)this->gridCells[r][c].obj)->getType();

            if (type != currentType)
            {
                type = currentType;
                count = 1;
            }
            else
            {
                count++;
                if (count == 3)
                {
                    toRemove.push_back(this->gridCells[r][c].obj);
                    toRemove.push_back(this->gridCells[r][c - 1].obj);
                    toRemove.push_back(this->gridCells[r][c - 2].obj);
                    std::cout << "Horizontal Match!" << "\n";
                }
                else if (count > 3) toRemove.push_back(this->gridCells[r][c].obj);
            }
        }
    }
    for (Uint8 c = 0; c < this->nWidth; c++)
    {
        GemType type = GemType::WHITE;
        int count = 0;

        for (Uint8 r = 0; r < this->nHeight; r++)
        {
            if (this->gridCells[r][c].blocked || this->gridCells[r][c].obj == NULL) continue;
            GemType currentType = ((Gem*)this->gridCells[r][c].obj)->getType();

            if (type != currentType)
            {
                type = currentType;
                count = 1;
            }
            else
            {
                count++;
                if (count == 3)
                {
                    toRemove.push_back(this->gridCells[r][c].obj);
                    toRemove.push_back(this->gridCells[r - 1][c].obj);
                    toRemove.push_back(this->gridCells[r - 2][c].obj);
                    std::cout << "Vertical Match!" << "\n";
                }
                else if (count > 3) toRemove.push_back(this->gridCells[r][c].obj);
            }
        }
    }

    for (int i = 0; i < toRemove.size(); i++)
    {
        if (toRemove[i] != NULL) GameObjectManager::getInstance()->deleteObject(toRemove[i]);
    }
}

void GemManager::createGems(float fScale)
{
    srand(time(0));

    std::string gemColors[]{ "Yellow", "Blue", "White", "Green", "Purple", "Red" };
    for (Uint8 r = 0; r < this->gridCells.size(); r++)
    {
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++)
        {
            if (!this->gridCells[r][c].blocked)
            {
                GemType gemType = GemType(rand() % 6);
                Gem* pGem = new Gem("gem_" + std::to_string((r + 1) * (c + 1)), gemType);
                pGem->setPos(this->gridCells[r][c].pos);
                pGem->setScale(Vector2D(fScale));
                this->gridCells[r][c].obj = pGem;
                GameObjectManager::getInstance()->addObject(pGem);
            }
        }
    }
}

GemManager* GemManager::P_SHARED_INSTANCE = NULL;

GemManager::GemManager(Uint8 w, Uint8 h, float fCellSize) : Grid("GemManager", w, h, fCellSize)
{
    this->pSelected[0] = NULL;
    this->pSelected[1] = NULL;
}

void GemManager::initialize(Uint8 w, Uint8 h, float fCellSize, Vector2D offset)
{
    EmptyObject* pManagerObject = new EmptyObject("GemManager");
    pManagerObject->setPos(offset);
    P_SHARED_INSTANCE = new GemManager(w, h, fCellSize);
    pManagerObject->attachComponent(P_SHARED_INSTANCE);
    GameObjectManager::getInstance()->addObject(pManagerObject);
}

void GemManager::destroy()
{
    delete P_SHARED_INSTANCE->pOwner;
}

GemManager* GemManager::getInstance()
{
    return P_SHARED_INSTANCE;
}
