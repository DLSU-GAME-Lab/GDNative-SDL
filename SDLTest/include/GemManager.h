#pragma once
#include "AComponent.h"
#include "Grid.h"
#include <vector>

class GemManager : public Grid
{
private:
    int selected[2] = { -1, -1 };
    std::vector<AGameObject*> vecGem;
    std::vector<Vector2D> vecTile;

public:
    void perform() override;

    static void loadResources();
    static void unloadResources();

    void startLevel1();
    void startLevel2();
    void startLevel3();

    AGameObject* getGem();
    void moveGems();
    void matchGems();

private:
    void createGems(float fScale);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static GemManager* P_SHARED_INSTANCE;

private:
    GemManager(Uint8 w, Uint8 h, float fCellSize);
    GemManager(const GemManager&);
    GemManager& operator = (const GemManager&) {};

public:
    static void initialize(Uint8 w, Uint8 h, float fCellSize, Vector2D offset);
    static void destroy();

    static GemManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */

};

