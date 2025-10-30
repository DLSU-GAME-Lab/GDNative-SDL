#include "Diary.h"
#include "Background.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"

Diary::Diary(const std::string& strBackroundIMG, Vector2D fVecTranslate, Vector2D fVecScale):AGameObject("Diary")
{
	this->strBackroundIMG = strBackroundIMG;
	this->dCurrentPage = 0;
	this->EKey = EventKey::DIARY_SCREEN;
	this->pStartingText = NULL;
	this->pTitleText = NULL;
	this->bListenerEnabled = true;
	this->vecPages = {};
}

void Diary::initialize()
{
	
	SpriteRenderer* pSpriteRenderer = new SpriteRenderer(this->strBackroundIMG);
	this->attachComponent(pSpriteRenderer);
	EventBroadcaster::getInstance()->registerListener(this);

	this->pTitleText = new Text("Title_Text", "JainiPurva-Regular.ttf", 90, 0, false);
	this->pTitleText->setMessage(this->vecPageTitles[0]);
	this->pTitleText->setPos(Vector2D(0, 400));
	this->pTitleText->setScale(Vector2D(1, 1));
	this->attachChild(this->pTitleText);

	this->pStartingText = new Text("Title_Text", "JainiPurva-Regular.ttf", 45, 0, false);
	this->pStartingText->setMessage(this->vecStartTexts[0]);
	this->pStartingText->setPos(Vector2D(-60, 250));
	this->pStartingText->setScale(Vector2D(1, 1));
	this->attachChild(this->pStartingText);
}


int Diary::getCurrentPage()
{
	return this->dCurrentPage;
}

void Diary::setCurrentPage(int dCurrentPage)
{
	this->dCurrentPage = dCurrentPage;
}

void Diary::addPageText(std::string strTitle, std::string strStartText)
{
	this->vecPageTitles.push_back(strTitle);
	this->vecStartTexts.push_back(strStartText);
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
		//this->vecPages[dCurrentPage]->setEnabled(false);
		this->dCurrentPage++;
		this->pTitleText->modifyText(this->vecPageTitles[this->dCurrentPage]);
		this->pStartingText->modifyText(this->vecStartTexts[this->dCurrentPage]);
		//this->vecPages[dCurrentPage]->setEnabled(true);
	}
	else
	{
		//this->vecPages[dCurrentPage]->setEnabled(false);
		//this->vecPages[0]->setEnabled(true);
		this->dCurrentPage = 0;
		this->pTitleText->modifyText(this->vecPageTitles[0]);
		this->pStartingText->modifyText(this->vecStartTexts[0]);
	}
}

void Diary::regressPage()
{
	if (this->dCurrentPage  > 0)
	{
		//this->vecPages[dCurrentPage]->setEnabled(false);
		this->dCurrentPage--;
		this->pTitleText->modifyText(this->vecPageTitles[this->dCurrentPage]);
		this->pStartingText->modifyText(this->vecStartTexts[this->dCurrentPage]);
		//this->vecPages[dCurrentPage]->setEnabled(true);

	}
	else 
	{
		//this->vecPages[dCurrentPage]->setEnabled(false);
		//this->vecPages[this->vecPages.size() - 1]->setEnabled(true);
		this->dCurrentPage = this->vecPages.size()-1;
		this->pTitleText->modifyText(this->vecPageTitles[this->dCurrentPage]);
		this->pStartingText->modifyText(this->vecStartTexts[this->dCurrentPage]);
	}
}

void Diary::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{

	bool bFromToggle = false;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "Close_Button");

	}
	if (!this->bEnabled)
	{
		this->setEnabled(true);
		this->pStartingText->modifyText(this->vecStartTexts[this->dCurrentPage]);
		this->pTitleText->modifyText(this->vecPageTitles[this->dCurrentPage]);
		EventBroadcaster::getInstance()->disableOtherListenerExcept(this);
	}
	else if (this->bEnabled && bFromToggle)
	{
		this->setEnabled(false);
		this->dCurrentPage = 0;
		EventBroadcaster::getInstance()->enableAllListeners();

	}

}

EventKey Diary::getKey()
{
	return this->EKey;
}

bool Diary::isListenerEnabled()
{
	return this->bListenerEnabled;
}

void Diary::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string Diary::getListenerOwnerName()
{
	return this->strName;
}


