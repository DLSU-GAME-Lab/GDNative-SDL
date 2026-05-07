#pragma once
#include "AComponent.h"
#include "Player.h"

class PlayerManager : public AComponent
{
private:
	Player* pPlayer;

public:
    virtual void perform() override {};
	void setPlayer(Player* pPlayer);

    /* * * * * * * * * * * * * * * * * * * * *
     *       SINGLETON-RELATED CONTENT       *
     * * * * * * * * * * * * * * * * * * * * */
private:
    static PlayerManager* P_SHARED_INSTANCE;

private:
    PlayerManager() : AComponent("Player Manager", ComponentType::SCRIPT) {};
    PlayerManager(const PlayerManager&) : AComponent("Player Manager", ComponentType::SCRIPT) {};
    PlayerManager& operator = (const PlayerManager&) {};

public:
    static void initialize();
    static void destroy();

    static PlayerManager* getInstance();
    /* * * * * * * * * * * * * * * * * * * * */
};

