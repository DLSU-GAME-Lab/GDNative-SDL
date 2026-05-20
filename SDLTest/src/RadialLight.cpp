#include "RadialLight.h"
#include "Light2D.h"
#include "Lightmap.h"

RadialLight::RadialLight(const std::string& strName, SDL_Color color) : AGameObject(strName), color(color)
{

}

RadialLight::~RadialLight()
{
	AGameObject::~AGameObject();
	Lightmap::getInstance()->removeLight((Light2D*)this->findComponentByName("Light2D"));
}

void RadialLight::initialize()
{
	Light2D* pLight = new Light2D("radial_light");
	pLight->setColor(color);
	this->attachComponent(pLight);
	Lightmap::getInstance()->addLight(pLight);
	SDL_Log("Radial light was created.");
}

SDL_Color RadialLight::getColor()
{
	return ((Light2D*)findComponentByName("radial_light"))->getColor();
}

void RadialLight::setColor(SDL_Color color)
{
	((Light2D*)findComponentByName("radial_light"))->setColor(color);
}
