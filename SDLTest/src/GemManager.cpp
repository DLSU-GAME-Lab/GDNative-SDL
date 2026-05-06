#include "GemManager.h"
#include "EmptyObject.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "EventBroadcaster.h"
#include "AudioManager.h"
#include "StoreManager.h"
#include "Sprite.h"

#include "GUIUtils.h"
#include "GUIButton.h"
#include "SceneSwitcher.h"
#include "TurnCounter.h"
#include "Background.h"

#include "GUIToggle.h"

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
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_bg.png", "Level_Background");
    TextureManager::getInstance()->load("title_screen_pngs/Sprite_Window.png", "UI_Container");
    TextureManager::getInstance()->load("title_screen_pngs/UI_mermaid.png", "UI_Mermaid");
    TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_2.png", "Top_UI_Container");
    TextureManager::getInstance()->load("Menu/Sprite_Close.png", "Close_Button");
    TextureManager::getInstance()->load("Menu/Sprite_UI_setupicon.png", "Settings");
    TextureManager::getInstance()->load("Menu/Sprite_UI_TopBar_BG_round.png", "Level_Container_Extra");
    TextureManager::getInstance()->load("Menu/Sprite_UI_booster.png", "UI_Booster");
    TextureManager::getInstance()->load("Menu/Sprite_UI_Avatar_frame.png", "Avatar_Frame");
    TextureManager::getInstance()->load("Menu/UI_Panel.png", "UI_Panel");
    TextureManager::getInstance()->load("Menu/Sprite_ribbon_victory.png", "Victory_Ribbon");
    TextureManager::getInstance()->load("Menu/Sprite_ribbon_defeat.png", "Defeat_Ribbon");
    TextureManager::getInstance()->load("Menu/Sprite_Button_green.png", "Green_Button");
    TextureManager::getInstance()->load("title_screen_pngs/Background_Objects.png", "Design_BG");
    TextureManager::getInstance()->load("Sprite_Bubble_Small.png", "Bubble");
    TextureManager::getInstance()->load("Color_Target.png", "Color_Target");
    TextureManager::getInstance()->load("Eliminate_Hori.png", "Eliminate_Hori");
    TextureManager::getInstance()->load("Eliminate_Vert.png", "Eliminate_Vert");
    TextureManager::getInstance()->load("Small_Bomb.png", "BombS");


    TextureManager::getInstance()->load("Square.png", "Selector");
    TextureManager::getInstance()->load("gems/SP_Gem_Yellow.png", "Yellow");
    TextureManager::getInstance()->load("gems/SP_Gem_Blue.png", "Blue");
    TextureManager::getInstance()->load("gems/SP_Gem_Green.png", "Green");
    TextureManager::getInstance()->load("gems/SP_Gem_Purple.png", "Purple");
    TextureManager::getInstance()->load("gems/SP_Gem_Red.png", "Red");
    TextureManager::getInstance()->load("gems/SP_Gem_White.png", "White");
    TextureManager::getInstance()->load("Box.png", "Crate");

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
    TextureManager::getInstance()->unload("Level_Background");
    TextureManager::getInstance()->unload("Lower_UI_Container");
    TextureManager::getInstance()->unload("UI_Mermaid");
    TextureManager::getInstance()->unload("Settings");
    TextureManager::getInstance()->unload("Avatar_Frame");
    TextureManager::getInstance()->unload("UI_Panel");
    TextureManager::getInstance()->unload("UI_Booster");
    TextureManager::getInstance()->unload("Design_BG");
    TextureManager::getInstance()->unload("Level_Container");
    TextureManager::getInstance()->unload("Level_Container_Extra");
    TextureManager::getInstance()->unload("Top_UI_Container");
    TextureManager::getInstance()->unload("Close_Button");
    TextureManager::getInstance()->unload("Victory_Ribbon");
    TextureManager::getInstance()->unload("Defeat_Ribbon");
    TextureManager::getInstance()->unload("Green_Button");
    TextureManager::getInstance()->unload("Bubble");
    TextureManager::getInstance()->unload("BombS");

    TextureManager::getInstance()->unload("Selector");
    TextureManager::getInstance()->unload("Yellow");
    TextureManager::getInstance()->unload("Blue");
    TextureManager::getInstance()->unload("Green");
    TextureManager::getInstance()->unload("Purple");
    TextureManager::getInstance()->unload("Red");
    TextureManager::getInstance()->unload("White");
    TextureManager::getInstance()->unload("Crate");

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

