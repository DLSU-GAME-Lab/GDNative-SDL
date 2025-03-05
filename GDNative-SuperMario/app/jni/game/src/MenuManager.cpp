#include "MenuManager.h"
#include "GDCore.h"
#include "TouchManager.h"

/* ******************************************** */

MenuManager::MenuManager(void) {
    this->currentGameState = eMainMenu;

    this->oMainMenu = new MainMenu();
    this->oLoadingMenu = new LoadingMenu();
    this->oAboutMenu = new AboutMenu();
    this->oOptionsMenu = new OptionsMenu();
    this->oPauseMenu = new PauseMenu();
}


MenuManager::~MenuManager(void) {
    delete activeOption;
    delete oMainMenu;
    delete oLoadingMenu;
    delete oAboutMenu;
}

/* ******************************************** */

void MenuManager::Update() {
    switch(currentGameState) {
        case eMainMenu:
            oMainMenu->Update();
            GDCore::getMap()->UpdateBlocks();
            break;
        case eGameLoading:
            oLoadingMenu->Update();
            break;
        case eGame:
            GDCore::getMap()->Update();
            GDCore::getMap()->UpdateMinionsCollisions();
            break;
        case eAbout:
            GDCore::getMap()->UpdateMinions();
            GDCore::getMap()->UpdateMinionBlokcs();
            GDCore::getMap()->UpdateBlocks();
            oAboutMenu->Update();
            break;
        case eOptions:
            oOptionsMenu->Update();
            break;
        case ePasue:
            oPauseMenu->Update();
            break;
    }
}

void MenuManager::Draw(SDL_Renderer* rR) {
    switch(currentGameState) {
        case eMainMenu:
            GDCore::getMap()->DrawMap(rR);
            GDCore::getMap()->getPlayer()->Draw(rR);
            GDCore::getMap()->DrawGameLayout(rR);
            oMainMenu->Draw(rR);
            break;
        case eGameLoading:
            oLoadingMenu->Draw(rR);
            break;
        case eGame:
            GDCore::getMap()->Draw(rR);
            break;
        case eAbout:
            GDCore::getMap()->DrawMap(rR);
            GDCore::getMap()->DrawMinions(rR);
            oAboutMenu->Draw(rR);
            break;
        case eOptions:
            GDCore::getMap()->DrawMap(rR);
            GDCore::getMap()->DrawMinions(rR);
            GDCore::getMap()->getPlayer()->Draw(rR);
            GDCore::getMap()->DrawGameLayout(rR);
            oOptionsMenu->Draw(rR);
            break;
        case ePasue:
            GDCore::getMap()->DrawMap(rR);
            GDCore::getMap()->DrawMinions(rR);
            GDCore::getMap()->getPlayer()->Draw(rR);
            GDCore::getMap()->DrawGameLayout(rR);
            oPauseMenu->Draw(rR);
            break;
    }
}

void MenuManager::setBackgroundColor(SDL_Renderer* rR) {
    switch(currentGameState) {
        case eMainMenu:
            GDCore::getMap()->setBackgroundColor(rR);
            break;
        case eGameLoading:
            SDL_SetRenderDrawColor(rR, 0, 0, 0, 255);
            break;
        case eGame:
            GDCore::getMap()->setBackgroundColor(rR);
            break;
        case eAbout:
            oAboutMenu->setBackgroundColor(rR);
            break;
    }
}

/* ******************************************** */

// New function to update touch controls based on game state
// Updated MenuManager function with correct -> syntax

