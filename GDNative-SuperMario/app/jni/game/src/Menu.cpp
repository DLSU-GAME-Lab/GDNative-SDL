#include "Menu.h"
#include "CFG.h"

/* ******************************************** */

Menu::Menu(void) {
	this->activeMenuOption = 0;
}


Menu::~Menu(void) {

}

/* ******************************************** */

void Menu::Update() {

}

void Menu::Draw(SDL_Renderer* rR) {
	for(unsigned int i = 0; i < lMO.size(); i++) {
		CCFG::getText()->Draw(rR, lMO[i]->getText(), lMO[i]->getXPos(), lMO[i]->getYPos());
	}

	CCFG::getMM()->getActiveOption()->Draw(rR, lMO[activeMenuOption]->getXPos() - 32, lMO[activeMenuOption]->getYPos());
}

/* ******************************************** */

void Menu::updateActiveButton(int iDir) {
	switch(iDir) {
		case 0:
            activeMenuOption = (activeMenuOption == 0) ? numOfMenuOptions - 1 : activeMenuOption - 1;
			break;
		case 2:
            activeMenuOption = (activeMenuOption + 1) % numOfMenuOptions;
            break;
		default:
			break;
	}
}