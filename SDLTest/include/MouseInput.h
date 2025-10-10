#pragma once
#include "AGeneralInput.h"
#include "IMouseInputListener.h"
#include <vector>

class MouseInput : public AGeneralInput
{
private:
	std::vector<IMouseInputListener*> vecListener;

public:
	MouseInput();
	~MouseInput();

	virtual void onAttach() override;
	virtual void perform() override;

	void addListener(IMouseInputListener* pListener);
	void removeListener(IMouseInputListener* pListener);

	bool contains(SDL_FRect spriteRect, Vector2D mousePos) const;
};

