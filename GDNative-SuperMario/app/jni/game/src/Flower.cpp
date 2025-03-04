#include "Flower.h"
#include "GDCore.h"

/* ******************************************** */

Flower::Flower(int iXPos, int fYPos, int iX, int iY) {
	this->fXPos = (float)iXPos;
	this->fYPos = (float)fYPos;

	this->iBlockID = 6;
	this->moveSpeed = 2;
	this->inSpawnState = true;
	this->minionSpawned = true;
	this->inSpawnY = 32;
	this->moveDirection = false;
	this->collisionOnlyWithPlayer = true;

	this->iX = iX;
	this->iY = iY;
}

Flower::~Flower(void) {

}

/* ******************************************** */

void Flower::Update() {
	if (inSpawnState) {
		if (inSpawnY <= 0) {
			inSpawnState = false;
		} else {
			if (fYPos > -5) {
				inSpawnY -= 2;
				fYPos -= 2;
			} else {
				inSpawnY -= 1;
				fYPos -= 1;
			}
		}
	}
}

bool Flower::updateMinion() {
	return minionSpawned;
}

void Flower::Draw(SDL_Renderer* rR, CIMG* iIMG) {
	if(minionState >= 0) {
		iIMG->Draw(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos + 2, false);
		if (inSpawnState) {
			GDCore::getMap()->getBlock(GDCore::getMap()->getLevelType() == 0 || GDCore::getMap()->getLevelType() == 4 ? 9 : 56)->getSprite()->getTexture()->Draw(rR, (int)fXPos + (int)GDCore::getMap()->getXPos(), (int)fYPos + (32 - inSpawnY) - GDCore::getMap()->getMapBlock(iX, iY)->getYPos() + 2, false);
		}
	}
}

/* ******************************************** */

void Flower::collisionWithPlayer(bool TOP) {
	if(!inSpawnState && minionState >= 0) {
		GDCore::getMap()->getPlayer()->setPowerLVL(GDCore::getMap()->getPlayer()->getPowerLVL() + 1);
		minionState = -1;
	}
}

void Flower::setMinionState(int minionState) { }