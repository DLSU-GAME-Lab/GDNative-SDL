#pragma once
#include "AGameObject.h"
class RadialLight : public AGameObject
{
private:
	SDL_Color color;

public:
	RadialLight(const std::string& strName, SDL_Color color);
	virtual ~RadialLight();

	virtual void initialize() override;

	SDL_Color getColor();
	void setColor(SDL_Color color);
};

