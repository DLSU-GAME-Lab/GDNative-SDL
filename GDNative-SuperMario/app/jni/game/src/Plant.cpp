#include "Plant.h"
#include "GDCore.h"

/* ******************************************** */

Plant::Plant(int fXPos, int fYPos, int iBlockID) {
	this->fXPos = (float)fXPos + 2;
	this->fYPos = (float)fYPos + 6;

	this->iHitBoxX = 28;
	this->iHitBoxY = 38;

	this->iBlockID = iBlockID;

	this->bWait = true;
	this->iTime = SDL_GetTicks();

	this->lockPlant = false;

	// -- true = TOP, false = BOT
	this->moveDirection = true;

	this->iLength = 50;

	this->X = GDCore::getMap()->getBlockIDX((int)fXPos);
	this->Y = GDCore::getMap()->getBlockIDY((int)fYPos);
}

Plant::~Plant(void) {

}

/* ******************************************** */

void Plant::Update() {
	lockPlant = GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() >= fXPos - 48 && GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() <= fXPos + iHitBoxX + 48;

	if(bWait) {
		if((!lockPlant || !moveDirection) && iTime + (moveDirection ? 1000 : 985) <= SDL_GetTicks()) {
			bWait = false;
		} else if(lockPlant && moveDirection) {
			iTime = SDL_GetTicks();
		}
	} else if(moveDirection) {
		if(iLength > 0) {
			fYPos -= 1;
			iLength -= 1;
		} else {
			moveDirection = false;
			iLength = 50;
			bWait = true;
			iTime = SDL_GetTicks();
		}
	} else {
		if(iLength > 0) {
			fYPos += 1;
			iLength -= 1;
		} else if(!lockPlant) {
			moveDirection = true;
			iLength = 50;
			bWait = true;
			iTime = SDL_GetTicks();
		}
	}
}

void Plant::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	iIMG->Draw(rR, (int)(fXPos + GDCore::getMap()->getXPos()) - 2, (int)fYPos - 6, false);

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 2; j++) {
			GDCore::getMap()->getBlock(GDCore::getMap()->getMapBlock(X + j, Y - i)->getBlockID())->Draw(rR, (int)((X + j) * 32 + GDCore::getMap()->getXPos()), CCFG::GAME_HEIGHT - 32 * (Y - i) - 16);
		}
	}
}

void Plant::minionPhysics() {

}

/* ******************************************** */

void Plant::collisionWithPlayer(bool onTOP) {
	if(GDCore::getMap()->getPlayer()->getStarEffect()) {
		minionState = -1;
		points(200);
	} else {
		GDCore::getMap()->playerDeath(true, false);
	}
}

void Plant::collisionEffect() { }

void Plant::lockMinion() {
	if(moveDirection) {
		fYPos += 50 - iLength;
	} else {
		fYPos += iLength;
	}
	moveDirection = true;
	iLength = 50;
	lockPlant = true;
	bWait = true;
	iTime = SDL_GetTicks();
}

void Plant::setMinionState(int minionState) {
	if(minionState < 0) {
		this->minionState = -1;
		points(200);
	} else {
		this->minionState = minionState;
	}
}

/* ******************************************** */