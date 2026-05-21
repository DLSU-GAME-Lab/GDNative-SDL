#pragma once
#include "AGameObject.h"
#include "Light2D.h"

class RadialLight : public AGameObject
{
private:
	Light2D* pLight;
	SDL_Color color;
	float radius;

public:
	RadialLight(const std::string& strName, float radius = 256.0f, SDL_Color color = { 255, 255, 255, 255 });
	virtual ~RadialLight();

	virtual void initialize() override;

	SDL_Color getColor();
	void setColor(SDL_Color color);
	Light2D::FalloffType getFalloffType();
	void setFalloffType(Light2D::FalloffType falloffType);
	};

