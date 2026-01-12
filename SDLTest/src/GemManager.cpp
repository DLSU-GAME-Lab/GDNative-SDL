#include "GemManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "EventBroadcaster.h"
#include "AudioManager.h"
#include "Sprite.h"

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
    TextureManager::getInstance()->load("Square.png", "Selector");
    TextureManager::getInstance()->load("gems/SP_Gem_Yellow.png", "Yellow");
    TextureManager::getInstance()->load("gems/SP_Gem_Blue.png", "Blue");
    TextureManager::getInstance()->load("gems/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("gems/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("gems/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("gems/SP_Gem_White.png", "White");
    TextureManager::getInstance()->load("Box.png", "Crate");
    TextureManager::getInstance()->load("Small_Bomb.png", "BombS");

	AudioManager::getInstance()->load("sounds/SFX/Swap_SFX.wav", "Swap");
	AudioManager::getInstance()->load("sounds/SFX/Match_SFX_1.wav", "Match_1");
	AudioManager::getInstance()->load("sounds/SFX/Match_SFX_2.wav", "Match_2");
	AudioManager::getInstance()->load("sounds/SFX/Match_SFX_3.wav", "Match_3");
	AudioManager::getInstance()->load("sounds/SFX/Bomb_SFX.wav", "Bomb_SFX");
	AudioManager::getInstance()->load("sounds/SFX/Box_Break.wav", "Break_SFX");

	AudioManager::getInstance()->load("sounds/SFX/Win_SFX.wav", "Win_SFX");
	AudioManager::getInstance()->load("sounds/SFX/Win_Laugh.wav", "Win_Laugh");
	AudioManager::getInstance()->load("sounds/SFX/Lose_SFX.wav", "Lose_SFX");
	AudioManager::getInstance()->load("sounds/SFX/Lose_Sigh.wav", "Lose_Sigh");
}

void GemManager::unloadResources()
{
    TextureManager::getInstance()->unload("Selector");
    TextureManager::getInstance()->unload("Yellow");
    TextureManager::getInstance()->unload("Blue");
    TextureManager::getInstance()->unload("Green");
    TextureManager::getInstance()->unload("Purple");
    TextureManager::getInstance()->unload("Red");
    TextureManager::getInstance()->unload("White");
    TextureManager::getInstance()->unload("Crate");
    TextureManager::getInstance()->unload("BombS");

    AudioManager::getInstance()->unload("Swap");
    AudioManager::getInstance()->unload("Match_1");
    AudioManager::getInstance()->unload("Match_2");
    AudioManager::getInstance()->unload("Match_3");
    AudioManager::getInstance()->unload("Bomb_SFX");
    AudioManager::getInstance()->unload("Break_SFX");

    AudioManager::getInstance()->unload("Win_SFX");
    AudioManager::getInstance()->unload("Win_Laugh");
    AudioManager::getInstance()->unload("Lose_SFX");
    AudioManager::getInstance()->unload("Lose_Laugh");
}

// user selected a gem; handle selection, swapping, checking, and chain reactions
void GemManager::setSelected(Gem* pGem)
{
    if (this->bAnimating || (int)pGem->getType() > (int)GemType::BOMB) return;

    if (this->pSelected[0] == NULL)
    {
        // first selection
        this->pSelected[0] = this->getDataFromGem(pGem);
		this->pSelector->setEnabled(true);
		this->pSelector->setLocalPos(pGem->getPos());
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
        else
        {
            this->pSelected[0] = NULL;
            this->pSelected[1] = NULL;
        }
        this->pSelector->setEnabled(false);
    }
    else
    {
        // deselect if same gem clicked twice
        this->pSelected[0] = NULL;
        this->pSelector->setEnabled(false);
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
    this->pSelected[0]->gem->setGridPosition(this->pSelected[0]->r, this->pSelected[0]->c);
    this->pSelected[1]->gem = objA;
    this->pSelected[1]->gem->setGridPosition(this->pSelected[1]->r, this->pSelected[1]->c);

    this->setTween(*this->pSelected[0], Vector2D(0.0f));
    this->setTween(*this->pSelected[1], Vector2D(0.0f));
    AudioManager::getInstance()->play(new AudioPlayer("Swap", AudioGroupTag::SFX));
}

void GemManager::setTween(GemData gemData, Vector2D startOffset, bool bounce)
{
    Vector2D startPos = gemData.gem->getPos() + startOffset;
    Vector2D endPos = getGemDataPosition(gemData);
    TweenAnimator* pTween = gemData.gem->getTweenAnimator();
    Tween2D tweenPos = Tween2D::from(startPos.x, startPos.y)
        .to(endPos.x, endPos.y)
        .during(500);

    if (bounce) tweenPos = tweenPos.via(tweeny::easing::bounceOut);
    else tweenPos = tweenPos.via(tweeny::easing::quadraticInOut);
    pTween->setTweenPos(tweenPos);

    pTween->play();
}

// detect and remove matches horizontally and vertically
// - when a match is found this function nulls cell->obj and schedules deletion of game objects
bool GemManager::checkMatches()
{
    bool bMatched = false;
    std::unordered_map<std::string, void*> mapParameter;

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
                    this->markForRemoval(this->data[r][c].gem);
                    this->markForRemoval(this->data[r][c - 1].gem);
                    this->markForRemoval(this->data[r][c - 2].gem);

                    bMatched = true;
                    mapParameter["GemType"] = static_cast<void*>(&currentType);
                    mapParameter["RemovedNumber"] = static_cast<void*>(&count);
                    EventBroadcaster::getInstance()->broadcast(EventKey::COLOR_MATCH, mapParameter);

                }
                else if (count > 3)
                {
                    // longer run; add current one
                    this->markForRemoval(this->data[r][c].gem);
                    mapParameter["GemType"] = static_cast<void*>(&currentType);
                    mapParameter["RemovedNumber"] = static_cast<void*>(&count);
                    std::cout << count << std::endl;
                    EventBroadcaster::getInstance()->broadcast(EventKey::COLOR_MATCH, mapParameter);
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
            if (this->data[r][c].blocked ||
                this->data[r][c].gem == NULL ||
                (int)this->data[r][c].gem->getType() > (int)GemType::PURPLE)
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
                    this->markForRemoval(this->data[r][c].gem);
                    this->markForRemoval(this->data[r - 1][c].gem);
                    this->markForRemoval(this->data[r - 2][c].gem);

                    //this->data[r][c].gem = NULL;
                    //this->data[r - 1][c].gem = NULL;
                    //this->data[r - 2][c].gem = NULL;
                    bMatched = true;
                    mapParameter["GemType"] = static_cast<void*>(&currentType);
                    mapParameter["RemovedNumber"] = static_cast<void*>(&count);
                    EventBroadcaster::getInstance()->broadcast(EventKey::COLOR_MATCH, mapParameter);
                }
                else if (count > 3)
                {
                    this->markForRemoval(this->data[r][c].gem);
                    //this->data[r][c].gem = NULL;
                    mapParameter["GemType"] = static_cast<void*>(&currentType);
                    mapParameter["RemovedNumber"] = static_cast<void*>(&count);
                    std::cout << count << std::endl;
                    EventBroadcaster::getInstance()->broadcast(EventKey::COLOR_MATCH, mapParameter);
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

bool GemManager::checkBombs()
{
    std::vector<Gem*> adjGems;
    for (int i = 0; i < 2; i++)
    {
        if (pSelected[i]->gem->getType() == GemType::BOMB)
        {
            this->markForRemoval(pSelected[i]->gem);
            adjGems = this->getAdjacentGems(*pSelected[i]);
            for (auto pGem : adjGems)
            {
                if (pGem == NULL)
                    continue;

                this->markForRemoval(pGem);
			}
            return true;
        }
    }
    return false;
}

void GemManager::markForRemoval(Gem* pGem)
{
    if (std::find(this->toRemove.begin(), this->toRemove.end(), pGem) != this->toRemove.end())
        return;

    this->toRemove.push_back(pGem);
}

// gravity: drop gems down but never through blocked cells
void GemManager::cascadeDown()
{

	//// iterate top-to-bottom so gems fall into lowest available cells first
 //   for (int r = 0; r < this->data.size() - 1; r++)
 //   {
 //       for (int c = 0; c < this->data[r].size(); c++)
 //       {
 //           if (this->data[r][c].blocked ||
 //               this->data[r][c].gem == NULL ||
 //               (int)this->data[r][c].gem->getType() > (int)GemType::BOMB)
 //               continue;

 //           int indexBelow = r + 1;
 //           int cOffset = c;

 //           if (this->data[indexBelow][cOffset].blocked ||
 //               this->data[indexBelow][cOffset].gem != NULL)
 //           {
 //               bool leftBlocked = cOffset > 0 &&
 //                   (this->data[indexBelow][cOffset - 1].blocked ||
 //                   this->data[indexBelow][cOffset - 1].gem != NULL);

 //               bool rightBlocked = cOffset < this->data[r].size() - 1 &&
 //                   (this->data[indexBelow][cOffset + 1].blocked ||
 //                   this->data[indexBelow][cOffset + 1].gem != NULL);

 //               if (!leftBlocked && !rightBlocked)
 //               {
	//				cOffset += bool(rand() % 2) ? -1 : 1;
 //               }
 //               else if (!leftBlocked) cOffset -= 1;
 //               else if (!rightBlocked) cOffset += 1;
 //               else continue; // both sides blocked; cannot move
 //           }

	//		if (cOffset < 0 || cOffset >= this->data[r].size()) continue;
 //           // move object pointer up and update its world position
 //           this->data[indexBelow][cOffset].gem = this->data[r][c].gem;
 //           this->setTween(this->data[r][c], Vector2D(0.0f), true);
 //           this->data[r][c].gem = NULL;
 //       }
 //   }

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
                    if (this->data[indexAbove][c].gem != NULL) 
                    {
                        if ((int)this->data[indexAbove][c].gem->getType() >= (int)GemType::CRATE_0)
                            indexAbove = -1;
                        
                        break;
                    }
                    --indexAbove;
                }

                if (indexAbove >= 0)
                {
                    // move object pointer down and update its world position
                    this->data[r][c].gem = this->data[indexAbove][c].gem;
                    this->data[r][c].gem->setGridPosition(r, c);
                    this->setTween(this->data[r][c], Vector2D(0.0f), true);
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
            case GemType::BOMB:
                std::cout << " @ |";
                break;
            case GemType::CRATE_0:
                std::cout << " 0 |";
                break;
            case GemType::CRATE_1:
                std::cout << " 1 |";
                break;
            case GemType::CRATE_2:
                std::cout << " 2 |";
                break;
            }
        }
        std::cout << "\n  ";
        for (Uint8 c = 0; c < this->data[r].size(); c++) std::cout << "--- ";
        std::cout << "\n";
    }
}

