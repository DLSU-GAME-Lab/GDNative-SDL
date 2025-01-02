#include "Bowser.h"
#include "GDCore.h"
#include "stdlib.h"
#include "time.h"

/* ******************************************** */

Bowser::Bowser(float fXPos, float fYPos, bool spawnHammer) {
	this->fXPos = fXPos;
	this->fYPos = fYPos;
	
	this->iHitBoxX = this->iHitBoxY = 64;

	this->iBlockID = 20;
	this->moveDirection = true;

	this->nextJumpFrameID = 128;
	this->nextFireFrameID = 1;

	this->minionSpawned = true;

	this->iMinBlockID = (int)fXPos/32 - 4;
	this->iMaxBlockID = (int)fXPos/32 + 4;

	this->collisionOnlyWithPlayer = true;

	this->moveDir = true;

	this->iYFireStart = (int)fYPos - 32;
	this->iFireID = 0;

	srand((unsigned)time(NULL));

	this->nextHammerFrameID = 128;
	this->numOfHammers = 3 + rand()%6;
	this->spawnHammer = spawnHammer;
}

Bowser::~Bowser(void) {

}

/* ******************************************** */

void Bowser::Update() {
	moveDirection = GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos() < fXPos + iHitBoxX;

	if(!GDCore::getMap()->getInEvent()) {
		if(nextJumpFrameID < 1 && jumpState == 0) {
			startJump(2);
			nextJumpFrameID = 110 + rand() % 100;
		} else if(jumpState == 0) {
			--nextJumpFrameID;
		}

		if(moveDirection && GDCore::getMap()->getBlockIDX((int)(GDCore::getMap()->getPlayer()->getXPos() - GDCore::getMap()->getXPos())) > GDCore::getMap()->getMapWidth()/2) {
			if(nextFireFrameID < 1) {
				createFire();
				nextFireFrameID = 150 + rand() % 115;
			} else if(nextFireFrameID < 30) {
				--nextFireFrameID;
				iBlockID = 21;
			} else {
				--nextFireFrameID;
				iBlockID = 20;
			}
		}
	
		if(!moveDirection) {
			moveDir = false;
		} else if(rand() % 275 == 0) {
			moveDir = !moveDir;
		}

		if(moveDir) {
			if(GDCore::getMap()->getBlockIDX((int)fXPos) <= iMinBlockID) {
				moveDir = false;
			} else {
				fXPos -= (rand()%4 == 0 ? 0.5f : 1);
			}
		} else {
			if(GDCore::getMap()->getBlockIDX((int)fXPos) >= iMaxBlockID) {
				moveDir = true;
			} else {
				fXPos += (rand()%4 == 0 ? 0.5f : 1);
			}
		}

		if(spawnHammer && moveDirection) {
			if(nextHammerFrameID <= 0) {
				if(numOfHammers > 0) {
					nextHammerFrameID = 8;
					--numOfHammers;
					GDCore::getMap()->addHammer((int)(fXPos + iHitBoxX/4), (int)(fYPos - 18), !moveDirection);
				} else {
					nextHammerFrameID = 45 + rand()%55;
					numOfHammers = 2 + rand()%9;
				}
			} else {
				--nextHammerFrameID;
			}
		}
	}
}

void Bowser::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	iIMG->Draw(rR,(int)(fXPos + GDCore::getMap()->getXPos()), (int)fYPos, !moveDirection);

	if(spawnHammer) {
		if(nextHammerFrameID <= 24 && moveDirection) {
			GDCore::getMap()->getMinionBlock(61)->getSprite()->getTexture()->Draw(rR, (int)(fXPos + GDCore::getMap()->getXPos() + 4), (int)(fYPos - 20), moveDirection);
		}
	}
}

void Bowser::minionPhysics() {
	if (jumpState == 1) {
		if (jumpDistance <= currentJumpDistance + 16) {
			fYPos -= 1;
			currentJumpDistance += 1;
		} else {
			fYPos -= 2;
				currentJumpDistance += 2;
		}

		if (jumpDistance <= currentJumpDistance) {
			jumpState = 2;
			currentJumpDistance = 0;
		}
	} else {
		if (!GDCore::getMap()->checkCollisionLB((int)fXPos + 2, (int)fYPos + 2, iHitBoxY, true) && !GDCore::getMap()->checkCollisionRB((int)fXPos - 2, (int)fYPos + 2, iHitBoxX, iHitBoxY, true)) {
			if(currentJumpDistance < 16) {
				currentJumpDistance += 1;
				updateYPos(1);
			} else {
				updateYPos(2);
			}
		} else {
			jumpState = 0;
		}
	}
}

void Bowser::createFire() {
	GDCore::getMap()->addFire(fXPos - 40, fYPos + 16, iYFireStart + 16 * (rand()%4 + 1) + 6);
	CCFG::getMusic()->PlayChunk(CCFG::getMusic()->cFIRE);
	++iFireID;

	if(iFireID > 2) {
		nextFireFrameID += 88 + rand() % 110;
		iFireID = 0;
	}
}

/* ******************************************** */

void Bowser::collisionWithPlayer(bool TOP) {
	GDCore::getMap()->playerDeath(true, false);
}