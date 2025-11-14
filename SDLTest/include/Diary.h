#pragma once
#include "AGameObject.h"
#include "EmptyObject.h"
#include "FontManager.h"
#include "EventListener.h"
#include "EventBroadcaster.h"
#include "Settings.h"
#include "string"
#include "vector"
#include "Text.h"
class Diary :public AGameObject, EventListener
{
private:
	std::vector<EmptyObject*> vecPages;
	std::string strBackroundIMG;
	int dCurrentPage;
	EventKey EKey;
	bool bListenerEnabled;
	std::vector <std::string> vecPageTitles;
	std::vector <std::string> vecStartTexts;
	Text* pTitleText;
	Text* pStartingText;
public:
	Diary(const std::string& strBackroundIMG, Vector2D fVecTranslate, Vector2D fVecScale);
public:
	void initialize() override;
public:
	int getCurrentPage();
	void setCurrentPage(int dCurrentPage);
	void addPageText(std::string strTitle, std::string strStartText);
	void addPage(EmptyObject* pPage);
	void progressPage();
	void regressPage();
	void resetPages();

	// Inherited via EventListener
	void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
	EventKey getKey() override;

	// Inherited via EventListener
	bool isListenerEnabled() override;
	void setListenerEnabled(bool bListenerEnabled) override;
	std::string getListenerOwnerName() override;
};

