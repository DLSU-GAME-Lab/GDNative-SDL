#pragma once
#include "vector"
#include "unordered_map"
#include "EventListener.h"
#include "EventBroadcaster.h"
#include "Tracker.h"
#include "EnumEventKey.h"
#include "AComponent.h"
#include "EmptyObject.h"
class TrackerManager:public AComponent,EventListener 
{
private:
    std::vector<Tracker*> vecTracker;
    std::unordered_map < std::string, Tracker*> mapTracker;
    EventKey EKey;
    bool bVictory;
    bool bTrackerEnabled;
    bool bListenerEnabled;
public:
    void perform();
    void registerTracker(Tracker* pTracker);
    void unRegisterTracker(Tracker* pTracker);
    void unRegisterAllTracker();
    int findTracker(Tracker* pTracker);
    void disableTracker();

public:
    bool CheckVictory();
    // Inherited via EventListener
    void onEventTrigger(std::unordered_map<std::string, void*> mapParameter) override;
    EventKey getKey() override;
    virtual bool isListenerEnabled() override;
    virtual void setListenerEnabled(bool bListenerEnabled) override;
    virtual std::string getListenerOwnerName() override;
    /* * * * * * * * * * * * * * * * * * * * *
 *       SINGLETON-RELATED CONTENT       *
 * * * * * * * * * * * * * * * * * * * * */
private:
    static TrackerManager* P_SHARED_INSTANCE;

private:
    TrackerManager(std::string strName, AGameObject* pOwner);
    TrackerManager(const TrackerManager&);
    TrackerManager& operator = (const TrackerManager&);

public:
    static TrackerManager* getInstance();
    static void initialize(std::string strName, AGameObject* pOwner);
    static void destroy();
    /* * * * * * * * * * * * * * * * * * * * */
};

