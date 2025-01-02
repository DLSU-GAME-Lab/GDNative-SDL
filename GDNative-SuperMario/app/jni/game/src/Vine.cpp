#include "Vine.h"
#include "GDCore.h"

/* ******************************************** */

Vine::Vine(int iXPos, int iYPos, int minionState, int iBlockID) {
	this->fXPos = (float)iXPos*32 + 4;
	this->fYPos = (float)(CCFG::GAME_HEIGHT - 16 - iYPos*32);

	this->iX = iXPos;
	this->iY = iYPos;

	this->minionState = minionState;
	this->iBlockID = iBlockID;

	this->moveSpeed = 0;

	this->collisionOnlyWithPlayer = true;

	this->jumpDistance = 0;
	this->minionSpawned = true;

	this->iHitBoxX = 24;
	this->iHitBoxY = -32;
}

Vine::~Vine(void) {

}

/* ******************************************** */

void Vine::Update() {
	if(minionState == 0) {
		if(CCFG::GAME_HEIGHT + 16 - iY * 32 >= jumpDistance) {
			jumpDistance += 2;
			iHitBoxY += 2;
			fYPos -= 2;
		}
	} else {
		if(jumpDistance < 256) {
			jumpDistance += 2;
			iHitBoxY += 2;
			fYPos -= 2;
		}
	}
}

void Vine::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	if(jumpDistance < 32) {
		GDCore::getMap()->getMinionBlock(iBlockID - 1)->Draw(rR, (int)(iX*32 + GDCore::getMap()->getXPos()), (int)(CCFG::GAME_HEIGHT - 16 - iY*32 - jumpDistance));
	} else {
		GDCore::getMap()->getMinionBlock(iBlockID - 1)->Draw(rR, (int)(iX*32 + GDCore::getMap()->getXPos()), (int)(CCFG::GAME_HEIGHT - 16 - iY*32 - jumpDistance));
		for(int i = 0; i < jumpDistance/32 - 1; i++) {
			iIMG->Draw(rR, (int)(iX*32 + GDCore::getMap()->getXPos()), (int)(CCFG::GAME_HEIGHT + 16 - iY*32 + i*32 - jumpDistance), false);
		}
	}
	GDCore::getMap()->getBlock(GDCore::getMap()->getMapBlock((int)iX, (int)iY)->getBlockID())->getSprite()->getTexture()->Draw(rR, (int)(iX*32 + GDCore::getMap()->getXPos()), (int)(CCFG::GAME_HEIGHT - iY*32 - 16 - GDCore::getMap()->getMapBlock((int)iX, (int)iY)->getYPos()));
}

void Vine::minionPhysics() { }

/* ******************************************** */

void Vine::collisionWithPlayer(bool TOP) {
	if(minionState == 0) {
		GDCore::getMap()->setInEvent(true);

		GDCore::getMap()->getEvent()->resetData();
		GDCore::getMap()->getPlayer()->resetJump();
		GDCore::getMap()->getPlayer()->stopMove();

		GDCore::getMap()->getEvent()->eventTypeID = GDCore::getMap()->getEvent()->eNormal;

		GDCore::getMap()->getEvent()->iSpeed = 2;

		GDCore::getMap()->getEvent()->newLevelType = GDCore::getMap()->getLevelType();
		GDCore::getMap()->getEvent()->newCurrentLevel = GDCore::getMap()->getCurrentLevelID();
		GDCore::getMap()->getEvent()->newMoveMap = true;
		GDCore::getMap()->getEvent()->iDelay = 0;
		GDCore::getMap()->getEvent()->inEvent = false;

		GDCore::getMap()->getEvent()->newMoveMap = true;
		GDCore::getMap()->getEvent()->newUnderWater = false;

		if(fXPos + iHitBoxX/2 > GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos()) {
			GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eRIGHT);
			GDCore::getMap()->getEvent()->vOLDLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxX() - 4);
		} else {
			GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eLEFT);
			GDCore::getMap()->getEvent()->vOLDLength.push_back(GDCore::getMap()->getPlayer()->getHitBoxX() - 4);
		}

		for(int i = 0; i < GDCore::getMap()->getPlayer()->getYPos() + GDCore::getMap()->getPlayer()->getHitBoxY(); i += 32) {
			GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINE1);
			GDCore::getMap()->getEvent()->vOLDLength.push_back(16);
			GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINE2);
			GDCore::getMap()->getEvent()->vOLDLength.push_back(16);
		}

		switch(GDCore::getMap()->getCurrentLevelID()) {
			case 4: {
				GDCore::getMap()->getEvent()->newMapXPos = -270*32;
				GDCore::getMap()->getEvent()->newPlayerXPos = 128;
				GDCore::getMap()->getEvent()->newPlayerYPos = CCFG::GAME_HEIGHT - GDCore::getMap()->getPlayer()->getHitBoxY();
				GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINESPAWN);
				GDCore::getMap()->getEvent()->vOLDLength.push_back(274);

				break;
			}
			case 8: {
				GDCore::getMap()->getEvent()->newMapXPos = -270*32;
				GDCore::getMap()->getEvent()->newPlayerXPos = 128;
				GDCore::getMap()->getEvent()->newPlayerYPos = CCFG::GAME_HEIGHT - GDCore::getMap()->getPlayer()->getHitBoxY();
				GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINESPAWN);
				GDCore::getMap()->getEvent()->vOLDLength.push_back(274);

				break;
			}
			case 13: {
				GDCore::getMap()->getEvent()->newMapXPos = -310*32;
				GDCore::getMap()->getEvent()->newPlayerXPos = 128;
				GDCore::getMap()->getEvent()->newPlayerYPos = CCFG::GAME_HEIGHT - GDCore::getMap()->getPlayer()->getHitBoxY();
				GDCore::getMap()->getEvent()->newLevelType = 0;

				GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINESPAWN);
				GDCore::getMap()->getEvent()->vOLDLength.push_back(314);
				break;
			}
			case 17: {
				GDCore::getMap()->getEvent()->newMapXPos = -325*32;
				GDCore::getMap()->getEvent()->newPlayerXPos = 128;
				GDCore::getMap()->getEvent()->newPlayerYPos = CCFG::GAME_HEIGHT - GDCore::getMap()->getPlayer()->getHitBoxY();
				GDCore::getMap()->getEvent()->newLevelType = 0;

				GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINESPAWN);
				GDCore::getMap()->getEvent()->vOLDLength.push_back(329);
				break;
			}
			case 21: {
				GDCore::getMap()->getEvent()->newMapXPos = -390*32;
				GDCore::getMap()->getEvent()->newPlayerXPos = 128;
				GDCore::getMap()->getEvent()->newPlayerYPos = CCFG::GAME_HEIGHT - GDCore::getMap()->getPlayer()->getHitBoxY();
				GDCore::getMap()->getEvent()->newLevelType = 4;

				GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eVINESPAWN);
				GDCore::getMap()->getEvent()->vOLDLength.push_back(394);
				break;
			}
		}

		GDCore::getMap()->getEvent()->vOLDDir.push_back(GDCore::getMap()->getEvent()->eNOTHING);
		GDCore::getMap()->getEvent()->vOLDLength.push_back(60);

		for(int i = 0; i < 64; i += 32) {
			GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->eVINE1);
			GDCore::getMap()->getEvent()->vNEWLength.push_back(16);
			GDCore::getMap()->getEvent()->vNEWDir.push_back(GDCore::getMap()->getEvent()->eVINE2);
			GDCore::getMap()->getEvent()->vNEWLength.push_back(16);
		}
		
	} else {

	}
}