void GemManager::loadGUI()
{
    Sprite* pLowerUIContainer = new Sprite("Lower_UI_Container", "UI_Container", Vector2D(0, 0), Vector2D(10, 2), 0.0f, false);
    GUIUtils::setGUIBotCenter(pLowerUIContainer, Vector2D(-20));
    GameObjectManager::getInstance()->addObject(pLowerUIContainer);

	std::vector<Vector2D> boosterPositions;

    for (int i = 0; i < 4; i++)
    {
        Sprite* pBooster = new Sprite("Booster_" + std::to_string(i), "UI_Booster", Vector2D(0, 0), Vector2D(0.7f), 0.0f, false);
		boosterPositions.push_back(Vector2D(80 + (i * 130), -70));
        GUIUtils::setGUIBotLeft(pBooster, boosterPositions[i]);
        GameObjectManager::getInstance()->addObject(pBooster);
    }

	const Vector2D posOffset(-6, -2);
    Sprite* pBombPowerup = new Sprite("Bomb_Powerup", "BombS", Vector2D(0, 0), Vector2D(0.4f), 0.0f, false);
    GUIUtils::setGUIBotLeft(pBombPowerup, boosterPositions[0] + posOffset);
    GameObjectManager::getInstance()->addObject(pBombPowerup);
    StoreManager::getInstance()->addStoreItem("Bomb_Powerup",50,"BombS");

    Sprite* pVertPowerup = new Sprite("Vert_Powerup", "Eliminate_Vert", Vector2D(0, 0), Vector2D(0.4f), 0.0f, false);
    GUIUtils::setGUIBotLeft(pVertPowerup, boosterPositions[1] + posOffset);
    GameObjectManager::getInstance()->addObject(pVertPowerup);
    StoreManager::getInstance()->addStoreItem("Vert_Powerup", 50, "Eliminate_Vert");


    Sprite* pHoriPowerup = new Sprite("Hori_Powerup", "Eliminate_Hori", Vector2D(0, 0), Vector2D(0.4f), 0.0f, false);
    GUIUtils::setGUIBotLeft(pHoriPowerup, boosterPositions[2] + posOffset);
    GameObjectManager::getInstance()->addObject(pHoriPowerup);
    StoreManager::getInstance()->addStoreItem("Hori_Powerup", 50, "Eliminate_Hori");

    Sprite* pColorPowerup = new Sprite("Color_Powerup", "Color_Target", Vector2D(0, 0), Vector2D(0.35f), 0.0f, false);
    GUIUtils::setGUIBotLeft(pColorPowerup, boosterPositions[3] + posOffset);
    GameObjectManager::getInstance()->addObject(pColorPowerup);
    StoreManager::getInstance()->addStoreItem("Color_Powerup", 50, "Color_Target");


    GUIButton* pSettings = new GUIButton("Settings", "Settings");
    GUIUtils::setGUIBotRight(pSettings, Vector2D(-40, -90));
    pSettings->setScale(Vector2D(.5f, .5f));
	GUIToggle* pShopToggle = new GUIToggle(EventKey::SHOP_TOGGLE);
    pSettings->attachComponent(pShopToggle);
    GameObjectManager::getInstance()->addObject(pSettings);
    //SceneSwitcher* pTitleSwitcher = new SceneSwitcher(SceneTag::TITLE_SCENE);
    //pSettings->attachComponent(pTitleSwitcher);

    Sprite* pUpperUIContainer = new Sprite("Upper_UI_Container", "Top_UI_Container", Vector2D(0, 0), Vector2D(100, 1.25f), 0.0f, false);
    GUIUtils::setGUITopCenter(pUpperUIContainer, Vector2D(0, 40));
    GameObjectManager::getInstance()->addObject(pUpperUIContainer);

    Sprite* pUIContainerExtra = new Sprite("Level_Container_Extra", "Level_Container_Extra", Vector2D(0, 0), Vector2D(0.8f, 1), 0.0f, false);
    GUIUtils::setGUITopCenter(pUIContainerExtra, Vector2D(0, 40));
    GameObjectManager::getInstance()->addObject(pUIContainerExtra);

    Sprite* pUIAvatarFrame = new Sprite("Avatar_Frame", "Avatar_Frame", Vector2D(0.0f), Vector2D(0.7f), 0.0f, false);
    GUIUtils::setGUITopCenter(pUIAvatarFrame, Vector2D(0, 120));
    GameObjectManager::getInstance()->addObject(pUIAvatarFrame);

    Sprite* pUIMermaid = new Sprite("UI_Mermaid", "UI_Mermaid", Vector2D(0.0f), Vector2D(0.5f), 0.0f, false);
    GUIUtils::setGUITopCenter(pUIMermaid, Vector2D(0, 120));
    GameObjectManager::getInstance()->addObject(pUIMermaid);

    Sprite* pUIGemsPanel = new Sprite("Gems_Panel", "UI_Panel", Vector2D(0.0f), Vector2D(1.2f), 0.0f, false);
    GUIUtils::setGUITopLeft(pUIGemsPanel, Vector2D(100, 80));
    GameObjectManager::getInstance()->addObject(pUIGemsPanel);

    Sprite* pUIMovesPanel = new Sprite("Moves_Panel", "UI_Panel", Vector2D(0.0f), Vector2D(1.2f), 0.0f, false);
    GUIUtils::setGUITopRight(pUIMovesPanel, Vector2D(-100, 80));
    GameObjectManager::getInstance()->addObject(pUIMovesPanel);

    TurnCounter* pTurnCount = new TurnCounter("TurnCounter", 20, Vector2D(210, 450), Vector2D(1, 1));
    GameObjectManager::getInstance()->addObject(pTurnCount);

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

void GemManager::placeGem(GemType EType, Uint8 r, Uint8 c)
{
    Gem* pGem = new Gem("gem_" + std::to_string(this->nGemNum), EType);
    this->nGemNum++;
    this->data[r][c].gem = pGem;

    pGem->setPos(this->getGemDataPosition(this->data[0][c]));
    pGem->setScale(Vector2D(this->fGemScale));
    pGem->setGridPosition(r, c);
    GameObjectManager::getInstance()->addObject(pGem);

    this->setTween(this->data[r][c], Vector2D(0.0f, this->fGemSize), true);
}

void GemManager::spawnGems()
{
    for (int r = (int)this->data.size() - 1; r >= 0; --r)
    {
        for (int c = 0; c < (int)this->data[r].size(); ++c)
        {
            if (!this->data[r][c].blocked && this->data[r][c].gem == NULL)
            {
                GemType gemType = GemType::WHITE;
                bool valid = false;

                // keep rerolling until no 3-match on spawn
                while (!valid)
                {
                    int randChance = rand() % 40;
                    if ((this->bSpawnCratesAuto ||
                        this->bSpawnBombsAuto) &&
                        randChance < 2)
                    {
						if (this->bSpawnBombsAuto &&
                            randChance == 0)
                            gemType = GemType::BOMB;
						else if (this->bSpawnCratesAuto &&
                            randChance == 1)
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
				this->placeGem(gemType, r, c);
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
            this->spawnGems();

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

void GemManager::setSpawnCratesAuto(bool bSpawnCratesAuto)
{
    this->bSpawnCratesAuto = bSpawnCratesAuto;
}

void GemManager::setSpawnBombsAuto(bool bSpawnBombsAuto)
{
    this->bSpawnBombsAuto = bSpawnBombsAuto;
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

GemManager::GemManager(Uint8 w, Uint8 h, float fGemSize, float fGemScale) : AComponent("GemManager", ComponentType::SCRIPT)
{
    this->nWidth = w;
    this->nHeight = h;
    this->fGemSize = fGemSize;
    this->pSelected[0] = NULL;
    this->pSelected[1] = NULL;
    this->fGemScale = fGemScale;
    this->bAnimating = false;
    this->nGemNum = 0;
    this->bSpawnCratesAuto = false;
    this->bSpawnBombsAuto = false;
    this->pSelector = new Sprite("Selector", "Selector", Vector2D(0.0f), Vector2D(0.12f));
	this->pSelector->setEnabled(false);
	GameObjectManager::getInstance()->addObject(this->pSelector);
}

void GemManager::initialize(Uint8 w, Uint8 h, float fGemSize, float fGemScale, Vector2D offset)
{
    srand((unsigned)time(nullptr)); // seed once
    EmptyObject* pManagerObject = new EmptyObject("GemManager");
    pManagerObject->setPos(offset);
    P_SHARED_INSTANCE = new GemManager(w, h, fGemSize, fGemScale);
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
