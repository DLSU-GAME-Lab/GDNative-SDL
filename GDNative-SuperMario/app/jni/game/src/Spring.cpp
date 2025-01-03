#include "Spring.h"
#include "GDCore.h"

/* ******************************************** */

Spring::Spring(int iXPos, int iYPos) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)iYPos + 2;

	this->iHitBoxX = 32;
	this->iHitBoxY = 72;

	this->iBlockID = GDCore::getMap()->getLevelType() == 0 || GDCore::getMap()->getLevelType() == 4 ? 37 : 40;

	this->minionState = 0;
	this->nextFrameID = 4;
	this->inAnimation = false;
}

Spring::~Spring(void) {

}

/* ******************************************** */

void Spring::Update() {
	if(inAnimation) {
		if(CCFG::keySpace) {
			extraJump = true;
			GDCore::getMap()->getPlayer()->resetJump();
			GDCore::getMap()->getPlayer()->setNextFallFrameID(16);
		} else {
			GDCore::getMap()->getPlayer()->stopMove();
		}

		GDCore::getMap()->getPlayer()->setMarioSpriteID(5);
		if(nextFrameID <= 0) {
			switch(minionState) {
				case 0:
					iBlockID = iBlockID == 37 ? 38 : 41;
					minionState = 1;
					GDCore::getMap()->getPlayer()->setYPos(GDCore::getMap()->getPlayer()->getYPos() + 16.0f);
					break;
				case 1:
					iBlockID = iBlockID == 38 ? 39 : 42;
					minionState = 2;
					GDCore::getMap()->getPlayer()->setYPos(GDCore::getMap()->getPlayer()->getYPos() + 16.0f);
					break;
				case 2:
					iBlockID = iBlockID == 39 ? 38 : 41;
					minionState = 3;
					GDCore::getMap()->getPlayer()->setYPos(GDCore::getMap()->getPlayer()->getYPos() - 16.0f);
					break;
				case 3:
					iBlockID = iBlockID == 38 ? 37 : 40;
					minionState = 0;
					GDCore::getMap()->getPlayer()->setYPos(GDCore::getMap()->getPlayer()->getYPos() - 16.0f);
					GDCore::getMap()->getPlayer()->resetJump();
					GDCore::getMap()->getPlayer()->startJump(4 + (extraJump ? 5 : 0));
					GDCore::getMap()->getPlayer()->setSpringJump(true);
					GDCore::getMap()->getPlayer()->startMove();
					if(extraJump) GDCore::getMap()->getPlayer()->setCurrentJumpSpeed(10.5f);
					inAnimation = false;
					break;
			}
			nextFrameID = 4;
		} else {
			--nextFrameID;
		}
	}
}

void Spring::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	iIMG->Draw(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos, false);
}

void Spring::minionPhysics() { }

/* ******************************************** */

void Spring::collisionWithPlayer(bool TOP) {
	if(!inAnimation) {
		if(TOP && GDCore::getMap()->getPlayer()->getJumpState() == 2) {
			GDCore::getMap()->getPlayer()->stopMove();
			GDCore::getMap()->getPlayer()->resetJump();
			GDCore::getMap()->getPlayer()->setNextFallFrameID(16);
			inAnimation = true;
			extraJump = false;
			CCFG::keySpace = false;
		} else {
			if(GDCore::getMap()->getPlayer()->getMoveDirection()) {
				GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getPlayer()->getMoveSpeed());
			} else {
				GDCore::getMap()->getPlayer()->setXPos((float)GDCore::getMap()->getPlayer()->getXPos() + GDCore::getMap()->getPlayer()->getMoveSpeed());
			}
		}
	}
}

void Spring::setMinionState(int minionState) { }