std::vector<Gem*> GemManager::getAdjacentGems(const GemData gemData)
{
    std::vector<Gem*> adjGems;
    std::vector<Gem*> adjVert = this->getAdjacentVerticalGems(gemData);
    std::vector<Gem*> adjHori = this->getAdjacentHorizontalGems(gemData);

    adjGems = adjVert;
    adjGems.insert(adjGems.end(), adjHori.begin(), adjHori.end());

    return adjGems;
}

std::vector<Gem*> GemManager::getAdjacentVerticalGems(const GemData gemData)
{
    std::vector<Gem*> adjGems;
    const int r = gemData.r;
    const int c = gemData.c;

    if (c > 0)
        adjGems.push_back(data[r][c - 1].gem);

    if (c < data[r].size() - 1)
        adjGems.push_back(data[r][c + 1].gem);

    return adjGems;
}

std::vector<Gem*> GemManager::getAdjacentHorizontalGems(const GemData gemData)
{
    std::vector<Gem*> adjGems;
    const int r = gemData.r;
    const int c = gemData.c;

    if (r > 0)
        adjGems.push_back(data[r - 1][c].gem);

    if (r < data.size() - 1)
        adjGems.push_back(data[r + 1][c].gem);

    return adjGems;
}

void GemManager::spawnGems(float fScale)
{
    this->fGemScale = fScale;

    for (int r = (int)this->data.size() - 1; r >= 0; --r)
    {
        for (int c = 0; c < (int)this->data[r].size(); ++c)
        {
            if (!this->data[r][c].blocked && this->data[r][c].gem == NULL)
            {
                GemType gemType;
                bool valid = false;

                // keep rerolling until no 3-match on spawn
                while (!valid)
                {
                    int gemChance = rand() % 20;
                    if (gemChance == 0)
                    {
                        int bombChance = rand() % 2;
						if (bombChance == 0)
                            gemType = GemType::BOMB;
						else
                            gemType = GemType::CRATE_2;
					}
                    else
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
                Gem* pGem = new Gem("gem_" + std::to_string(this->nGemNum), gemType);
                this->nGemNum++;
                this->data[r][c].gem = pGem;

                pGem->setPos(this->getGemDataPosition(this->data[0][c]));
                pGem->setScale(Vector2D(this->fGemScale));
                pGem->setGridPosition(r, c);
                GameObjectManager::getInstance()->addObject(pGem);

                this->setTween(this->data[r][c], Vector2D(0.0f, this->fGemSize), true);
                //pause after each row
            }
        }
    }
}

void GemManager::updateBoard()
{
	const bool hasBombs = this->checkBombs();
	const bool hasMatches = this->checkMatches();
    if (hasBombs || hasMatches)
    {
        EventBroadcaster::getInstance()->broadcast(EventKey::COUNTER_SUBTRACT);
        do
        {
            for (auto pGem : toRemove)
            {
                if (pGem != NULL)
                {
                    GemData* gemData = getDataFromGem(pGem);
                    if (gemData != NULL)
                    {
                        // Checks for crates around the gems being removed
						std::vector<Gem*> pAdjGems = this->getAdjacentGems(*gemData);
                        for (auto pAdjGem : pAdjGems)
                        {
                            if (pAdjGem != NULL)
                            {
                                int typeNum = (int)pAdjGem->getType();
                                if (typeNum > (int)GemType::CRATE_0)
                                {
                                    pAdjGem->setType(GemType(typeNum - 1));
                                    AudioManager::getInstance()->play(new AudioPlayer("Break_SFX", AudioGroupTag::SFX));
									std::cout << "Crate downgraded to type " << (typeNum - 1) << "\n";
                                }
                                else if (pAdjGem->getType() == GemType::CRATE_0)
                                {
									GemData* adjGemData = getDataFromGem(pAdjGem);
                                    adjGemData->gem = NULL;
                                    AudioManager::getInstance()->play(new AudioPlayer("Break_SFX", AudioGroupTag::SFX));
                                    GameObjectManager::getInstance()->deleteObject(pAdjGem);
                                }
                            }
                        }
                        gemData->gem = NULL;
                    }
                    GameObjectManager::getInstance()->deleteObject(pGem);
                }
            }
            toRemove.clear();

            if (hasBombs)
            {
                AudioManager::getInstance()->play(new AudioPlayer("Bomb_SFX", AudioGroupTag::SFX));
			}
            else
            {
                std::string sfxName = "Match_";
                sfxName += std::to_string((rand() % 3) + 1);
                AudioManager::getInstance()->play(new AudioPlayer(sfxName, AudioGroupTag::SFX));
            }

            this->cascadeDown();
            this->spawnGems(this->fGemScale);

        } while (this->checkMatches());
        AGameObject* pHolder = GameObjectManager::getInstance()->findObjectByName("EndScreen");
        GameObjectManager::getInstance()->sortObjectToEnd(pHolder);
    }
    else this->moveGems();

    // clear selection regardless
    this->pSelected[0] = NULL;
    this->pSelected[1] = NULL;
}

void GemManager::finishAnimation()
{
    this->bAnimating = false;

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
    if (pGem->getRow() < this->data.size() &&
        pGem->getCol() < this->data[pGem->getRow()].size())
        return &this->data[pGem->getRow()][pGem->getCol()];

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
    this->nGemNum = 0;
    this->pSelector = new Sprite("Selector", "Selector", Vector2D(0.0f), Vector2D(0.12f));
	this->pSelector->setEnabled(false);
	GameObjectManager::getInstance()->addObject(this->pSelector);
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
