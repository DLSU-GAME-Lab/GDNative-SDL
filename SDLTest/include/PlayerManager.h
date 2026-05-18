#pragma once
#include "AComponent.h"
#include "Player.h"
#include "PlayerController.h"

class PlayerManager : public AComponent
{
private:
	Player* pPlayer;
    PlayerController* pController;

    float fJumpForce = 100.0f;
    float fMoveSpeed = 100.0f;

public:
    void perform() override;
	void setPlayer(Player* pPlayer);
    PlayerController* getPlayerController() const;

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

