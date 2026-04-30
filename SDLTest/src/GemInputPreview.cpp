#include "GemInputPreview.h"
#include "GUIButton.h"
#include "Sprite.h"
#include "Text.h"
#include "GameObjectManager.h"
#include "EventBroadcaster.h"
#include "GUIToggle.h"
#include "DataAssetManager.h"
#include "GUIUtils.h"

GemInputPreview::GemInputPreview() : AGameObject("GemInputPreview")
{
    this->bIsScreenObject = true;
    this->EKey = EventKey::GEM_INPUT_PREVIEW;
	this->bListenerEnabled = true;
	this->pDataAsset = (CollectableGemDataAsset*)DataAssetManager::getInstance()->getDataAsset("CollectableGemDataAsset");
}

void GemInputPreview::setPreviewText(std::string strGemName)
{
	Text* pPreviewText = (Text*)this->findChildByName("GemInput_PreviewText");
	pPreviewText->setMessage(this->pDataAsset->getGemTextBreak(strGemName));
}

void GemInputPreview::initialize()
{
	Sprite* pPreviewBG = new Sprite("GemInput_PreviewWindow", "Square");
	pPreviewBG->setIsScreenObject(true);
	this->attachChild(pPreviewBG);
	pPreviewBG->setColor({ 255, 225, 170, 255 });
	pPreviewBG->setPos(Vector2D(510, 590));

	Text* pPreviewText = new Text("GemInput_PreviewText", "JainiPurva-Regular.ttf", 60, 0.f, false);
	pPreviewText->setIsScreenObject(true);
	this->attachChild(pPreviewText);
	pPreviewText->setMessage("Preview \n Text");
	pPreviewText->setPos(Vector2D(510, 540));

	GUIButton* pClosePreview = new GUIButton("GemInput_ClosePreview", "Close_Button");
	GUIToggle* pCloseToggle = new GUIToggle(EventKey::GEM_INPUT_PREVIEW);
	pClosePreview->attachComponent(pCloseToggle);
	pClosePreview->setIsScreenObject(true);
	this->attachChild(pClosePreview);
	pClosePreview->setPos(Vector2D(510, 790));
	pClosePreview->setScale(Vector2D(0.1f));


	Text* pClosePreviewText = new Text("GemInput_ClosePreviewText", "JainiPurva-Regular.ttf", 50, 0.f, false);
	pClosePreviewText->setIsScreenObject(true);
	this->attachChild(pClosePreviewText);
	pClosePreviewText->setMessage("Close");
	pClosePreviewText->setColor({ 180, 20, 20, 255 });
	pClosePreviewText->setPos(Vector2D(510, 785));

	EventBroadcaster::getInstance()->registerListener(this);
	this->setEnabled(false);

}

void GemInputPreview::onEventTrigger(std::unordered_map<std::string, void*> mapParameter)
{
	bool bFromToggle = false;
	std::cout << "Enter" << std::endl;
	if (mapParameter.find("Sender") != mapParameter.end())
	{
		std::string senderType = *static_cast<std::string*>(mapParameter["Sender"]);
		bFromToggle = (senderType == "GemInput_ClosePreview");
		std::cout << senderType << std::endl;
	}
	// if object not enabled enable it based on name
	if (!this->bEnabled)
	{
		std::string strGemName = *static_cast<std::string*>(mapParameter["GemName"]);
		this->setEnabled(true);
		this->setPreviewText(strGemName);
	}
	//do not change this handles the closing if separated any object that throws the event will close it
	else if (this->bEnabled && bFromToggle)
	{
		this->setEnabled(false);
		EventBroadcaster::getInstance()->enableAllListeners();
	}
}

EventKey GemInputPreview::getKey()
{
	return this->EKey;
}

bool GemInputPreview::isListenerEnabled()
{
    return this->bListenerEnabled;
}

void GemInputPreview::setListenerEnabled(bool bListenerEnabled)
{
	this->bListenerEnabled = bListenerEnabled;
}

std::string GemInputPreview::getListenerOwnerName()
{
	return this->strName;
}
