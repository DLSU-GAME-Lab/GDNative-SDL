//
// Created by delgallegon on 02/01/2025.
//
#include "header.h"
#include "GDCore.h"
#include "IMG.h"
#include "CFG.h"
#include "Text.h"
#include "SDL_mixer.h"

#include <android/log.h>

Map* GDCore::oMap = new Map();
bool GDCore::mouseLeftPressed = false;
bool GDCore::mouseRightPressed = false;
int GDCore::mouseX = 0;
int GDCore::mouseY = 0;
bool GDCore::quitGame = false;

bool GDCore::movePressed = false;
bool GDCore::keyMenuPressed = false;
bool GDCore::keyS = false;
bool GDCore::keyW = false;
bool GDCore::keyA = false;
bool GDCore::keyD = false;
bool GDCore::keyShift = false;
bool GDCore::keyAPressed = false;
bool GDCore::keyDPressed = false;

GDCore::GDCore(void) {
    this->quitGame = false;
    this->iFPS = 0;
    this->iNumOfFPS = 0;
    this->lFPSTime = 0;

//    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);

    // Get screen dimensions
    int screenWidth, screenHeight;
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);
    screenWidth = displayMode.w;
    screenHeight = displayMode.h;

    // Calculate scale factors
    float scaleX = (float)screenWidth / CCFG::GAME_WIDTH;
    float scaleY = (float)screenHeight / CCFG::GAME_HEIGHT;
    float scale = std::min(scaleX, scaleY);

    // Calculate window dimensions
    int windowWidth = CCFG::GAME_WIDTH * scale;
    int windowHeight = CCFG::GAME_HEIGHT * scale;

    window = SDL_CreateWindow("uMario - DLSU GAME LAB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        quitGame = true;
    }

    rR = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(rR, CCFG::GAME_WIDTH, CCFG::GAME_HEIGHT);
//    SDL_RenderSetScale(rR, 2.4f, 2.4f);


//     ----- ICO
    std::string fileName = "files/images/ico.bmp";
    SDL_Surface* loadedSurface = SDL_LoadBMP(fileName.c_str());
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));

    SDL_SetWindowIcon(window, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    mainEvent = new SDL_Event();
    // ----- ICO

//    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
//
    oMap = new Map(rR);
    CCFG::getMM()->setActiveOption(rR);
    CCFG::getSMBLOGO()->setIMG("super_mario_bros", rR);
//
    CCFG::getMusic()->PlayMusic();

    this->keyMenuPressed = this->movePressed = this->keyS = this->keyW = this->keyA = this->keyD = this->keyShift = false;
    this->keyAPressed = this->keyDPressed = this->firstDir = false;
    this->mouseX = this->mouseY = 0;

    CCFG::keyIDA = SDLK_a;
    CCFG::keyIDS = SDLK_s;
    CCFG::keyIDD = SDLK_d;
    CCFG::keyIDSpace = SDLK_SPACE;
    CCFG::keyIDShift = SDLK_LSHIFT;

    CCFG::getMusic()->LoadAllMusic();

    // ----- ImGui
    InitializeTouchVariables();
    InitializeImGui();
}

GDCore::~GDCore(void) {
    delete oMap;
    delete mainEvent;
    SDL_DestroyRenderer(rR);
    SDL_DestroyWindow(window);

    // Destroy ImGui
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

/* ******************************************** */

void GDCore::mainLoop() {
    lFPSTime = SDL_GetTicks();

    while(!quitGame && mainEvent->type != SDL_QUIT) {
        frameTime = SDL_GetTicks();
        SDL_PollEvent(mainEvent);
        SDL_RenderClear(rR);

        CCFG::getMM()->setBackgroundColor(rR);
        SDL_RenderFillRect(rR, NULL);

        Input();
        MouseInput();
        TouchInput();
        Update();
        Draw();

        /*CCFG::getText()->Draw(rR, "FPS:" + std::to_string(iNumOfFPS), CCFG::GAME_WIDTH - CCFG::getText()->getTextWidth("FPS:" + std::to_string(iNumOfFPS), 8) - 8, 5, 8);

        if(SDL_GetTicks() - 1000 >= lFPSTime) {
            lFPSTime = SDL_GetTicks();
            iNumOfFPS = iFPS;
            iFPS = 0;
        }

        ++iFPS;*/

        SDL_RenderPresent(rR);

        if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
            SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
        }

        this->ticks = SDL_GetTicks64();
        if (this->ticks % 50 == 0) {
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Total ticks so far: %d", this->ticks);
        }
    }
}

