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

    this->spawnGems(0.2f);
    this->printGridData();
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

    this->spawnGems(0.2f);
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

    this->spawnGems(0.15f);
}

void GemManager::setSelected(Gem* pSelected)
{
    if (this->pSelectedObjects[0] == NULL)
    {
        this->pSelectedObjects[0] = pSelected;
        this->pSelectedCells[0] = getCellFromObject(pSelected);
    }
    else if (this->pSelectedObjects[1] != pSelected)
    {
        this->pSelectedObjects[1] = pSelected;
        this->pSelectedCells[1] = getCellFromObject(pSelected);

        Vector2D pos0 = this->pSelectedObjects[0]->getPos();
        Vector2D pos1 = this->pSelectedObjects[1]->getPos();
        if ((pos0 - pos1).SqrMagnitude() <= this->fCellSize * this->fCellSize)
        {
            this->moveGems();
            if (this->checkMatches())
            {
                this->cascadeDown();
                this->spawnGems(this->fGemSize);
            }
            this->printGridData();
        }

        else
        {
            this->pSelectedObjects[0] = NULL;
            this->pSelectedObjects[1] = NULL;
        }
    }
    else this->pSelectedObjects[0] = NULL;
}

void GemManager::moveGems()
{
    Vector2D pos = pSelectedObjects[0]->getPos();
    pSelectedObjects[0]->setPos(pSelectedObjects[1]->getPos());
    pSelectedObjects[1]->setPos(pos);

    Grid::CellData cell = *pSelectedCells[0];
    *pSelectedCells[0] = *pSelectedCells[1];
    *pSelectedCells[1] = cell;

    pSelectedObjects[0] = NULL;
    pSelectedObjects[1] = NULL;
}

bool GemManager::checkMatches()
{
    bool bMatched = false;
    std::vector<AGameObject*> removeHorizontal;
    for (Uint8 r = 0; r < this->nHeight; r++)
    {
        GemType type = GemType::WHITE;
        int count = 0;

        for (Uint8 c = 0; c < this->nWidth; c++)
        {
            if (this->gridCells[r][c].blocked || this->gridCells[r][c].obj == NULL) continue;
            GemType currentType = static_cast<Gem*>(this->gridCells[r][c].obj)->getType();

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
                    removeHorizontal.push_back(this->gridCells[r][c].obj);
                    removeHorizontal.push_back(this->gridCells[r][c - 1].obj);
                    removeHorizontal.push_back(this->gridCells[r][c - 2].obj);

                    this->gridCells[r][c].obj = NULL;
                    this->gridCells[r][c - 1].obj = NULL;
                    this->gridCells[r][c - 2].obj = NULL;
                    bMatched |= true;
                }
                else if (count > 3)
                {
                    removeHorizontal.push_back(this->gridCells[r][c].obj);
                    this->gridCells[r][c].obj = NULL;
                }
            }
        }
    }

    for (int i = 0; i < removeHorizontal.size(); i++)
    {
        if (removeHorizontal[i] != NULL) GameObjectManager::getInstance()->deleteObject(removeHorizontal[i]);
    }

    std::vector<AGameObject*> removeVertical;
    for (Uint8 c = 0; c < this->nWidth; c++)
    {
        GemType type = GemType::WHITE;
        int count = 0;

        for (Uint8 r = 0; r < this->nHeight; r++)
        {
            if (this->gridCells[r][c].blocked || this->gridCells[r][c].obj == NULL) continue;
            GemType currentType = static_cast<Gem*>(this->gridCells[r][c].obj)->getType();

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
                    removeVertical.push_back(this->gridCells[r][c].obj);
                    removeVertical.push_back(this->gridCells[r - 1][c].obj);
                    removeVertical.push_back(this->gridCells[r - 2][c].obj);

                    this->gridCells[r][c].obj = NULL;
                    this->gridCells[r - 1][c].obj = NULL;
                    this->gridCells[r - 2][c].obj = NULL;
                    bMatched |= true;
                }
                else if (count > 3)
                {
                    removeVertical.push_back(this->gridCells[r][c].obj);
                    this->gridCells[r][c].obj = NULL;
                }
            }
        }
    }

    for (int i = 0; i < removeVertical.size(); i++)
    {
        if (removeVertical[i] != NULL) GameObjectManager::getInstance()->deleteObject(removeVertical[i]);
    }
    std::cout << "Checked for matches." << "\n";
    this->printGridData();
    return bMatched;
}

void GemManager::cascadeDown()
{
    for (int r = this->gridCells.size() - 1; r >= 0; r--)
    {
        for (int c = this->gridCells[r].size() - 1; c >= 0; c--)
        {
            if (!this->gridCells[r][c].blocked && this->gridCells[r][c].obj == NULL)
            {
                int indexAbove = r - 1;
                while (indexAbove >= 0 && this->gridCells[indexAbove][c].obj == NULL) indexAbove--;
                if (indexAbove >= 0)
                {
                    this->gridCells[r][c].obj = this->gridCells[indexAbove][c].obj;
                    this->gridCells[r][c].obj->setPos(this->gridCells[r][c].pos);
                    this->gridCells[indexAbove][c].obj = NULL;
                }
            }
        }
    }
}

void GemManager::printGridData()
{
    std::cout << "\n  ";
    for (Uint8 c = 0; c < this->nWidth; c++) std::cout << "--- ";
    std::cout << "\n";
    for (Uint8 r = 0; r < this->gridCells.size(); r++)
    {
        std::cout << " |";
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++)
        {
            if (this->gridCells[r][c].blocked)
            {
                std::cout << " * |";
                continue;
            }
            else if (this->gridCells[r][c].obj == NULL)
            {
                std::cout << "   |";
                continue;
            }

            switch (static_cast<Gem*>(this->gridCells[r][c].obj)->getType())
            {
            case GemType::WHITE:
                std::cout << " W |";
                break;
            case GemType::RED:
                std::cout << " R |";
                break;
            case GemType::YELLOW:
                std::cout << " Y |";
                break;
            case GemType::GREEN:
                std::cout << " G |";
                break;
            case GemType::BLUE:
                std::cout << " B |";
                break;
            case GemType::PURPLE:
                std::cout << " P |";
                break;
            }
        }
        std::cout << "\n  ";
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++) std::cout << "--- ";
        std::cout << "\n";
    }
}

void GemManager::spawnGems(float fScale)
{
    this->fGemSize = fScale;
    srand(time(0));

    std::string gemColors[]{ "Yellow", "Blue", "White", "Green", "Purple", "Red" };
    for (Uint8 r = 0; r < this->gridCells.size(); r++)
    {
        for (Uint8 c = 0; c < this->gridCells[r].size(); c++)
        {
            if (!this->gridCells[r][c].blocked && this->gridCells[r][c].obj == NULL)
            {
                GemType gemType = GemType(rand() % 6);
                Gem* pGem = new Gem("gem_" + std::to_string((r + 1) * (c + 1)), gemType);
                pGem->setPos(this->gridCells[r][c].pos);
                pGem->setScale(Vector2D(this->fGemSize));
                this->gridCells[r][c].obj = pGem;
                GameObjectManager::getInstance()->addObject(pGem);
            }
        }
    }
}

GemManager* GemManager::P_SHARED_INSTANCE = NULL;

GemManager::GemManager(Uint8 w, Uint8 h, float fCellSize) : Grid("GemManager", w, h, fCellSize)
{
    this->pSelectedObjects[0] = NULL;
    this->pSelectedObjects[1] = NULL;
    this->pSelectedCells[0] = NULL;
    this->pSelectedCells[1] = NULL;
    this->fGemSize = 1.0f;
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
