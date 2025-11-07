#pragma once
#include "AGeneralInput.h"
#include "ScrollView.h"
class ScrollInput: public AGeneralInput
{
private:
	ScrollView* pScrollView;
public:
	ScrollInput(ScrollView* pScrollView);
	~ScrollInput();

	void perform() override;

};

