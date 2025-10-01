#include "GemManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include <ctime>
#include <cstdlib>

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

// user selected a gem; handle selection, swapping, checking, and chain reactions
void GemManager::setSelected(Gem* pSelected)
{
    if (this->pSelectedObjects[0] == NULL)
    {
        // first selection
        this->pSelectedObjects[0] = pSelected;
        this->pSelectedCells[0] = getCellFromObject(pSelected);
        return;
    }

    // second selection: avoid double-selecting same gem
    if (this->pSelectedObjects[1] == NULL && this->pSelectedObjects[0] != pSelected)
    {
        this->pSelectedObjects[1] = pSelected;
        this->pSelectedCells[1] = getCellFromObject(pSelected);

        // ensure they are adjacent (distance check)
        Vector2D pos0 = this->pSelectedObjects[0]->getPos();
        Vector2D pos1 = this->pSelectedObjects[1]->getPos();
        if ((pos0 - pos1).SqrMagnitude() <= this->fCellSize * this->fCellSize)
        {
            // perform swap (swap obj pointers and update positions)
            this->moveGems();

            // if swap produces no match, revert swap
            if (!this->checkMatches())
            {
                // swap back immediately
                this->moveGems();
            }
            else
            {
                // chain reaction: keep cascading and filling while new matches appear
                do
                {
                    this->cascadeDown();
                    this->spawnGems(this->fGemSize);
                    // loop while another match is found after fill
                } while (this->checkMatches());
            }

            this->printGridData();
        }

        // clear selection regardless
        this->pSelectedObjects[0] = NULL;
        this->pSelectedObjects[1] = NULL;
        this->pSelectedCells[0] = NULL;
        this->pSelectedCells[1] = NULL;
    }
    else
    {
        // deselect if same gem clicked twice
        this->pSelectedObjects[0] = NULL;
        this->pSelectedCells[0] = NULL;
    }
}

// swap only the object pointers in the two selected cells and update render positions
// - do not swap entire CellData (r/c) so it remains consistent
void GemManager::moveGems()
{
    if (this->pSelectedCells[0] == NULL || this->pSelectedCells[1] == NULL) return;

    AGameObject* objA = this->pSelectedCells[0]->obj;
    AGameObject* objB = this->pSelectedCells[1]->obj;

    // swap object pointers in the cells
    this->pSelectedCells[0]->obj = objB;
    this->pSelectedCells[1]->obj = objA;

    // update world positions of the swapped objects so sprites render at correct places
    if (objB != NULL) objB->setPos(this->pSelectedCells[0]->pos);
    if (objA != NULL) objA->setPos(this->pSelectedCells[1]->pos);
}

// detect and remove matches horizontally and vertically
// - when a match is found this function nulls cell->obj and schedules deletion of game objects
bool GemManager::checkMatches()
{
    bool bMatched = false;
    std::vector<AGameObject*> removeHorizontal;

    // horizontal scan
    for (int r = 0; r < (int)this->nHeight; ++r)
    {
        GemType runType = GemType::WHITE;
        int count = 0;

        for (int c = 0; c < (int)this->nWidth; ++c)
        {
            if (this->gridCells[r][c].blocked || this->gridCells[r][c].obj == NULL)
            {
                // gap or blocker; reset current run
                runType = GemType::WHITE;
                count = 0;
                continue;
            }

            GemType currentType = static_cast<Gem*>(this->gridCells[r][c].obj)->getType();

            if (count == 0)
            {
                runType = currentType;
                count = 1;
            }
            else if (currentType == runType)
            {
                ++count;
                if (count == 3)
                {
                    // mark the three in the run (current and previous two)
                    removeHorizontal.push_back(this->gridCells[r][c].obj);
                    removeHorizontal.push_back(this->gridCells[r][c - 1].obj);
                    removeHorizontal.push_back(this->gridCells[r][c - 2].obj);

                    // clear pointers in grid immediately so cascade won't see them
                    this->gridCells[r][c].obj = NULL;
                    this->gridCells[r][c - 1].obj = NULL;
                    this->gridCells[r][c - 2].obj = NULL;
                    bMatched = true;
                }
                else if (count > 3)
                {
                    // longer run; add current one
                    removeHorizontal.push_back(this->gridCells[r][c].obj);
                    this->gridCells[r][c].obj = NULL;
                }
            }
            else
            {
                // different gem type; start a new run
                runType = currentType;
                count = 1;
            }
        }
    }

    // delete the collected horizontal objects
    for (auto obj : removeHorizontal)
    {
        if (obj != NULL) GameObjectManager::getInstance()->deleteObject(obj);
    }

    // vertical scan
    std::vector<AGameObject*> removeVertical;
    for (int c = 0; c < (int)this->nWidth; ++c)
    {
        GemType runType = GemType::WHITE;
        int count = 0;

        for (int r = 0; r < (int)this->nHeight; ++r)
        {
            if (this->gridCells[r][c].blocked || this->gridCells[r][c].obj == NULL)
            {
                // reset on gap or blocked cell
                runType = GemType::WHITE;
                count = 0;
                continue;
            }

            GemType currentType = static_cast<Gem*>(this->gridCells[r][c].obj)->getType();

            if (count == 0)
            {
                runType = currentType;
                count = 1;
            }
            else if (currentType == runType)
            {
                ++count;
                if (count == 3)
                {
                    removeVertical.push_back(this->gridCells[r][c].obj);
                    removeVertical.push_back(this->gridCells[r - 1][c].obj);
                    removeVertical.push_back(this->gridCells[r - 2][c].obj);

                    this->gridCells[r][c].obj = NULL;
                    this->gridCells[r - 1][c].obj = NULL;
                    this->gridCells[r - 2][c].obj = NULL;
                    bMatched = true;
                }
                else if (count > 3)
                {
                    removeVertical.push_back(this->gridCells[r][c].obj);
                    this->gridCells[r][c].obj = NULL;
                }
            }
            else
            {
                runType = currentType;
                count = 1;
            }
        }
    }

    // delete the collected vertical objects
    for (auto obj : removeVertical)
    {
        if (obj != NULL) GameObjectManager::getInstance()->deleteObject(obj);
    }

    std::cout << "Checked for matches." << "\n";
    this->printGridData();
    return bMatched;
}