void GDCore::Input() {
    switch(CCFG::getMM()->getViewID()) {
        case 2: case 7:
            if(!oMap->getInEvent()) {
                InputPlayer();
            } else {
                resetMove();
            }
            break;
        default:
            InputMenu();
            break;
    }
}

void GDCore::InputMenu() {
    if(mainEvent->type == SDL_KEYDOWN) {
        CCFG::getMM()->setKey(mainEvent->key.keysym.sym);

        switch(mainEvent->key.keysym.sym) {
            case SDLK_s: case SDLK_DOWN:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(2);
                    keyMenuPressed = true;
                }
                break;
            case SDLK_w: case SDLK_UP:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(0);
                    keyMenuPressed = true;
                }
                break;
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(!keyMenuPressed) {
                    CCFG::getMM()->enter();
                    keyMenuPressed = true;
                }
                break;
            case SDLK_ESCAPE:
                if(!keyMenuPressed) {
                    CCFG::getMM()->escape();
                    keyMenuPressed = true;
                }
                break;
            case SDLK_LEFT: case SDLK_d:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(3);
                    keyMenuPressed = true;
                }
                break;
            case SDLK_RIGHT: case SDLK_a:
                if(!keyMenuPressed) {
                    CCFG::getMM()->keyPressed(1);
                    keyMenuPressed = true;
                }
                break;
        }
    }

    if(mainEvent->type == SDL_KEYUP) {
        switch(mainEvent->key.keysym.sym) {
            case SDLK_s: case SDLK_DOWN: case SDLK_w: case SDLK_UP: case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE: case SDLK_a: case SDLK_RIGHT: case SDLK_LEFT: case SDLK_d:
                keyMenuPressed = false;
                break;
            default:
                break;
        }
    }
}

