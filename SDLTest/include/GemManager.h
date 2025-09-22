#pragma once
#include "AComponent.h"
#include "Vector2D.h"
#include <vector>

class GemManager : public AComponent
{
public:
    struct CellData
    {
        Uint8 c;
        Uint8 r;
        Vector2D pos;
        AGameObject* obj;
    };
private:
    int selected[2] = { -1, -1 };
    std::vector<AGameObject*> vecGem;
    std::vector<Vector2D> vecTile;
    float fCellSize;

public:
    void perform() override;
    void startLevel();

    AGameObject* getGem();
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

