#pragma once
#include "ADataAsset.h"
#include <string>
#include <vector>
#include <unordered_map>

class CollectableGemDataAsset : public ADataAsset
{
private:
	struct GemData
	{
		std::string strTitle;
		std::string strText;
		std::string strTextBreak;
		bool bPickedUp;
	};
	std::unordered_map<std::string, GemData> mapGemData;

public:
	void initialize() override;
	unsigned int getGemCount();
	std::string getGemTitle(std::string strGemName);
	std::string getGemText(std::string strGemName);
	std::string getGemTextBreak(std::string strGemName);
	bool getGemPickedUp(std::string strGemName);
	void setGemPickedUp(std::string strGemName, bool bPickedUp);
};