void GDCore::InputPlayer() {
    if(mainEvent->type == SDL_WINDOWEVENT) {
        switch(mainEvent->window.event) {
            case SDL_WINDOWEVENT_FOCUS_LOST:
                CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePause);
                CCFG::getMM()->setViewID(CCFG::getMM()->ePause);
                CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
                CCFG::getMusic()->PauseMusic();
                break;
        }
    }

    if(mainEvent->type == SDL_KEYUP) {
        if(mainEvent->key.keysym.sym == CCFG::keyIDD) {
            if(firstDir) {
                firstDir = false;
            }

            keyDPressed = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDS) {
            oMap->getPlayer()->setSquat(false);
            keyS = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDA) {
            if(!firstDir) {
                firstDir = true;
            }

            keyAPressed = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDSpace) {
            CCFG::keySpace = false;
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDShift) {
            if(keyShift) {
                oMap->getPlayer()->resetRun();
                keyShift = false;
            }
        }
        switch(mainEvent->key.keysym.sym) {
            case SDLK_KP_ENTER: case SDLK_RETURN: case SDLK_ESCAPE:
                keyMenuPressed = false;
                break;
        }
    }

    if(mainEvent->type == SDL_KEYDOWN) {
        if(mainEvent->key.keysym.sym == CCFG::keyIDD) {
            keyDPressed = true;
            if(!keyAPressed) {
                firstDir = true;
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDS) {
            if(!keyS) {
                keyS = true;
                if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation()) oMap->getPlayer()->setSquat(true);
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDA) {
            keyAPressed = true;
            if(!keyDPressed) {
                firstDir = false;
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDSpace) {
            if(!CCFG::keySpace) {
                oMap->getPlayer()->jump();
                CCFG::keySpace = true;
            }
        }

        if(mainEvent->key.keysym.sym == CCFG::keyIDShift) {
            if(!keyShift) {
                oMap->getPlayer()->startRun();
                keyShift = true;
            }
        }

        switch(mainEvent->key.keysym.sym) {
            case SDLK_KP_ENTER: case SDLK_RETURN:
                if(!keyMenuPressed) {
                    CCFG::getMM()->enter();
                    keyMenuPressed = true;
                }
            case SDLK_ESCAPE:
                if(!keyMenuPressed && CCFG::getMM()->getViewID() == CCFG::getMM()->eGame) {
                    CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePause);
                    CCFG::getMM()->setViewID(CCFG::getMM()->ePause);
                    CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
                    CCFG::getMusic()->PauseMusic();
                    keyMenuPressed = true;
                }
                break;
        }
    }

    if(keyAPressed) {
        if(!oMap->getPlayer()->getMove() && firstDir == false && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
            oMap->getPlayer()->startMove();
            oMap->getPlayer()->setMoveDirection(false);
        } else if(!keyDPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
            oMap->getPlayer()->setChangeMoveDirection();
        }
    }

    if(keyDPressed) {
        if(!oMap->getPlayer()->getMove() && firstDir == true && !oMap->getPlayer()->getChangeMoveDirection() && !oMap->getPlayer()->getSquat()) {
            oMap->getPlayer()->startMove();
            oMap->getPlayer()->setMoveDirection(true);
        } else if(!keyAPressed && oMap->getPlayer()->getMoveSpeed() > 0 && firstDir != oMap->getPlayer()->getMoveDirection()) {
            oMap->getPlayer()->setChangeMoveDirection();
        }
    }

    if(oMap->getPlayer()->getMove() && !keyAPressed && !keyDPressed) {
        oMap->getPlayer()->resetMove();
    }
}

void GDCore::MouseInput() {
    switch(mainEvent->type) {
        case SDL_MOUSEBUTTONDOWN: {
            switch (mainEvent->button.button) {
                case SDL_BUTTON_LEFT:
                    mouseLeftPressed = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseRightPressed = true;
                    break;
            }
            break;
        }
        case SDL_MOUSEMOTION: {

            SDL_GetMouseState(&mouseX, &mouseY);
            //CCFG::getMM()->getConsole()->print("x:" + std::to_string(mouseX));
            //CCFG::getMM()->getConsole()->print("y:" + std::to_string(mouseY));
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            switch (mainEvent->button.button) {
                case SDL_BUTTON_LEFT:
                    mouseLeftPressed = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    mouseRightPressed = false;
                    break;
            }
            break;
        }
    }
}

//==================================================================================================

void GDCore::TouchInput() {
    switch(mainEvent->type) {
        case SDL_FINGERDOWN: {
            SDL_FingerID fingerId = mainEvent->tfinger.fingerId;

            float touchX = mainEvent->tfinger.x * (float)rendererOutputWindowWidth;
            float touchY = mainEvent->tfinger.y * (float)rendererOutputWindowHeight;

            ImVec2 position = GetTouchCoordinates(touchX, touchY);

            TouchPoint touch = {
                    fingerId,
                    position,
                    true
            };
            touchPoints[fingerId] = touch;

            ImGuiIO &io = ImGui::GetIO();
            io.MousePos = position;
            io.MouseDown[0] = true;

            break;
        }

        case SDL_FINGERUP: {
            SDL_FingerID fingerId = mainEvent->tfinger.fingerId;

            if (touchPoints.count(fingerId) > 0)
                touchPoints.erase(fingerId);

            ImGuiIO& io = ImGui::GetIO();
            io.MouseDown[0] = false;

            break;
        }

        case SDL_FINGERMOTION: {
            SDL_FingerID fingerId = mainEvent->tfinger.fingerId;

            if (touchPoints.count(fingerId) > 0) {
                float touchX = mainEvent->tfinger.x * (float)rendererOutputWindowWidth;
                float touchY = mainEvent->tfinger.y * (float)rendererOutputWindowHeight;

                ImVec2 position = GetTouchCoordinates(touchX, touchY);

                touchPoints[fingerId].pos = position;
            }

            break;
        }
    }
}

