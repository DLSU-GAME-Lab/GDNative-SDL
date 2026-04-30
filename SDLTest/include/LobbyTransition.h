#pragma once
#include "AComponent.h"
#include "ButtonInput.h"

class LobbyPlayer;

class LobbyTransition : public AComponent
{
private:
	ButtonInput* pInput;
	LobbyPlayer* pPlayer;
	bool bRight;

public:
	LobbyTransition(bool bRight);

	virtual void onAttach() override;
	virtual void perform() override;
};

