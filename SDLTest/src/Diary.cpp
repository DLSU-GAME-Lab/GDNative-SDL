#include "Diary.h"
#include "Background.h"
#include "GameObjectManager.h"

Diary::Diary(const std::string& strBackroundIMG, Vector2D fVecTranslate, Vector2D fVecScale):AGameObject("Diary")
{
	this->strBackroundIMG = strBackroundIMG;
	this->dCurrentPage = 0;
}

void Diary::initialize()
{
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strBackroundIMG);
	this->attachComponent(pSpriteRenderer);

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

void Diary::progressPage()
{
	if (this->dCurrentPage < this->vecPages.size() - 1)
	{
		this->vecPages[dCurrentPage]->setEnabled(false);
		this->dCurrentPage++;
		this->vecPages[dCurrentPage]->setEnabled(true);
	}
	else
	{
		this->vecPages[dCurrentPage]->setEnabled(false);
		this->vecPages[0]->setEnabled(true);
		this->dCurrentPage = 0;
	}
}

void Diary::regressPage()
{
	if (this->dCurrentPage  > 0)
	{
		this->vecPages[dCurrentPage]->setEnabled(false);
		this->dCurrentPage--;
		this->vecPages[dCurrentPage]->setEnabled(true);

	}
	else if(this->dCurrentPage <= 0)
	{
		this->vecPages[dCurrentPage]->setEnabled(false);
		this->vecPages[this->vecPages.size() - 1]->setEnabled(true);
		this->dCurrentPage = this->vecPages.size()-1;
	}
}


