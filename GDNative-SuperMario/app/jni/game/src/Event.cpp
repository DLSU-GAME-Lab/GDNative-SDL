#include "Event.h"
#include "GDCore.h"

/* ******************************************** */

Event::Event(void) {
	this->iDelay = 0;
	this->newUnderWater = false;
	this->endGame = false;
	this->iTime = 0;
	this->bState = true;
	this->stepID = 0;
}

Event::~Event(void) {

}

/* ******************************************** */

void Event::Draw(SDL_Renderer* rR) {
	for(unsigned int i = 0; i < reDrawX.size(); i++) {
		if(reDrawX[i] < GDCore::getMap()->getMapWidth())
			GDCore::getMap()->getBlock(GDCore::getMap()->getMapBlock(reDrawX[i], reDrawY[i])->getBlockID())->Draw(rR, 32 * reDrawX[i] + (int)GDCore::getMap()->getXPos(), CCFG::GAME_HEIGHT - 32 * reDrawY[i] - 16);
	}
}

/* ******************************************** */

void Event::Animation() {
	switch(eventTypeID) {
		case eNormal: {
			Normal();
			break;
		}
		case eEnd: {
			Normal();
			end();
			break;
		}
		case eBossEnd: {
			Normal();
			break;
		}
		default:
			Normal();
			break;
	}
}

