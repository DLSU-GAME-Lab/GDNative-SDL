#include "Goombas.h"
#include "GDCore.h"

/* ******************************************** */

Goombas::Goombas(int iX, int iY, int iBlockID, bool moveDirection) {
	this->fXPos = (float)iX;
	this->fYPos = (float)iY;
	this->iBlockID = iBlockID;
	this->moveDirection = moveDirection;
	this->moveSpeed = 1;
}

Goombas::~Goombas(void) {

}

/* ******************************************** */

void Goombas::Update() {
	if (minionState == 0) {
		updateXPos();
	} else if(minionState == -2) {
		Minion::minionDeathAnimation();
	} else if (SDL_GetTicks() - 500 >= (unsigned)deadTime) {
		minionState = -1;
	}
}

void Goombas::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	if(minionState != -2) {
		iIMG->Draw(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos + 2, false);
	} else {
		iIMG->DrawVert(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos + 2);
	}
}

/* ******************************************** */

void Goombas::collisionWithPlayer(bool TOP) {
	if(GDCore::getMap()->getPlayer()->getStarEffect()) {
		setMinionState(-2);
	} else if(TOP) {
		if(minionState == 0) {
			minionState = 1;
			iBlockID = GDCore::getMap()->getLevelType() == 0 || GDCore::getMap()->getLevelType() == 4 ? 1 : GDCore::getMap()->getLevelType() == 1 ? 9 : 11;
			deadTime = SDL_GetTicks();
			GDCore::getMap()->getPlayer()->resetJump();
			GDCore::getMap()->getPlayer()->startJump(1);
			points(100);
			CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cSTOMP);
		}
	} else {
		GDCore::getMap()->playerDeath(true, false);
	}
}

/* ******************************************** */

void Goombas::setMinionState(int minionState) {
	this->minionState = minionState;

	if (this->minionState == 1) {
		deadTime = SDL_GetTicks();
	}

	Minion::setMinionState(minionState);
}