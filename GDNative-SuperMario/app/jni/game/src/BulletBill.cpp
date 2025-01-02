#include "BulletBill.h"
#include "GDCore.h"

/* ******************************************** */

BulletBill::BulletBill(int iXPos, int iYPos, bool moveDirection, int minionState) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos - 2;

	this->moveDirection = GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() + GDCore::getMap()->getPlayer()->getHitBoxX()/32 < fXPos + iHitBoxX/32;
	this->moveSpeed = 4;

	this->iHitBoxY = 30;

	this->collisionOnlyWithPlayer = true;

	this->iBlockID = GDCore::getMap()->getCurrentLevelID() == 22 ? 60 : 59;

	this->minionSpawned = true;

	this->minionState = minionState;

	this->currentJumpSpeed = (float)GDCore::getMap()->getBlockIDX((int)(fXPos + 16));
	this->currentFallingSpeed = (float)GDCore::getMap()->getBlockIDY((int)(fYPos));

	this->currentJumpDistance = (float)(GDCore::getMap()->getCurrentLevelID() == 17 ? 73*32 : 0); // -- MIN X
	this->jumpState = GDCore::getMap()->getCurrentLevelID() == 17 ? 303*32 : GDCore::getMap()->getMapWidth()*32; // -- MAX X

	CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cBULLETBILL);
}

BulletBill::~BulletBill(void) {

}

/* ******************************************** */

void BulletBill::Update() {
	if(minionState != -2) {
		fXPos += moveDirection ? -moveSpeed : moveSpeed;

		if(fXPos + iHitBoxX < currentJumpDistance || fXPos > jumpState) {
			minionState = -1;
		}
	} else {
		Minion::minionDeathAnimation();
	}
}

void BulletBill::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	if(minionState != -2) {
		iIMG->Draw(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos + 2, moveDirection);
	} else {
		iIMG->DrawVert(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos);
	}

	if(minionState == 0) GDCore::getMap()->getBlock(145)->getSprite()->getTexture()->Draw(rR,(int)(currentJumpSpeed*32 + GDCore::getMap()->getXPos()), (int)(CCFG::GAME_HEIGHT - 16 - currentFallingSpeed*32) + 32);
}

void BulletBill::minionPhysics() { }

/* ******************************************** */

void BulletBill::collisionWithPlayer(bool TOP) {
	if(GDCore::getMap()->getPlayer()->getStarEffect() || TOP) {
		setMinionState(-2);
	} else {
		GDCore::getMap()->playerDeath(true, false);
	}
}