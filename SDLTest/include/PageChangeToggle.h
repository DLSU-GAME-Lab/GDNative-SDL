#pragma once
#include "AComponent.h"
class PageChangeToggle : public AComponent
{
private:
	std::string strName;
	bool bProgress;
public:
	PageChangeToggle(std::string strName, bool bProgress);
	~PageChangeToggle();

	// Inherited via AComponent
	void perform() override;
};

