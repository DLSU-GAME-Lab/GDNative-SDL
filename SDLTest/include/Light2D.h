#pragma once
#include "ARenderer.h"

class Light2D : public ARenderer
{
private:
	SDL_Texture* pTexture;
	Vector2D size;
	bool flipX, flipY;

public:
	Light2D(const std::string& strName, const std::string& texName);

	virtual void drawWidget() override;
	virtual void perform() override;

	SDL_FRect getDstRect() const;

	Vector2D getSize() const;
	void setSize(Vector2D size);
	bool getFlipX() const;
	void setFlipX(bool flipX);
	bool getFlipY() const;
	void setFlipY(bool flipY);
};

