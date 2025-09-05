#include "Background.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"

Background::Background(std::string, std::string strImageName) : AGameObject(strName)
{
	this->strImageName = strImageName;
	//SpriteRenderer* pSpriteRenderer = new SpriteRenderer(strImageName, new SDL);
}

Background::~Background()
{

}
