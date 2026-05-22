#include "RadialLight.h"
#include "Light2D.h"
#include "Lightmap.h"

RadialLight::RadialLight(const std::string& strName, float radius, SDL_Color color) : AGameObject(strName), radius(radius), color(color)
{

}

RadialLight::~RadialLight()
{
	Lightmap::getInstance()->removeLight((Light2D*)this->findComponentByName("Light2D"));
}

void RadialLight::initialize()
{
	Light2D* pLight = new Light2D(Light2D::FalloffType::QUADRATIC, radius);
	pLight->setColor(color);
	this->attachComponent(pLight);
	Lightmap::getInstance()->addLight(pLight);
	SDL_Log("Radial light was created.");
}

SDL_Color RadialLight::getColor()
{
	return this->pLight->getColor();
}

void RadialLight::setColor(SDL_Color color)
{
	this->pLight->setColor(color);
}

Light2D::FalloffType RadialLight::getFalloffType()
{
	return Light2D::FalloffType();
}

void RadialLight::setFalloffType(Light2D::FalloffType falloffType)
{}
