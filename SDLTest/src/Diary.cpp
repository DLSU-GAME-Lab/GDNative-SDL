#include "Diary.h"
#include "Background.h"
#include "GameObjectManager.h"
Diary::Diary(const std::string& strBackroundIMG, Vector2D fVecTranslate, Vector2D fVecScale):AGameObject("Diary")
{
	this->strBackroundIMG = strBackroundIMG;
}

Diary::~Diary()
{
}

void Diary::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strBackroundIMG, this->fVecTranslate.x, this->fVecTranslate.y);
	this->attachComponent((AComponent*)pSpriteRenderer);
}

int Diary::getCurrentPage()
{
	return this->dCurrentPage;
}

void Diary::setCurrentPage(int dCurrentPage)
{
	this->dCurrentPage = dCurrentPage;
}

void Diary::otherPagesFollow()
{
	for (int i =1; i < this->vecPages.size(); i++)
	{
		this->vecPages[i]->setFollowParent(true);
	}
}

void Diary::otherPagesStopFollow()
{
	for (int i = 1; i < this->vecPages.size(); i++)
	{
		this->vecPages[i]->setFollowParent(false);
	}
}

void Diary::addPage(EmptyObject* pPage)
{
	this->vecPages.push_back(pPage);
	this->attachChild(pPage);
}