ImVec2 GDCore::GetTouchCoordinates(float touchX, float touchY) {
    scaledX = (touchX / (float)rendererOutputWindowWidth) * (float)logicalWidth + offsetX;
    scaledY = (touchY / (float)rendererOutputWindowHeight) * (float)logicalHeight - offsetY;

    return ImVec2(scaledX, scaledY);
}

bool GDCore::IsPointInRect(const ImVec2& point, const ImVec2& rectMin, const ImVec2& rectMax) {
    return point.x >= rectMin.x && point.x <= rectMax.x && point.y >= rectMin.y && point.y <= rectMax.y;
}

bool GDCore::IsTouchingButton(const ImVec2& buttonPos, const ImVec2& buttonSize) {
    ImVec2 buttonMax(buttonPos.x + buttonSize.x, buttonPos.y + buttonSize.y);
    for (const auto& [fingerId, touch] : touchPoints) {
        if (touch.active && IsPointInRect(touch.pos, buttonPos, buttonMax)) {
            return true;
        }
    }
    return false;
}

void GDCore::InitializeTouchVariables() {
    SDL_GetRendererOutputSize(rR, &rendererOutputWindowWidth, &rendererOutputWindowHeight);

    logicalWidth = CCFG::GAME_WIDTH;
    logicalHeight = CCFG::GAME_HEIGHT;

    offsetX = 75.0f;
    offsetY = 10.0f;
}

void GDCore::InitializeImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // ImGui style
    ImGui::StyleColorsDark();

    // Load font from assets folder
    ImGuiIO& io = ImGui::GetIO();

    #ifdef __ANDROID__
        SDL_RWops* fontFile = SDL_RWFromFile("files/fonts/MarklMono.otf", "rb");
        if (fontFile) {
            size_t fileSize = SDL_RWsize(fontFile);
            void* fontData = IM_ALLOC(fileSize);
            SDL_RWread(fontFile, fontData, fileSize, 1);
            SDL_RWclose(fontFile);

            ImFontConfig fontConfig;
            fontConfig.MergeMode = false;

            static const ImWchar unicodeRanges[] = {
                    0x0020, 0x00FF,
                    0x25A0, 0x25FF,
                    0
            };
            
            io.Fonts->AddFontFromMemoryTTF(fontData, (int)fileSize, 30.0f, &fontConfig, unicodeRanges);
        }
        else
            SDL_Log("Failed to load font from assets!");
    #endif

    // Initialize SDL2 bindings
    ImGui_ImplSDL2_InitForSDLRenderer(window, rR);
    ImGui_ImplSDLRenderer2_Init(rR);
}

