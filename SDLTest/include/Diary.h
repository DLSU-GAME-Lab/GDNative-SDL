#pragma once
#include "AGameObject.h"
#include "EmptyObject.h"
#include "FontManager.h"
#include "Settings.h"
#include "string"
#include "vector"
class Diary:public AGameObject
{
private:
	std::vector<EmptyObject*> vecPages;
	std::string strBackroundIMG;
	int dCurrentPage;
public:
	Diary(const std::string& strBackroundIMG, Vector2D fVecTranslate, Vector2D fVecScale);
	~Diary();
public:
	void initialize() override;
public:
	int getCurrentPage();
	void setCurrentPage(int dCurrentPage);
	void otherPagesFollow();
	void otherPagesStopFollow();
	void addPage(EmptyObject* pPage);
};

