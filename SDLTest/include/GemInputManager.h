#pragma once
#include "vector"
#include "AComponent.h"
#include "Vector2D.h"
#include "GemInputToken.h"
#include "Text.h"
#include "Sprite.h"
#include "ButtonInput.h"
#include "CollectableGemDataAsset.h"
#include "Gate.h"

class GemInputManager : public AComponent
{
private:
    std::vector<GemInputToken*> vecToken;
    std::vector<Vector2D> vecPosition;
    std::vector<Sprite*> vecTextBox;
    std::vector<Text*> vecText;
    std::vector<std::string> vecTextPlaceholder;
	std::vector<ButtonInput*> vecUndo;
	ButtonInput* pSubmit;
	std::vector<int> vecTextAssigned;
	std::vector<int> vecWinCon;
	CollectableGemDataAsset* pDataAsset;
    Gate* pGate;

public:
    void perform() override;

    void addToken(GemInputToken* pToken);
    void addTextBox(Sprite* pSprite);
    void addText(Text* pText);
	void addUndoButton(ButtonInput* pButton);
	void addSubmitButton(ButtonInput* pButton);
	void addWinCon(int nWincon);
    void addGate(Gate* pGate);

    void checkCollection();
    void assignText(int nTextIndex, int nTokenIndex);
    void resetText(int nTextIndex);
    void resetAllText();

    bool checkWinCon();

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static GemInputManager* P_SHARED_INSTANCE;

private:
    GemInputManager();
    GemInputManager(const GemInputManager&);
    GemInputManager& operator = (const GemInputManager&);

public:
    static GemInputManager* getInstance();
    static void initialize();
    static void destroy();
    /* * * * * * * * * * * * * * * * * * * * */
};

