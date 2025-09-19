#pragma once
#include "AComponent.h"
#include <vector>

class GemManager : public AComponent
{
private:
    std::vector<AGameObject*> vecGem;

public:
    void perform() override;
    void startLevel();
    void moveGems();
    void matchGems();

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static GemManager* P_SHARED_INSTANCE;

private:
    GemManager() : AComponent("GUI Manager", ComponentType::SCRIPT) {};
    GemManager(const GemManager&) : AComponent("GUI Manager", ComponentType::SCRIPT) {};
    GemManager& operator = (const GemManager&) {};

public:
    static void initialize();
    static void destroy();

    static GemManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

