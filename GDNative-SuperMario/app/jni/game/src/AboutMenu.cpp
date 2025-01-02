#include "AboutMenu.h"
#include "CFG.h"
#include "GDCore.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

AboutMenu::AboutMenu(void) {
	lMO.push_back(new MenuOption("MAIN MENU", 150, 340));

	this->numOfMenuOptions = lMO.size();

	this->cR = 93;
	this->cG = 148;
	this->cB = 252;
	this->nR = 0;
	this->nG = 0;
	this->nB = 0;
	this->colorStepID = 1;
	this->iColorID = 0;

	this->moveDirection = true;

	this->iNumOfUnits = 0;

	srand((unsigned)time(NULL));
}


AboutMenu::~AboutMenu(void) {

}

/* ******************************************** */

void AboutMenu::Update() {
	if(SDL_GetTicks() >= iTime + 35) {
		this->cR = getColorStep(cR, nR);
		this->cG = getColorStep(cG, nG);
		this->cB = getColorStep(cB, nB);

		if (colorStepID >= 15 || (cR == nR && cG == nG && cB == nB)) {
			nextColor();
			colorStepID = 1;
		} else {
			++colorStepID;
		}

		GDCore::getMap()->setLevelType(rand()%4);
		
		if(rand()%10 < 6) {
			GDCore::getMap()->addGoombas(-(int)GDCore::getMap()->getXPos() + rand() % (CCFG::GAME_WIDTH + 128), -32, rand()%2 == 0);
			GDCore::getMap()->addGoombas(-(int)GDCore::getMap()->getXPos() + rand() % (CCFG::GAME_WIDTH + 128), -32, rand()%2 == 0);
		} else if(rand()%10 < 8) {
			GDCore::getMap()->addKoppa(-(int)GDCore::getMap()->getXPos() + rand() % (CCFG::GAME_WIDTH + 128), -32, 0, rand()%2 == 0);
			GDCore::getMap()->addKoppa(-(int)GDCore::getMap()->getXPos() + rand() % (CCFG::GAME_WIDTH + 128), -32, 0, rand()%2 == 0);
		} else if(rand()%6 < 4) {
			GDCore::getMap()->addFire(-GDCore::getMap()->getXPos() + CCFG::GAME_WIDTH + 128, CCFG::GAME_HEIGHT - 16.0f - rand()%16*32, CCFG::GAME_HEIGHT - 16 - rand()%16*32);
		} else if(rand()%6 < 4) {
			GDCore::getMap()->addBulletBill((int)(-GDCore::getMap()->getXPos() + CCFG::GAME_WIDTH + 128), CCFG::GAME_HEIGHT - 16 - rand()%16*32, true, 1);
		} else {
			GDCore::getMap()->addFireBall(-(int)GDCore::getMap()->getXPos() + rand() % (CCFG::GAME_WIDTH + 128) + 8, CCFG::GAME_HEIGHT - 16 - rand()%16 * 32, rand()%8 + 4 + 8, rand()%360, rand()%2 == 0);
		}

		//iNumOfUnits += 2;

		iTime = SDL_GetTicks();
	}
	
	if(moveDirection && CCFG::GAME_WIDTH - GDCore::getMap()->getXPos() >= (GDCore::getMap()->getMapWidth() - 20) * 32) {
		moveDirection = !moveDirection;
	} else if(!moveDirection && -GDCore::getMap()->getXPos() <= 0) {
		moveDirection = !moveDirection;
	}

	GDCore::getMap()->setXPos(GDCore::getMap()->getXPos() + 4 * (moveDirection ? -1 : 1));
	//GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() + 4 * (moveDirection ? -1 : 1));
}

