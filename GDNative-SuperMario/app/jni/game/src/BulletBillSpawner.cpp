#include "BulletBillSpawner.h"
#include "GDCore.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

BulletBillSpawner::BulletBillSpawner(int iXPos, int iYPos, int minionState) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos;

	this->moveSpeed = 0;

	this->collisionOnlyWithPlayer = true;

	this->minionState = minionState;

	this->iBlockID = 0;
	this->minionSpawned = minionState != 0;

	srand((unsigned)time(NULL));

	this->nextBulletBillFrameID = 12;
}

BulletBillSpawner::~BulletBillSpawner(void) {

}

/* ******************************************** */

void BulletBillSpawner::Update() {
	if(nextBulletBillFrameID <= 0) {
		if(minionState == 0) {
			if(fXPos > -GDCore::getMap()->getXPos() - 64 && fXPos < -GDCore::getMap()->getXPos() + CCFG::GAME_WIDTH + 64 + iHitBoxX) {
				if(!(GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 > fXPos - 96 && GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 < fXPos + 96)) {
					GDCore::getMap()->addBulletBill((int)fXPos, (int)fYPos - 14, true, minionState);
					nextBulletBillFrameID = 145 + rand()%145;
				}
			}
		} else {
			GDCore::getMap()->addBulletBill((int)(-GDCore::getMap()->getXPos() + CCFG::GAME_WIDTH + iHitBoxX * 2), (int)fYPos - rand()%9*32 - 16, true, minionState);
			nextBulletBillFrameID = 80 + rand()%96;
		}
	} else {
		if(!(GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 > fXPos - 96 && GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/2 < fXPos + 96)) {
			--nextBulletBillFrameID;
		}
	}
}

void BulletBillSpawner::minionPhysics() { }

/* ******************************************** */