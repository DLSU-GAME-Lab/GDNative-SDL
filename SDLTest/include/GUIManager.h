#pragma once
#include "AComponent.h"
#include "AGameObject.h"
#include <unordered_map>

class GUIManager : public AComponent
{
private:
    std::unordered_map<std::string, AGameObject*> vecGUI;

public:
    void onAttach() override;
    void perform() override;
    void registerGUI(AGameObject* pGameObject);
    void unregisterGUI(std::string strName);

    AGameObject* getGUI(std::string strName);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static GUIManager* P_SHARED_INSTANCE;

private:
    GUIManager() : AComponent("GUI Manager", ComponentType::INPUT) {};
    GUIManager(const GUIManager&) : AComponent("GUI Manager", ComponentType::INPUT) {};
    GUIManager& operator = (const GUIManager&) {};

public:
    static void initialize();
    static void destroy();

    static GUIManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