void AboutMenu::Draw(SDL_Renderer* rR) {
	CCFG::getText()->DrawWS(rR, "MARIO V 1.04 - C++ AND SDL2", 50, 128, 0, 0, 0);
	CCFG::getText()->DrawWS(rR, "PRIMARY AUTHOR: LUKASZ JAKOWSKI 2014", 50, 146, 0, 0, 0);
	CCFG::getText()->DrawWS(rR, "EXTENDED BY: NEIL PATRICK DEL GALLEGO, PH.D.", 50, 165, 0, 0, 0);
	CCFG::getText()->DrawWS(rR, "AND DLSU GAME LAB 2025", 50, 190, 0, 0, 0);

	CCFG::getText()->DrawWS(rR, "WWW.LUKASZJAKOWSKI.PL", 50, 264, 0, 0, 0);
	CCFG::getText()->DrawWS(rR, "WWW.FACEBOOK.COM/DLSUGAMELAB", 50, 315, 0, 0, 0);

	//CCFG::getText()->DrawWS(rR, std::to_string(iNumOfUnits), 5, CCFG::GAME_HEIGHT - 21, 0, 0, 0);

	for(unsigned int i = 0; i < lMO.size(); i++) {
		CCFG::getText()->DrawWS(rR, lMO[i]->getText(), lMO[i]->getXPos(), lMO[i]->getYPos(), 0, 0, 0);
	}

	CCFG::getMM()->getActiveOption()->Draw(rR, lMO[activeMenuOption]->getXPos() - 32, lMO[activeMenuOption]->getYPos());
}

/* ******************************************** */

void AboutMenu::enter() {
	CCFG::getMM()->resetActiveOptionID(CCFG::getMM()->eMainMenu);
	CCFG::getMM()->setViewID(CCFG::getMM()->eMainMenu);
	reset();
	CCFG::getMusic()->StopMusic();
}

/* ******************************************** */

void AboutMenu::launch() {
	this->cR = 93;
	this->cG = 148;
	this->cB = 252;
}

void AboutMenu::reset() {
	GDCore::getMap()->setXPos(0);
	GDCore::getMap()->loadLVL();
}

/* ******************************************** */

void AboutMenu::nextColor() {
	int iN = iColorID;

	while(iN == iColorID) {
		iColorID = rand() % 16;
	}

	++iColorID;

	switch (iColorID) {
		case 0:
			nR = 73;
			nG = 133;
			nB = 203;
			break;
		case 1:
			nR = 197;
			nG = 197;
			nB = 223;
			break;
		case 2:
			nR = 27;
			nG = 60;
			nB = 173;
			break;
		case 3:
			nR = 6;
			nG = 21;
			nB = 86;
			break;
		case 4:
			nR = 183;
			nG = 85;
			nB = 76;
			break;
		case 5:
			nR = 110;
			nG = 58;
			nB = 70;
			break;
		case 6:
			nR = 55;
			nG = 19;
			nB = 63;
			break;
		case 7:
			nR = 115;
			nG = 53;
			nB = 126;
			break;
		case 8:
			nR = 227;
			nG = 200;
			nB = 0;
			break;
		case 9:
			nR = 255;
			nG = 180;
			nB = 2;
			break;
		case 10:
			nR = 231;
			nG = 51;
			nB = 24;
			break;
		case 11:
			nR = 255;
			nG = 180;
			nB = 2;
			break;
		case 12:
			nR = 4;
			nG = 2;
			nB = 15;
			break;
		case 13:
			nR = 135;
			nG = 178;
			nB = 168;
			break;
		case 14:
			nR = 64;
			nG = 43;
			nB = 24;
			break;
		case 15:
			nR = rand() % 255;
			nG = rand() % 255;
			nB = rand() % 255;
			break;
	}
}

int AboutMenu::getColorStep(int iOld, int iNew) {
	return iOld + (iOld > iNew ? (iNew - iOld) * colorStepID / 30 : (iNew - iOld) * colorStepID / 30);
}

/* ******************************************** */

void AboutMenu::setBackgroundColor(SDL_Renderer* rR) {
	SDL_SetRenderDrawColor(rR, cR, cG, cB, 255);
}

void AboutMenu::updateTime() {
	this->iTime = SDL_GetTicks();
}