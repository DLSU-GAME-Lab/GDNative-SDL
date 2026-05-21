#include "Light2D.h"
#include "RendererContext.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "AGameObject.h"

Light2D::Light2D(FalloffType falloffType, float radius) : ARenderer("Light2D")
{
	this->type = type;
	this->falloffType = falloffType;
	this->pRenderer = RendererContext::getInstance()->getRenderer();
	this->mColor = { 255, 255, 255, 255 };
	this->type = LightType::RADIAL;
	this->radius = radius;
	createRadialLightTexture(radius);
	//this->pTexture = TextureManager::getInstance()->get("radial_light");
	this->size = this->pTexture ? Vector2D(this->pTexture->w, this->pTexture->h) : Vector2D(this->radius * 2);
	SDL_Log("Size set to: %.2f, %.2f", this->size.x, this->size.y);
}

Light2D::Light2D(FalloffType falloffType, Vector2D size) : ARenderer("Light2D")
{
	this->type = type;
	this->falloffType = falloffType;
	this->pRenderer = RendererContext::getInstance()->getRenderer();
	this->mColor = { 255, 255, 255, 255 };
	this->type = LightType::RECT;
	this->size = size;
	createRectLightTexture(this->size);
}

Light2D::~Light2D()
{
	SDL_DestroyTexture(this->pTexture);
}

void Light2D::drawLight()
{
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();
	SDL_FRect dstRect = getDstRect();
	if (!pCam->isInView(dstRect)) return;

	//SDL_SetRenderDrawBlendMode(this->pRenderer, SDL_BLENDMODE_ADD);

	float worldRot = this->pOwner->getRot() + pCam->getRot();
	RendererContext::getInstance()->draw(
		this->pTexture,
		this->mColor,
		NULL,
		&dstRect,
		worldRot,
		false,
		false
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

	Vector2D pos = pCam->worldToScreenPoint(owner->getPos());
	SDL_FRect pivotRect = { pos.x - 4, pos.y - 4, 8, 8 };
	SDL_SetRenderDrawColor(pRenderer, 255, 255, 0, 255);
	SDL_RenderFillRect(pRenderer, &pivotRect);
}

float Light2D::getFalloffValue(float distance) const
{
	switch (falloffType)
	{
	case FalloffType::LINEAR:
		return 1.0f - distance;
	case FalloffType::QUADRATIC:
		return 1.0f - powf(distance, 2.0f);
	case FalloffType::SMOOTHSTEP:
		return 1.0f - (distance * distance * (3.f - 2.f * distance));
	case FalloffType::EXPONENTIAL:
		return expf(-distance * distance * 3.0f); // Adjust exponent for sharper/softer falloff
	case FalloffType::HARD_CUTOFF:
		return 1.0f - SDL_clamp((distance - 0.8f) / 0.2f, 0.0f, 1.0f);
	default:
		break;
	}
}

void Light2D::createRadialLightTexture(float radius)
{
	type = LightType::RADIAL;
	int diameter = radius * 2;

	// Create a surface to write pixels onto
	SDL_Surface* pSurface = SDL_CreateSurface(diameter, diameter, SDL_PIXELFORMAT_RGBA8888);
	if (!pSurface)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Light2D] ERROR: Surface creation failed; %s", SDL_GetError());
		return;
	}

	SDL_LockSurface(pSurface);

	Uint32* pixels = (Uint32*)pSurface->pixels;

	for (int y = 0; y < diameter; y++)
	{
		for (int x = 0; x < diameter; x++)
		{
			// Distance from center, normalized 0.0 - 1.0
			float dx = (x - radius) / (float)radius;
			float dy = (y - radius) / (float)radius;
			float dist = sqrtf(dx * dx + dy * dy);

			// Clamp to circle boundary
			if (dist > 1.0f) dist = 1.0f;

			// Falloff curve adjust exponent for softer/harder edge
			// 1.0 = linear, 2.0 = quadratic (softer), 0.5 = sharper
			float falloff = getFalloffValue(dist);

			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;
			Uint8 a = (Uint8)(255 * falloff);

			pixels[y * diameter + x] = SDL_MapRGBA(SDL_GetPixelFormatDetails(pSurface->format), nullptr, r, g, b, a);
			//if (x % 32 == 0 && y % 32 == 0) // Log every 32 pixels for debugging
			//	SDL_Log("Set pixel at (%d, %d) to RGBA(%d, %d, %d, %d) with falloff %.2f", x, y, r, g, b, a, falloff);
		}
	}

	SDL_UnlockSurface(pSurface);

	const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(pSurface->format);
	if (!fmt)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Light2D] ERROR: Failed to get pixel format; %s", SDL_GetError());
		return;
	}

	// Convert surface to texture
	this->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
	SDL_DestroySurface(pSurface);

	if (!pTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Light2D] ERROR: Texture creation failed; %s", SDL_GetError());
		return;
	}
	else SDL_Log("light texture was created with dimensions: %d, %d", pTexture->w, pTexture->w);

	SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_ADD);
}

