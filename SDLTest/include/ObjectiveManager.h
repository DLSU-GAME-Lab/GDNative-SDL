#pragma once

#include "AComponent.h"
#include "Vector2D.h"
#include "EventBroadcaster.h"   
#include "EnumEventKey.h"       
#include <vector>
#include <unordered_map>
#include <string>

class AGameObject;
class SpriteRenderer;
class Gem;
class ButtonInput;

class ObjectiveManager : public AComponent, public EventListener
{
private:
    // scene objects (looked up at initialize)
    AGameObject* pPlayer = nullptr;
    AGameObject* pArrow = nullptr;
    AGameObject* pObjectivePanel = nullptr;
    AGameObject* pOptionalPanel = nullptr;

    // GUI buttons from PlatformerLevel1Scene (common names)
    AGameObject* pItemsButton = nullptr;   // triggers findGems()
    AGameObject* pStoryButton = nullptr;   // triggers findDoor()
    AGameObject* pPauseButton = nullptr;
    AGameObject* pObjectiveButton = nullptr;

    // cached renderers (optional visual flips)
    SpriteRenderer* pItemsButtonRenderer = nullptr;
    SpriteRenderer* pStoryButtonRenderer = nullptr;

    // pathfinding / tween state
    bool isPathFinding = false;
    Vector2D startPos = Vector2D(0, 0);
    Vector2D targetPos = Vector2D(0, 0);
    float duration = 1.0f;      // seconds to move arrow
    float elapsed = 0.0f;

    // helper config
    constexpr static float FINISH_EPS = 1.5f; // pixels threshold to finish

    // Event/listener support
    bool bListenerEnabled = true;
    EventKey EKey = EventKey::OBJECTIVE_BUTTON; // must exist in EnumEventKey.h

public:
    ObjectiveManager();
    ~ObjectiveManager();

    // component lifecycle
    void initialize();
    void perform() override;

    // EventListener API — MUST match exact signatures from EventListener
    void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
    EventKey getKey() override;
    bool isListenerEnabled() override;
    void setListenerEnabled(bool b) override;
    std::string getListenerOwnerName() override;

    // public API
    void findDoor();
    void findGems();
    void toggleObjectivePanel();
    void toggleOptionalPanel();

private:
    // internal helpers
    void startArrowPathfinding(const Vector2D& target);
    void stopArrowPathfinding();
    std::vector<Vector2D> collectActiveGems();
    AGameObject* findDoorObject();
    Vector2D getClosestGemToPlayer(const std::vector<Vector2D>& gems);
    float distance(const Vector2D& a, const Vector2D& b) const;
    Vector2D moveTowards(const Vector2D& from, const Vector2D& to, float maxDist) const;
    SpriteRenderer* findSpriteRenderer(AGameObject* pObj);
    ButtonInput* findButtonInput(AGameObject* pObj);
};