void Event::Normal() {
	if(bState) {
		if(vOLDDir.size() > stepID) {
			if(vOLDLength[stepID] > 0) {
				switch(vOLDDir[stepID]) {
					case eTOP: // TOP
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						break;
					case eBOT:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						break;
					case eRIGHT:
						GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->moveAnimation();
						GDCore::getMap()->getPlayer()->setMoveDirection(true);
						break;
					case eRIGHTEND:
						GDCore::getMap()->setXPos((float)GDCore::getMap()->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->moveAnimation();
						GDCore::getMap()->getPlayer()->setMoveDirection(true);
						break;
					case eLEFT:
						GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->moveAnimation();
						GDCore::getMap()->getPlayer()->setMoveDirection(false);
						break;
					case eBOTRIGHTEND: // BOT & RIGHT
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() + iSpeed);
						GDCore::getMap()->setXPos((float)GDCore::getMap()->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->moveAnimation();
						break;
					case eENDBOT1:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(10);
						break;
					case eENDBOT2:
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMoveDirection(false);
						break;
					case eENDPOINTS:
						if(GDCore::getMap()->getMapTime() > 0) {
							GDCore::getMap()->setMapTime(GDCore::getMap()->getMapTime() - 1);
							GDCore::getMap()->getPlayer()->setScore(GDCore::getMap()->getPlayer()->getScore() + 50);
							if(CCFG::getMusic()->musicStopped) {
								CCFG::getMusic()->PlayMusic(CCFG::getMusic()->mSCORERING);
							}
						} else {
							vOLDLength[stepID] = 0;
							CCFG::getMusic()->StopMusic();
						}
						GDCore::getMap()->getFlag()->UpdateCastleFlag();
						break;
					case eDEATHNOTHING:
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(0);
						break;
					case eDEATHTOP: // DEATH TOP
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(0);
						break;
					case eDEATHBOT: // DEATH BOT
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() + iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(0);
						break;
					case eNOTHING: // NOTHING YAY
						vOLDLength[stepID] -= 1;
						break;
					case ePLAYPIPERIGHT:
						GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() + iSpeed);
						vOLDLength[stepID] -= 1;
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPIPE);
						break;
					case eLOADINGMENU:
						vOLDLength[stepID] -= 1;

						if(vOLDLength[stepID] < 2) {
							GDCore::getMap()->setInEvent(false);
							inEvent = false;
							GDCore::getMap()->getPlayer()->stopMove();
							
							CCFG::getMM()->getLoadingMenu()->loadingType = true;
							CCFG::getMM()->getLoadingMenu()->updateTime();
							CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
						}
						break;
					case eGAMEOVER:
						vOLDLength[stepID] -= 1;

						if(vOLDLength[stepID] < 2) {
							GDCore::getMap()->setInEvent(false);
							inEvent = false;
							GDCore::getMap()->getPlayer()->stopMove();
							
							CCFG::getMM()->getLoadingMenu()->loadingType = false;
							CCFG::getMM()->getLoadingMenu()->updateTime();
							CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);

							CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cGAMEOVER);
						}
						break;
					case eBOSSEND1:
						for(int i = GDCore::getMap()->getMapWidth() - 1; i > 0; i--) {
							if(GDCore::getMap()->getMapBlock(i, 6)->getBlockID() == 82) {
								GDCore::getMap()->getMapBlock(i, 6)->setBlockID(0);
								break;
							}
						}
						GDCore::getMap()->getMapBlock(GDCore::getMap()->getBlockIDX((int)(GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 - GDCore::getMap()->getXPos()) + vOLDLength[stepID] - 1), 6)->setBlockID(0);
						GDCore::getMap()->clearPlatforms();
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBRIDGEBREAK);
						vOLDLength[stepID] = 0;
						GDCore::getMap()->getPlayer()->setMoveDirection(false);
						break;
					case eBOSSEND2:
						//GDCore::getMap()->getMapBlock(GDCore::getMap()->getBlockIDX((int)(GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 - GDCore::getMap()->getXPos())) - 1, 5)->setBlockID(0);
						//GDCore::getMap()->getMapBlock(GDCore::getMap()->getBlockIDX((int)(GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 - GDCore::getMap()->getXPos())) - 1, 4)->setBlockID(0);
						for(int i = GDCore::getMap()->getMapWidth() - 1; i > 0; i--) {
							if(GDCore::getMap()->getMapBlock(i, 5)->getBlockID() == 79) {
								GDCore::getMap()->getMapBlock(i, 5)->setBlockID(0);
								break;
							}
						}
						for(int i = GDCore::getMap()->getMapWidth() - 1; i > 0; i--) {
							if(GDCore::getMap()->getMapBlock(i, 4)->getBlockID() == 76) {
								GDCore::getMap()->getMapBlock(i, 4)->setBlockID(0);
								break;
							}
						}
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBRIDGEBREAK);
						vOLDLength[stepID] = 0;
						break;
					case eBOSSEND3:
						for(int i = GDCore::getMap()->getMapWidth() - 1; i > 0; i--) {
							if(GDCore::getMap()->getMapBlock(i, 4)->getBlockID() == 76) {
								GDCore::getMap()->getMapBlock(i, 4)->setBlockID(0);
								break;
							}
						}
						GDCore::getMap()->getMapBlock(GDCore::getMap()->getBlockIDX((int)(GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 - GDCore::getMap()->getXPos())) - vOLDLength[stepID], 4)->setBlockID(0);
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBRIDGEBREAK);
						GDCore::getMap()->getPlayer()->setMoveDirection(true);
						vOLDLength[stepID] = 0;
						break;
					case eBOSSEND4:
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBOWSERFALL);
						vOLDLength[stepID] = 0;
						break;
					case eBOTRIGHTBOSS: // BOT & RIGHT
						GDCore::getMap()->getPlayer()->moveAnimation();
						GDCore::getMap()->getPlayer()->playerPhysics();
						GDCore::getMap()->setXPos((float)GDCore::getMap()->getXPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						break;
					case eBOSSTEXT1:
						GDCore::getMap()->addText(vOLDLength[stepID], CCFG::GAME_HEIGHT - 16 - 9*32, "THANK YOU MARIOz");
						vOLDLength[stepID] = 0;
						break;
					case eBOSSTEXT2:
						GDCore::getMap()->addText(vOLDLength[stepID] + 16, CCFG::GAME_HEIGHT - 16 - 7*32, "BUT OUR PRINCESS IS IN");
						GDCore::getMap()->addText(vOLDLength[stepID] + 16, CCFG::GAME_HEIGHT - 16 - 6*32, "ANOTHER CASTLEz");
						vOLDLength[stepID] = 0;
						break;
					case eENDGAMEBOSSTEXT1:
						GDCore::getMap()->addText(vOLDLength[stepID], CCFG::GAME_HEIGHT - 16 - 9*32, "THANK YOU MARIOz");
						vOLDLength[stepID] = 0;
						break;
					case eENDGAMEBOSSTEXT2:
						GDCore::getMap()->addText(vOLDLength[stepID] + 16, CCFG::GAME_HEIGHT - 16 - 7*32, "YOUR QUEST IS OVER.");
						vOLDLength[stepID] = 0;
						break;
					case eMARIOSPRITE1:
						GDCore::getMap()->getPlayer()->setMarioSpriteID(1);
						vOLDLength[stepID] = 0;
						break;
					case eVINE1:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(10);
						break;
					case eVINE2:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vOLDLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(11);
						break;
					case eVINESPAWN:
						GDCore::getMap()->addVine(vOLDLength[stepID], 0, 1, newLevelType == 0 || newLevelType == 4 ? 34 : 36);
						vOLDLength[stepID] = 0;
						break;
				}
			} else {
				++stepID;
				iTime = SDL_GetTicks();
			}
		} else {
			if(!endGame) {
				if(SDL_GetTicks() >= iTime + iDelay) {
					bState = false;
					stepID = 0;
					newLevel();
					GDCore::getMap()->getPlayer()->stopMove();
					if(inEvent) {
						CCFG::getMM()->getLoadingMenu()->updateTime();
						CCFG::getMM()->getLoadingMenu()->loadingType = true;
						CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
						GDCore::getMap()->startLevelAnimation();
					}

					CCFG::keySpace = false;
				}
			} else {
				GDCore::getMap()->resetGameData();
				CCFG::getMM()->setViewID(CCFG::getMM()->eMainMenu);
				GDCore::getMap()->setInEvent(false);
				GDCore::getMap()->getPlayer()->stopMove();
				inEvent = false;
				CCFG::keySpace = false;
				endGame = false;
				stepID = 0;
			}
		}
	} else {
		if(vNEWDir.size() > stepID) {
			if(vNEWLength[stepID] > 0) {
				switch(vNEWDir[stepID]) {
					case eTOP: // TOP
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						break;
					case eBOT:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() + iSpeed);
						vNEWLength[stepID] -= iSpeed;
						break;
					case eRIGHT:
						GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() + iSpeed);
						vNEWLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->moveAnimation();
						break;
					case eLEFT:
						GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->moveAnimation();
						break;
					case ePLAYPIPETOP:
						vNEWLength[stepID] -= 1;
						CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPIPE);
						break;
					case eNOTHING: // NOTHING YAY
						vNEWLength[stepID] -= 1;
						break;
					case eVINE1:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(10);
						break;
					case eVINE2:
						GDCore::getMap()->getPlayer()->setYPos((float)GDCore::getMap()->getPlayer()->getYPos() - iSpeed);
						vNEWLength[stepID] -= iSpeed;
						GDCore::getMap()->getPlayer()->setMarioSpriteID(11);
						break;
				}
			} else {
				++stepID;
			}
		} else {
			GDCore::getMap()->setInEvent(false);
			GDCore::getMap()->getPlayer()->stopMove();
//			CCFG::getMusic()->changeMusic(true, true);
			inEvent = false;
			CCFG::keySpace = false;
			GDCore::resetKeys();
		}
	}
}

