#include "Light2D.h"
#include "RendererContext.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "AGameObject.h"

Light2D::Light2D(const std::string& texName) : ARenderer("Light2D")
{
	this->pTexture = TextureManager::getInstance()->get(texName);
	this->mColor = { 255, 255, 255, 255 };
	this->size = Vector2D(512.0f, 512.0f);
	this->flipX = false;
	this->flipY = false;
}

void Light2D::drawLight()
{
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
	SDL_FRect dstRect = getDstRect();
	if (!pCam->isInView(dstRect)) return;

	float worldRot = this->pOwner->getRot() + pCam->getRot();

	SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_ADD);

	RendererContext::getInstance()->draw(
		this->pTexture,
		this->mColor,
		NULL,
		&dstRect,
		worldRot,
		this->flipX,
		this->flipY
	);
}

void Light2D::drawLightWidget()
{
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
	AGameObject* owner = this->getOwner();
	SDL_FRect dstRect = getDstRect();

	SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 0, 127);
	SDL_RenderRect(pRenderer, &dstRect);
}

SDL_FRect Light2D::getDstRect() const
{
	AGameObject* owner = this->getOwner();
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

	Vector2D scale = owner->getScale();
	Vector2D screenSize = this->size * scale;
	Vector2D pos = owner->getPos();
	pos -= size * Vector2D(0.5f);

	SDL_FRect dstRect = {};
	dstRect.x = pos.x;
	dstRect.y = pos.y;
	dstRect.w = screenSize.x;
	dstRect.h = screenSize.y;

	return pCam->worldToScreenRect(dstRect);
}

Vector2D Light2D::getSize() const
{
	return this->size;
}

void Light2D::setSize(Vector2D size)
{
	this->size = size;
}

bool Light2D::getFlipX() const
{
	return this->flipX;
}

void Light2D::setFlipX(bool flipX)
{
	this->flipX = flipX;
}

bool Light2D::getFlipY() const
{
	return this->flipY;
}

void Light2D::setFlipY(bool flipY)
{
	this->flipY = flipY;
}
