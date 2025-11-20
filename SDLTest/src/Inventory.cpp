#include "Inventory.h"

Inventory::Inventory(std::string strName):AComponent(strName, ComponentType::SCRIPT)
{
	this->vecItems = {};
}

Inventory::~Inventory()
{
}

void Inventory::perform()
{
}