void Event::end() {
	if(GDCore::getMap()->getFlag() != NULL && GDCore::getMap()->getFlag()->iYPos < CCFG::GAME_HEIGHT - 16 - 3*32 - 4) {
		GDCore::getMap()->getFlag()->Update();
	}
}

void Event::newLevel() {
	GDCore::getMap()->setXPos((float)newMapXPos);
	GDCore::getMap()->getPlayer()->setXPos((float)newPlayerXPos);
	GDCore::getMap()->getPlayer()->setYPos((float)newPlayerYPos);
	GDCore::getMap()->setMoveMap(newMoveMap);
	if(GDCore::getMap()->getCurrentLevelID() != newCurrentLevel) {
		CCFG::getMM()->getLoadingMenu()->updateTime();
		CCFG::getMM()->getLoadingMenu()->loadingType = true;
		CCFG::getMM()->setViewID(CCFG::getMM()->eGameLoading);
		GDCore::getMap()->getPlayer()->setCoins(0);
	}
	GDCore::getMap()->setCurrentLevelID(newCurrentLevel);
	GDCore::getMap()->setLevelType(newLevelType);
	if(newUnderWater) {
		GDCore::getMap()->getPlayer()->resetRun();
	}
	GDCore::getMap()->setUnderWater(newUnderWater);

	GDCore::getMap()->lockMinions();
}

/* ******************************************** */

void Event::resetData() {
	vNEWDir.clear();
	vNEWLength.clear();
	vOLDDir.clear();
	vOLDLength.clear();
	resetRedraw();

	this->eventTypeID = eNormal;

	this->bState = true;
	this->stepID = 0;
	this->inEvent = false;
	this->endGame = false;
	this->newUnderWater = false;
}

void Event::resetRedraw() {
	reDrawX.clear();
	reDrawY.clear();
}