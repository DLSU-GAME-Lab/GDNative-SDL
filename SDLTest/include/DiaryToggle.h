#pragma once
#include "AComponent.h"
class DiaryToggle : public AComponent
{
private:
	std::string strDiaryName;
public:
	DiaryToggle(std::string strDiaryName);
	~DiaryToggle();
public:
	void perform() override;
};