void Light2D::createRectLightTexture(Vector2D size)
{
	type == LightType::RECT;
	SDL_Surface* pSurface = SDL_CreateSurface(size.x, size.y, SDL_PIXELFORMAT_RGBA8888);
	if (!pSurface)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Light2D] ERROR: Surface creation failed; %s", SDL_GetError());
		return;
	}

	SDL_LockSurface(pSurface);

	Uint32* pixels = (Uint32*)pSurface->pixels;

	for (int y = 0; y < size.y; y++)
	{
		for (int x = 0; x < size.x; x++)
		{
			// Normalized distance from each edge (0.0 at edge, 1.0 at center)
			float fromLeft = x / size.x;
			float fromRight = 1.0f - fromLeft;
			float fromTop = y / size.y;
			float fromBottom = 1.0f - fromTop;

			// Closest edge distance
			float edgeDist = SDL_min(SDL_min(fromLeft, fromRight), SDL_min(fromTop, fromBottom));

			// Softness controls how far the fade extends inward (0.0 = hard, 0.5 = very soft)
			float softness = 0.2f;
			float falloff = SDL_clamp(edgeDist / softness, 0.0f, 1.0f);

			// Smooth the falloff curve
			falloff = falloff * falloff * (3.0f - 2.0f * falloff);

			Uint8 r = (Uint8)(mColor.r * falloff);
			Uint8 g = (Uint8)(mColor.g * falloff);
			Uint8 b = (Uint8)(mColor.b * falloff);
			Uint8 a = (Uint8)(mColor.a * falloff);

			pixels[y * (int)size.x + x] = SDL_MapRGBA(SDL_GetPixelFormatDetails(pSurface->format), nullptr, r, g, b, a);
		}
	}

	SDL_UnlockSurface(pSurface);

	const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(pSurface->format);
	if (!fmt)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Light2D] ERROR: Failed to get pixel format; %s", SDL_GetError());
		return;
	}

	this->pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
	SDL_DestroySurface(pSurface);

	if (!pTexture)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[Light2D] ERROR: Texture creation failed; %s", SDL_GetError());
		return;
	}

	SDL_SetTextureBlendMode(pTexture, SDL_BLENDMODE_ADD);
}

SDL_FRect Light2D::getDstRect() const
{
	AGameObject* owner = this->getOwner();
	Camera* pCam = CameraManager::getInstance()->getCurrentCamera();

	Vector2D scale = owner->getScale();
	Vector2D screenSize = this->size * scale;
	Vector2D pos = owner->getPos();
	pos -= screenSize * Vector2D(0.5f);

	SDL_FRect dstRect = {};
	dstRect.x = pos.x;
	dstRect.y = pos.y;
	dstRect.w = screenSize.x;
	dstRect.h = screenSize.y;

	return pCam->worldToScreenRect(dstRect);
}

float Light2D::getRadius() const
{
	return this->radius;
}

void Light2D::setRadius(float radius)
{
	this->radius = radius;
	createRadialLightTexture(radius);
}

Vector2D Light2D::getSize() const
{
	return this->size;
}

void Light2D::setSize(Vector2D size)
{
	this->size = size;
	createRectLightTexture(size);
}
