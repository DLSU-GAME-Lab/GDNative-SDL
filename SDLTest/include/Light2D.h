#pragma once
#include "ARenderer.h"

class Light2D : public ARenderer
{
public:
	enum class LightType
	{
		RADIAL,
		RECT
	};

	enum class FalloffType
	{
		LINEAR,
		QUADRATIC,
		SMOOTHSTEP,
		EXPONENTIAL,
		HARD_CUTOFF
	};

private:
	SDL_Texture* pTexture;
	Vector2D size;
	float radius;

	LightType type = LightType::RADIAL;
	FalloffType falloffType = FalloffType::LINEAR;

	float getFalloffValue(float distance) const;

public:
	Light2D(FalloffType falloffType, float radius);
	Light2D(FalloffType falloffType, Vector2D size);
	virtual ~Light2D();

	virtual void drawWidget() override {};
	virtual void perform() override {};

	void drawLight();
	void drawLightWidget();

	void createRadialLightTexture(float radius);
	void createRectLightTexture(Vector2D size);

	SDL_FRect getDstRect() const;

	float getRadius() const;
	void setRadius(float radius);
	Vector2D getSize() const;
	void setSize(Vector2D size);
};

