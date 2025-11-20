#pragma once
#include "AComponent.h"
#include "AGameObject.h"
#include "vector"
#include "unordered_map"
#include "Prop.h"
class Inventory:public AComponent
{
private:
	std::unordered_map<std::string, bool> mapInventoryStored;
	std::vector<Prop*> vecItems;
public:
	Inventory(std::string strName);
	~Inventory();

	// Inherited via AComponent
	void perform() override;
	void addItem(AGameObject* pObject);

};

