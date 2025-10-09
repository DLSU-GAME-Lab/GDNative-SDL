#include "GemManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include <ctime>
#include <cstdlib>

void GemManager::onAttach()
{
    for (Uint8 r = 0; r < this->nHeight; r++)
    {
        std::vector<GemData> col;
        for (Uint8 c = 0; c < this->nWidth; c++)
        {
            col.push_back({ r, c, NULL, false });
        }

        this->data.push_back(col);
    }
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

// user selected a gem; handle selection, swapping, checking, and chain reactions
void GemManager::setSelected(Gem* pGem)
{
    if (this->bAnimating) return;

    if (this->pSelected[0] == NULL)
    {
        // first selection
        this->pSelected[0] = this->getDataFromGem(pGem);
        return;
    }

    // second selection: avoid double-selecting same gem
    if (this->pSelected[1] == NULL && this->pSelected[0]->gem != pGem)
    {
        this->pSelected[1] = this->getDataFromGem(pGem);
        if (this->pSelected == NULL) return;

        std::cout << "selected 0: " << this->pSelected[0]->gem->getName() <<
                    " selected 1: " << this->pSelected[1]->gem->getName() << "\n";

        // ensure they are adjacent (distance check)
        Vector2D pos0 = getGemDataPosition(*this->pSelected[0]);
        Vector2D pos1 = getGemDataPosition(*this->pSelected[1]);
        if ((pos0 - pos1).SqrMagnitude() <= this->fGemSize * this->fGemSize)
        {
            // perform swap (swap obj pointers and update positions)
            this->pSelected[0]->gem->setActive(true);
            this->moveGems();
            this->printGridData();
            this->bAnimating = true;
        }
    }
    else
    {
        // deselect if same gem clicked twice
        this->pSelected[0] = NULL;
    }
}

// swap only the object pointers in the two selected cells and update render positions
// - do not swap entire CellData (r/c) so it remains consistent
void GemManager::moveGems()
{
    Gem* objA = this->pSelected[0]->gem;
    Gem* objB = this->pSelected[1]->gem;

    // swap object pointers in the cells
    this->pSelected[0]->gem = objB;
    this->pSelected[1]->gem = objA;

    this->setTween(*this->pSelected[0]);
    this->setTween(*this->pSelected[1]);
}

void GemManager::setTween(GemData gemData)
{
    Vector2D startPos = gemData.gem->getPos();
    Vector2D endPos = getGemDataPosition(gemData);
    TweenAnimator* pTween = gemData.gem->getTweenAnimator();
    pTween->setTweenPos(
        Tween2D::from(startPos.x, startPos.y)
                .to(endPos.x, endPos.y)
                .during(500)
                .via(tweeny::easing::quadraticInOut)
    );

    pTween->play();
}

// detect and remove matches horizontally and vertically
// - when a match is found this function nulls cell->obj and schedules deletion of game objects
bool GemManager::checkMatches()
{
    bool bMatched = false;

    // horizontal scan
    for (int r = 0; r < (int)this->nHeight; ++r)
    {
        GemType runType = GemType::WHITE;
        int count = 0;

        for (int c = 0; c < (int)this->nWidth; ++c)
        {
            if (this->data[r][c].blocked || this->data[r][c].gem == NULL)
            {
                // gap or blocker; reset current run
                runType = GemType::WHITE;
                count = 0;
                continue;
            }

            GemType currentType = this->data[r][c].gem->getType();

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
                    toRemove.push_back(this->data[r][c].gem);
                    toRemove.push_back(this->data[r][c - 1].gem);
                    toRemove.push_back(this->data[r][c - 2].gem);

                    // clear pointers in grid immediately so cascade won't see them
                    this->data[r][c].gem = NULL;
                    this->data[r][c - 1].gem = NULL;
                    this->data[r][c - 2].gem = NULL;
                    bMatched = true;
                }
                else if (count > 3)
                {
                    // longer run; add current one
                    toRemove.push_back(this->data[r][c].gem);
                    this->data[r][c].gem = NULL;
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

    // vertical scan
    for (int c = 0; c < (int)this->nWidth; ++c)
    {
        GemType runType = GemType::WHITE;
        int count = 0;

        for (int r = 0; r < (int)this->nHeight; ++r)
        {
            if (this->data[r][c].blocked || this->data[r][c].gem == NULL)
            {
                // reset on gap or blocked cell
                runType = GemType::WHITE;
                count = 0;
                continue;
            }

            GemType currentType = this->data[r][c].gem->getType();

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
                    toRemove.push_back(this->data[r][c].gem);
                    toRemove.push_back(this->data[r - 1][c].gem);
                    toRemove.push_back(this->data[r - 2][c].gem);

                    this->data[r][c].gem = NULL;
                    this->data[r - 1][c].gem = NULL;
                    this->data[r - 2][c].gem = NULL;
                    bMatched = true;
                }
                else if (count > 3)
                {
                    toRemove.push_back(this->data[r][c].gem);
                    this->data[r][c].gem = NULL;
                }
            }
            else
            {
                runType = currentType;
                count = 1;
            }
        }
    }

    std::cout << "Checked for matches." << "\n";
    return bMatched;
}

// gravity: drop gems down but never through blocked cells
void GemManager::cascadeDown()
{
    // iterate bottom-to-top so we move lower empty cells first
    for (int r = (int)this->data.size() - 1; r >= 0; --r)
    {
        for (int c = (int)this->data[r].size() - 1; c >= 0; --c)
        {
            if (this->data[r][c].blocked) continue;

            if (this->data[r][c].gem == NULL)
            {
                int indexAbove = r - 1;
                // search upwards for first available gem, stop if a blocked cell is encountered
                while (indexAbove >= 0)
                {
                    if (this->data[indexAbove][c].blocked)
                    {
                        indexAbove = -1;
                        break;
                    }
                    if (this->data[indexAbove][c].gem != NULL) break;
                    --indexAbove;
                }

                if (indexAbove >= 0)
                {
                    // move object pointer down and update its world position
                    this->data[r][c].gem = this->data[indexAbove][c].gem;
                    this->setTween(this->data[r][c]);
                    this->data[indexAbove][c].gem = NULL;
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
    for (Uint8 r = 0; r < this->data.size(); r++)
    {
        std::cout << " |";
        for (Uint8 c = 0; c < this->data[r].size(); c++)
        {
            if (this->data[r][c].blocked)
            {
                std::cout << " * |";
                continue;
            }
            else if (this->data[r][c].gem == NULL)
            {
                std::cout << "   |";
                continue;
            }

            switch (this->data[r][c].gem->getType())
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
        for (Uint8 c = 0; c < this->data[r].size(); c++) std::cout << "--- ";
        std::cout << "\n";
    }
}

void GemManager::spawnGems(float fScale)
{
    this->fGemScale = fScale;
    std::string gemColors[]{ "Yellow", "Blue", "White", "Green", "Purple", "Red" };

    for (int r = 0; r < (int)this->data.size(); ++r)
    {
        for (int c = 0; c < (int)this->data[r].size(); ++c)
        {
            if (!this->data[r][c].blocked)
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
                        this->data[r][c - 1].gem != NULL &&
                        this->data[r][c - 2].gem != NULL)
                    {
                        GemType t1 = this->data[r][c - 1].gem->getType();
                        GemType t2 = this->data[r][c - 2].gem->getType();
                        if (t1 == gemType && t2 == gemType) valid = false;
                    }

                    // check vertical above neighbors
                    if (r >= 2 &&
                        this->data[r - 1][c].gem != NULL &&
                        this->data[r - 2][c].gem != NULL)
                    {
                        auto t1 = this->data[r - 1][c].gem->getType();
                        auto t2 = this->data[r - 2][c].gem->getType();
                        if (t1 == gemType && t2 == gemType)
                            valid = false;
                    }
                }

                // safe to spawn gem now
                Gem* pGem = new Gem("gem_" + std::to_string(r) + "_" + std::to_string(c), gemType);
                pGem->setPos(this->getGemDataPosition(this->data[r][c]));
                pGem->setScale(Vector2D(this->fGemScale));
                this->data[r][c].gem = pGem;
                GameObjectManager::getInstance()->addObject(pGem);
            }
        }
    }
}

void GemManager::finishAnimation()
{
    this->bAnimating = false;

}

void GemManager::destroyMatches()
{
    this->finishAnimation();

    //// delete the collected vertical objects
    //for (auto pGem : toRemove)
    //{
    //    if (pGem != NULL)
    //    {
    //        GemData* gemData = getDataFromGem(pGem);
    //        if (gemData != NULL) gemData->gem = NULL;
    //        GameObjectManager::getInstance()->deleteObject(pGem);
    //    }
    //}

    //this->cascadeDown();
    //this->spawnGems(this->fGemSize);

}

void GemManager::clearSelection()
{

    // clear selection regardless
    this->pSelected[0] = NULL;
    this->pSelected[1] = NULL;

}

void GemManager::setBlocked(Uint8 r, Uint8 c, bool bBlocked)
{
    this->data[r][c].blocked = bBlocked;
}

void GemManager::setBlocked(Uint8 r, const std::vector<Uint8>&cols, bool bBlocked)
{
    if (r > this->data.size() || cols.empty()) return;
    for (Uint8 c : cols)
    {
        if (c <= this->data[r].size())
            this->data[r][c].blocked = bBlocked;
    }
}

Vector2D GemManager::getGemDataPosition(GemData gemData)
{
    Vector2D cellSize = Vector2D(this->fGemSize);
    Vector2D gridSize = Vector2D(this->nWidth, this->nHeight);
    Vector2D pos = cellSize * (Vector2D(gemData.c, gemData.r) - (gridSize * 0.5f));
    pos.y *= -1.0f;
    pos += pOwner->getPos();
    return pos;
}

GemData* GemManager::getDataFromGem(Gem* pGem)
{
    for (Uint8 r = 0; r < this->nHeight; r++)
    {
        for (Uint8 c = 0; c < this->nWidth; c++)
        {
            if (this->data[r][c].gem == pGem) return &this->data[r][c];
        }
    }

    return NULL;
}

GemManager* GemManager::P_SHARED_INSTANCE = NULL;

GemManager::GemManager(Uint8 w, Uint8 h, float fGemSize) : AComponent("GemManager", ComponentType::SCRIPT)
{
    this->nWidth = w;
    this->nHeight = h;
    this->fGemSize = fGemSize;
    this->pSelected[0] = NULL;
    this->pSelected[1] = NULL;
    this->fGemScale = 1.0f;
    this->bAnimating = false;
}

void GemManager::initialize(Uint8 w, Uint8 h, float fGemSize, Vector2D offset)
{
    srand((unsigned)time(nullptr)); // seed once
    EmptyObject* pManagerObject = new EmptyObject("GemManager");
    pManagerObject->setPos(offset);
    P_SHARED_INSTANCE = new GemManager(w, h, fGemSize);
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
