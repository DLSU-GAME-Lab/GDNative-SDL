#include "CollectableGemDataAsset.h"

void CollectableGemDataAsset::initialize()
{

	this->mapGemData["Gem_Cyan"] = {
		"Character",
		"The Young Moth is the Main Character",
		"The Young Moth is the Main \n Character",
		false
	};
	this->mapGemData["Gem_Green"] = {
		"Initiating Event",
		"To Experience the flame's beauty",
		"To Experience the flame's \n beauty",
		false
	};
	this->mapGemData["Gem_Orange"] = {
		"Problem",
		"it was deadly",
		"it was deadly",
		false
	};
	this->mapGemData["Gem_Purple"] = {
		"Plan",
		"She ignored her mother's warning",
		"She ignored her mother's \n warning",
		false
	};
	this->mapGemData["Gem_Red"] = {
		"Consequence",
		"She died, teaching the narrator to respect danger",
		"She died, teaching the \n narrator to respect danger",
		false
	};
}

unsigned int CollectableGemDataAsset::getGemCount()
{
	return static_cast<unsigned int>(this->mapGemData.size());
}

std::string CollectableGemDataAsset::getGemTitle(std::string strGemName)
{
	if (this->mapGemData.find(strGemName) != this->mapGemData.end())
	{
		return this->mapGemData[strGemName].strTitle;
	}

	return std::string();
}

std::string CollectableGemDataAsset::getGemText(std::string strGemName)
{
	if (this->mapGemData.find(strGemName) != this->mapGemData.end())
	{
		return this->mapGemData[strGemName].strText;
	}

	return std::string();
}

std::string CollectableGemDataAsset::getGemTextBreak(std::string strGemName)
{
	if (this->mapGemData.find(strGemName) != this->mapGemData.end())
	{
		return this->mapGemData[strGemName].strTextBreak;
	}
	return std::string();
}

bool CollectableGemDataAsset::getGemPickedUp(std::string strGemName)
{
	if (this->mapGemData.find(strGemName) != this->mapGemData.end())
	{
		return this->mapGemData[strGemName].bPickedUp;
	}
	return false;
}

void CollectableGemDataAsset::setGemPickedUp(std::string strGemName, bool bPickedUp)
{
	this->mapGemData[strGemName].bPickedUp = bPickedUp;
}
