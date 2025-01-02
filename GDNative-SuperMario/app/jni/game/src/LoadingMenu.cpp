#include "LoadingMenu.h"
#include "CFG.h"
#include "GDCore.h"

/* ******************************************** */

LoadingMenu::LoadingMenu(void) {
	this->iTime = 0;
	this->loadingType = true;
}

LoadingMenu::~LoadingMenu(void) {

}

/* ******************************************** */

void LoadingMenu::Update() {
	if(SDL_GetTicks() >= iTime + 2500 + (loadingType ? 0 : 2750)) {
		if(loadingType) {
			if(!GDCore::getMap()->getInEvent()) {
				GDCore::getMap()->setSpawnPoint();
				GDCore::getMap()->loadLVL();
			}
			CCFG::getMM()->setViewID(CCFG::getMM()->eGame);
			CCFG::getMusic()->changeMusic(true, true);
		} else {
			GDCore::getMap()->resetGameData();
			CCFG::getMM()->setViewID(CCFG::getMM()->eMainMenu);
		}
	} else {
		CCFG::getMusic()->StopMusic();
	}
	GDCore::getMap()->UpdateBlocks();
}

void LoadingMenu::Draw(SDL_Renderer* rR) {
	if(loadingType) {
		GDCore::getMap()->DrawGameLayout(rR);
		CCFG::getText()->Draw(rR, "WORLD", 320, 144);
		CCFG::getText()->Draw(rR, GDCore::getMap()->getLevelName(), 416, 144);

		GDCore::getMap()->getPlayer()->getMarioSprite()->getTexture()->Draw(rR, 342, 210 - GDCore::getMap()->getPlayer()->getHitBoxY()/2);

		CCFG::getText()->Draw(rR, "y", 384, 208);

		if(GDCore::getMap()->getPlayer()->getNumOfLives() > 9) {
			GDCore::getMap()->getBlock(180)->getSprite()->getTexture()->Draw(rR, 410, 210);
		}
		CCFG::getText()->Draw(rR, std::to_string(GDCore::getMap()->getPlayer()->getNumOfLives()), 432, 208);
		CCFG::getText()->DrawCenterX(rR, "REMEMBER THAT YOU CAN RUN WITH " + CCFG::getKeyString(CCFG::keyIDShift), 400, 16);
	} else {
		GDCore::getMap()->DrawGameLayout(rR);
		CCFG::getText()->DrawCenterX(rR, "GAME OVER", 240, 16);
	}
}

/* ******************************************** */

void LoadingMenu::updateTime() {
	this->iTime = SDL_GetTicks();
}