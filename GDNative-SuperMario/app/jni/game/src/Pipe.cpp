#include "Pipe.h"
#include "GDCore.h"

/* ******************************************** */

Pipe::Pipe(int iType, int iLX, int iLY, int iRX, int iRY, int newPlayerPosX, int newPlayerPosY, int newCurrentLevel, int newLevelType, bool newMoveMap, int iDelay, int iSpeed, bool newUnderWater) {
	this->iType = iType;
	
	this->iLX = iLX;
	this->iLY = iLY;
	this->iRX = iRX;
	this->iRY = iRY;

	this->newPlayerPosX = newPlayerPosX;
	this->newPlayerPosY = newPlayerPosY;

	this->newCurrentLevel = newCurrentLevel;
	this->newLevelType = newLevelType;
	this->newMoveMap = newMoveMap;
	this->newUnderWater = newUnderWater;

	this->iDelay = iDelay;

	this->iSpeed = iSpeed;
}


Pipe::~Pipe(void) {

}

/* ******************************************** */

void Pipe::checkUse() {
	if(iType == 0 || iType == 2) {
		if(GDCore::getMap()->getPlayer()->getSquat() && -(int)GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getXPos() >= iLX * 32 + 4 && -(int)GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX() < (iRX + 1) * 32 - 4) {
			setEvent();
		}
	} else {
		if(!GDCore::getMap()->getPlayer()->getSquat() && GDCore::getMap()->getBlockIDX(-(int)GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX() / 2 + 2) == iRX - 1 && GDCore::getMap()->getBlockIDY(GDCore::getMap()->getPlayer()->getYPos() + GDCore::getMap()->getPlayer()->getHitBoxY() + 2) == iRY - 1) {
			setEvent();
		}
	}
}

/* ******************************************** */

void Pipe::setEvent() {
	GDCore::getMap()->getEvent()->resetData();
	GDCore::getMap()->getPlayer()->stopMove();
	GDCore::getMap()->getPlayer()->resetJump();

	CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cPIPE);

	GDCore::getMap()->getEvent()->eventTypeID = GDCore::getMap()->getEvent()->eNormal;

	GDCore::getMap()->getEvent()->newCurrentLevel = newCurrentLevel;
	GDCore::getMap()->getEvent()->newLevelType = newLevelType;
	GDCore::getMap()->getEvent()->newMoveMap = newMoveMap;

	GDCore::getMap()->getEvent()->iSpeed = iSpeed;
	GDCore::getMap()->getEvent()->iDelay = iDelay;

	GDCore::getMap()->getEvent()->inEvent = false;

	GDCore::getMap()->getEvent()->newUnderWater = newUnderWater;

	GDCore::getMap()->getEvent()->newMapXPos = (newPlayerPosX <= 32 * 2 ? 0 : -(newPlayerPosX - 32 * 2));
	GDCore::getMap()->getEvent()->newPlayerXPos = (newPlayerPosX <= 32 * 2 ? newPlayerPosX : 32 * 2);
	GDCore::getMap()->getEvent()->newPlayerYPos = newPlayerPosY;

	if(iType == 0) { // VERTICAL -> NONE
		GDCore::getMap()->getEvent()->newPlayerYPos -= GDCore::getMap()->getPlayer()->getPowerLVL() > 0 ? 32 : 0;
		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eBOT);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxY());

		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eNOTHING);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(35);

		for(int i = 0; i < 3; i++) {
			GDCore::getMap()->getEvent()->reDrawX.push_back(iLX);
			GDCore::getMap()->getEvent()->reDrawY.push_back(iLY - i);
			GDCore::getMap()->getEvent()->reDrawX.push_back(iRX);
			GDCore::getMap()->getEvent()->reDrawY.push_back(iRY - i);
		}
	} else if(iType == 1) {
		GDCore::getMap()->getEvent()->newPlayerXPos += 32 - GDCore::getMap()->getPlayer()->getHitBoxX() / 2;

		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eRIGHT);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxX());

		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eNOTHING);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(35);

		GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->ePLAYPIPETOP);
		GDCore::getMap()->getEvent()->vNEWLength.push_back(1);

		GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->eNOTHING);
		GDCore::getMap()->getEvent()->vNEWLength.push_back(35);

		GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->eTOP);
		GDCore::getMap()->getEvent()->vNEWLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxY());

		for(int i = 0; i < 3; i++) {
			GDCore::getMap()->getEvent()->reDrawX.push_back(iLX + i);
			GDCore::getMap()->getEvent()->reDrawY.push_back(iLY);
			GDCore::getMap()->getEvent()->reDrawX.push_back(iRX + i);
			GDCore::getMap()->getEvent()->reDrawY.push_back(iRY);

			GDCore::getMap()->getEvent()->reDrawX.push_back(GDCore::getMap()->getBlockIDX(GDCore::getMap()->getEvent()->newPlayerXPos - GDCore::getMap()->getEvent()->newMapXPos));
			GDCore::getMap()->getEvent()->reDrawY.push_back(GDCore::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
			GDCore::getMap()->getEvent()->reDrawX.push_back(GDCore::getMap()->getBlockIDX(GDCore::getMap()->getEvent()->newPlayerXPos - GDCore::getMap()->getEvent()->newMapXPos) + 1);
			GDCore::getMap()->getEvent()->reDrawY.push_back(GDCore::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
		}
	} else { // -- VERT -> VERT
		GDCore::getMap()->getEvent()->newPlayerXPos -= GDCore::getMap()->getPlayer()->getPowerLVL() > 0 ? 32 : 0 - GDCore::getMap()->getPlayer()->getHitBoxX()/2;
		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eBOT);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxY());

		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eNOTHING);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(55);

		for(int i = 0; i < 3; i++) {
			GDCore::getMap()->getEvent()->reDrawX.push_back(iLX);
			GDCore::getMap()->getEvent()->reDrawY.push_back(iLY - i);
			GDCore::getMap()->getEvent()->reDrawX.push_back(iRX);
			GDCore::getMap()->getEvent()->reDrawY.push_back(iRY - i);

			GDCore::getMap()->getEvent()->reDrawX.push_back(GDCore::getMap()->getBlockIDX(GDCore::getMap()->getEvent()->newPlayerXPos - GDCore::getMap()->getEvent()->newMapXPos));
			GDCore::getMap()->getEvent()->reDrawY.push_back(GDCore::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
			GDCore::getMap()->getEvent()->reDrawX.push_back(GDCore::getMap()->getBlockIDX(GDCore::getMap()->getEvent()->newPlayerXPos - GDCore::getMap()->getEvent()->newMapXPos) + 1);
			GDCore::getMap()->getEvent()->reDrawY.push_back(GDCore::getMap()->getBlockIDY(newPlayerPosY) - 1 - i);
		}

		GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->ePLAYPIPETOP);
		GDCore::getMap()->getEvent()->vNEWLength.push_back(1);

		GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->eNOTHING);
		GDCore::getMap()->getEvent()->vNEWLength.push_back(35);

		GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->eTOP);
		GDCore::getMap()->getEvent()->vNEWLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxY());
	}

	GDCore::getMap()->setInEvent(true);
}