void MenuManager::updateTouchControlsForGameState(gameState state) {
    switch (state) {
        case eGame:
            // Show all touch controls during gameplay
            TouchManager::getInstance()->setAllTouchAreasVisible(true);
            TouchManager::getInstance()->setAllTouchAreasEnabled(true);
            break;

        case eMainMenu:
            // In main menu, only show dpad and A button for navigation
            TouchManager::getInstance()->setAllTouchAreasVisible(false);
            TouchManager::getInstance()->setAllTouchAreasEnabled(false);

            // Enable D-pad for menu navigation
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_UP, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_DOWN, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_LEFT, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_RIGHT, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_UP, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_DOWN, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_LEFT, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_RIGHT, true);

            // Enable A button for selection
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::BUTTON_A, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::BUTTON_A, true);
            break;

        case ePasue:
            // In pause menu, only show dpad and A/B buttons
            TouchManager::getInstance()->setAllTouchAreasVisible(false);
            TouchManager::getInstance()->setAllTouchAreasEnabled(false);

            // Enable D-pad for menu navigation
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_UP, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_DOWN, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_UP, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_DOWN, true);

            // Enable A/B buttons for selection/back
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::BUTTON_A, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::BUTTON_B, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::BUTTON_A, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::BUTTON_B, true);
            break;

        case eOptions:
            // In options menu, show navigation controls
            TouchManager::getInstance()->setAllTouchAreasVisible(false);
            TouchManager::getInstance()->setAllTouchAreasEnabled(false);

            // Enable D-pad for menu navigation
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_UP, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_DOWN, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_LEFT, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::DPAD_RIGHT, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_UP, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_DOWN, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_LEFT, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::DPAD_RIGHT, true);

            // Enable A/B buttons for selection/back
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::BUTTON_A, true);
            TouchManager::getInstance()->setTouchAreaVisible(TouchControlID::BUTTON_B, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::BUTTON_A, true);
            TouchManager::getInstance()->setTouchAreaEnabled(TouchControlID::BUTTON_B, true);
            break;

        default:
            // Hide all touch controls for other states
            TouchManager::getInstance()->setAllTouchAreasVisible(false);
            TouchManager::getInstance()->setAllTouchAreasEnabled(false);
            break;
    }
}

void MenuManager::enter() {
    switch(currentGameState) {
        case eMainMenu:
            oMainMenu->enter();
            break;
        case eGame:
            GDCore::getMap()->setDrawLines(!GDCore::getMap()->getDrawLines());
            break;
        case eAbout:
            oAboutMenu->enter();
            break;
        case eOptions:
            oOptionsMenu->enter();
            break;
        case ePasue:
            oPauseMenu->enter();
            break;
    }
}

void MenuManager::escape() {
    switch(currentGameState) {
        case eGame:

            break;
        case eAbout:
            oAboutMenu->enter();
            break;
        case eOptions:
            oOptionsMenu->escape();
            break;
        case ePasue:
            oPauseMenu->escape();
            break;
        case eMainMenu:
            oMainMenu->escape();
            break;
    }
}

void MenuManager::setKey(int keyID) {
    switch(currentGameState) {
        case eOptions:
            oOptionsMenu->setKey(keyID);
            break;
    }
}

void MenuManager::keyPressed(int iDir) {
    switch(currentGameState) {
        case eMainMenu:
            oMainMenu->updateActiveButton(iDir);
            break;
        case eOptions:
            oOptionsMenu->updateActiveButton(iDir);
            break;
        case ePasue:
            oPauseMenu->updateActiveButton(iDir);
            break;
    }
}

void MenuManager::resetActiveOptionID(gameState ID) {
    switch(ID) {
        case eMainMenu:
            oMainMenu->activeMenuOption = 0;
            break;
        case eOptions:
            oOptionsMenu->activeMenuOption = 0;
            break;
        case ePasue:
            oPauseMenu->activeMenuOption = 0;
            break;
    }
}

/* ******************************************** */

int MenuManager::getViewID() {
    return currentGameState;
}

void MenuManager::setViewID(gameState viewID) {
    this->currentGameState = viewID;

    // Update touch controls for the new game state
    updateTouchControlsForGameState(viewID);
}

CIMG* MenuManager::getActiveOption() {
    return activeOption;
}

void MenuManager::setActiveOption(SDL_Renderer* rR) {
    activeOption = new CIMG("active_option", rR);
}

LoadingMenu* MenuManager::getLoadingMenu() {
    return oLoadingMenu;
}

AboutMenu* MenuManager::getAboutMenu() {
    return oAboutMenu;
}

OptionsMenu* MenuManager::getOptions() {
    return oOptionsMenu;
}