// gravity: drop gems down but never through blocked cells
void GemManager::cascadeDown()
{
    // iterate bottom-to-top so we move lower empty cells first
    for (int r = (int)this->gridCells.size() - 1; r >= 0; --r)
    {
        for (int c = (int)this->gridCells[r].size() - 1; c >= 0; --c)
        {
            if (this->gridCells[r][c].blocked) continue;

            if (this->gridCells[r][c].obj == NULL)
            {
                int indexAbove = r - 1;
                // search upwards for first available gem, stop if a blocked cell is encountered
                while (indexAbove >= 0)
                {
                    if (this->gridCells[indexAbove][c].blocked)
                    {
                        indexAbove = -1;
                        break;
                    }
                    if (this->gridCells[indexAbove][c].obj != NULL) break;
                    --indexAbove;
                }

                if (indexAbove >= 0)
                {
                    // move object pointer down and update its world position
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
    std::string gemColors[]{ "Yellow", "Blue", "White", "Green", "Purple", "Red" };

    for (int r = 0; r < (int)this->gridCells.size(); ++r)
    {
        for (int c = 0; c < (int)this->gridCells[r].size(); ++c)
        {
            if (!this->gridCells[r][c].blocked && this->gridCells[r][c].obj == NULL)
            {
                GemType gemType;
                bool valid = false;

                // keep rerolling until no 3-match on spawn
                while (!valid)
                {
                    gemType = GemType(rand() % 6);
                    valid = true;

                    // check horizontal left neighbors
                    if (c >= 2 &&
                        this->gridCells[r][c - 1].obj != NULL &&
                        this->gridCells[r][c - 2].obj != NULL)
                    {
                        auto t1 = static_cast<Gem*>(this->gridCells[r][c - 1].obj)->getType();
                        auto t2 = static_cast<Gem*>(this->gridCells[r][c - 2].obj)->getType();
                        if (t1 == gemType && t2 == gemType)
                            valid = false;
                    }

                    // check vertical above neighbors
                    if (r >= 2 &&
                        this->gridCells[r - 1][c].obj != NULL &&
                        this->gridCells[r - 2][c].obj != NULL)
                    {
                        auto t1 = static_cast<Gem*>(this->gridCells[r - 1][c].obj)->getType();
                        auto t2 = static_cast<Gem*>(this->gridCells[r - 2][c].obj)->getType();
                        if (t1 == gemType && t2 == gemType)
                            valid = false;
                    }
                }

                // safe to spawn gem now
                Gem* pGem = new Gem("gem_" + std::to_string(r) + "_" + std::to_string(c), gemType);
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
    srand((unsigned)time(nullptr)); // seed once
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