void GDCore::RenderGameUI() {
    // Start ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    // Border Variables
    float borderSize = 3.0f;
    ImVec4 borderColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Label Variables
    float ABLabelSize = 2.5f;
    ImVec4 blackTextColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    // --- Menu ---
    if (CCFG::getMM()->getViewID() == MenuManager::eGame) {
        ImVec2 menuWindowPos(75, 1.5);
        ImGui::SetNextWindowPos(menuWindowPos);
        ImGui::SetNextWindowSize(ImVec2(75, 75));
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                      ImGuiWindowFlags_NoBackground);

        ImVec2 MenuSize(50, 50);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);
        ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 0.625f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 0.75f));

        ImGui::SetCursorPos(ImVec2(10, 10));
        if (ImGui::Button("■", MenuSize)) {
            CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->ePause);
            CCFG::getMM()->setViewID(CCFG::getMM()->ePause);
            CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPASUE);
            CCFG::getMusic()->PauseMusic();
        }

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        ImGui::End();
    }

    // --- D-Pad ---
    ImVec2 DPADWindowPos(75, 185);
    ImGui::SetNextWindowPos(ImVec2(75, 185));
    ImGui::SetNextWindowSize(ImVec2(250, 245));
    ImGui::Begin("D-Pad", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    ImVec2 DPadSize(75, 75);

    float centerX = ImGui::GetWindowSize().x / 2;
    float buttonOffset = 10.0f;
    float LRGap = 30.0f;

    // Colors for D-Pad
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.6f, 0.6f, 0.625f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 0.75f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);

    // Button positions relative to window
    ImVec2 upButtonPos(centerX - DPadSize.x / 2, 0);  // Up
    ImVec2 leftButtonPos(centerX - DPadSize.x - buttonOffset - LRGap, DPadSize.y + buttonOffset);  // Left
    ImVec2 rightButtonPos(centerX + buttonOffset + LRGap, DPadSize.y + buttonOffset);  // Right
    ImVec2 downButtonPos(centerX - DPadSize.x / 2, DPadSize.y * 2 + buttonOffset);  // Down

    // Absolute coordinates (window position + relative position)
    ImVec2 upButtonAbsolute(DPADWindowPos.x + upButtonPos.x, DPADWindowPos.y + upButtonPos.y + 10.0f);
    ImVec2 leftButtonAbsolute(DPADWindowPos.x + leftButtonPos.x - 20.0f, DPADWindowPos.y + leftButtonPos.y);
    ImVec2 rightButtonAbsolute(DPADWindowPos.x + rightButtonPos.x - 20.0f, DPADWindowPos.y + rightButtonPos.y);
    ImVec2 downButtonAbsolute(DPADWindowPos.x + downButtonPos.x, DPADWindowPos.y + downButtonPos.y);

    // Up
    ImGui::SetCursorPosX(centerX - DPadSize.x / 2);
    if (ImGui::Button("▲", DPadSize)) {
        if (CCFG::getMM()->getViewID() != MenuManager::eGame) { CCFG::getMM()->keyPressed(0); }
    }
    if (IsTouchingButton(upButtonAbsolute, DPadSize)) {
        if (CCFG::getMM()->getViewID() == MenuManager::eGame){
            oMap->getPlayer()->jump();
            CCFG::keySpace = true;
        }
    }
    else {
        CCFG::keySpace = false;
    }

    // Left & Right
    ImGui::SetCursorPos(ImVec2(centerX - DPadSize.x - buttonOffset - LRGap, DPadSize.y + buttonOffset));
    if (ImGui::Button("◀", DPadSize)) {
        if (CCFG::getMM()->getViewID() != MenuManager::eGame) { CCFG::getMM()->keyPressed(3); }
    }
    if (IsTouchingButton(leftButtonAbsolute, DPadSize)) {
        if (CCFG::getMM()->getViewID() == MenuManager::eGame){
            keyAPressed = true;
            if(!keyDPressed)
                firstDir = false;
        }
    }
    else {
        if(!firstDir)
            firstDir = true;
        keyAPressed = false;
    }
    
    ImGui::SameLine();

    ImGui::SetCursorPosX(centerX + buttonOffset + LRGap);
    if (ImGui::Button("▶", DPadSize)) {
        if (CCFG::getMM()->getViewID() != MenuManager::eGame) { CCFG::getMM()->keyPressed(1); }
    }
    if (IsTouchingButton(rightButtonAbsolute, DPadSize)) {
        if (CCFG::getMM()->getViewID() == MenuManager::eGame){
            keyDPressed = true;
            if(!keyAPressed)
                firstDir = true;
        }
    }
    else {
        if(firstDir)
            firstDir = false;
        keyDPressed = false;
    }

    // Down
    ImGui::SetCursorPos(ImVec2(centerX - DPadSize.x / 2, DPadSize.y * 2 + buttonOffset));
    if (ImGui::Button("▼", DPadSize)) {
        if (CCFG::getMM()->getViewID() != MenuManager::eGame) { CCFG::getMM()->keyPressed(2); }
    }
    if (IsTouchingButton(downButtonAbsolute, DPadSize)) {
        if (CCFG::getMM()->getViewID() == MenuManager::eGame && !keyS){
            keyS = true;
            if(!oMap->getUnderWater() && !oMap->getPlayer()->getInLevelAnimation())
                oMap->getPlayer()->setSquat(true);
        }
    }
    else {
        oMap->getPlayer()->setSquat(false);
        keyS = false;
    }

    // Pop border size & color
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::PopStyleColor(3);

    ImGui::End();

    // --- A/B Buttons
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 50.0f);

    ImVec2 ABWindowPos(685, 260);
    ImGui::SetNextWindowPos(ImVec2(685, 260));
    ImGui::SetNextWindowSize(ImVec2(200, 100));
    ImGui::Begin("Buttons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    ImVec2 ABSize(75, 75);
    float windowCenterX = ImGui::GetWindowSize().x / 2;
    float ABgap = 20.0f;
    float totalButtonWidth = 2 * ABSize.x + ImGui::GetStyle().ItemSpacing.x + ABgap / 2;
    float offset = windowCenterX - totalButtonWidth / 2;

    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
    ImGui::PushStyleColor(ImGuiCol_Text, blackTextColor);

    // colors for A Button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.1f, 0.1f, 0.625f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.7f, 0.0f, 0.0f, 0.75f));

    ImVec2 aButtonPos(offset, 0);
    ImVec2 aButtonAbsolute(ABWindowPos.x + aButtonPos.x, ABWindowPos.y + aButtonPos.y + 10.0f);


    ImGui::SetCursorPosX(offset);
    if (ImGui::Button("A", ABSize)) {
        if (CCFG::getMM()->getViewID() != MenuManager::eGame) { CCFG::getMM()->enter(); }
    }
    if (IsTouchingButton(aButtonAbsolute, DPadSize)) {
        if (CCFG::getMM()->getViewID() == MenuManager::eGame){
            oMap->getPlayer()->jump();
            CCFG::keySpace = true;
        }
    }
   else {
        CCFG::keySpace = false;
    }

    ImGui::PopStyleColor(3);

    // colors for B Button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.8f, 0.0f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.9f, 0.1f, 0.625f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.7f, 0.0f, 0.75f));

    ImVec2 bButtonPos(offset + ABSize.x + ABgap, 0);
    ImVec2 bButtonAbsolute(ABWindowPos.x + bButtonPos.x, ABWindowPos.y + bButtonPos.y + 10.0f);

    ImGui::SameLine(0.0f, ABgap);
    if (ImGui::Button("B", ABSize)) {
        if (CCFG::getMM()->getViewID() != MenuManager::eGame) { CCFG::getMM()->escape(); }
    }
    if (IsTouchingButton(bButtonAbsolute, DPadSize)) {
        if (CCFG::getMM()->getViewID() == MenuManager::eGame){
            oMap->getPlayer()->startRun();
            keyShift = true;
        }
    }
    else {
        oMap->getPlayer()->resetRun();
        keyShift = false;
    }
    ImGui::PopStyleColor(4);

    // Pop border size & color
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    // Text Rounding
    ImGui::PopStyleVar();

    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), rR);
}

//==================================================================================================

void GDCore::resetKeys() {
    movePressed = keyMenuPressed = keyS = keyW = keyA = keyD = CCFG::keySpace = keyShift = keyAPressed = keyDPressed = false;
}

void GDCore::Update() {
    CCFG::getMM()->Update();
}


void GDCore::Draw() {
    CCFG::getMM()->Draw(rR);

    // Render ImGui
    RenderGameUI();
}

/* ******************************************** */

void GDCore::resetMove() {
    this->keyAPressed = this->keyDPressed = false;
}

Map* GDCore::getMap() {
    return oMap;
}
