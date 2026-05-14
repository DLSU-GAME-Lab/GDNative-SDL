#pragma once
#pragma once
#include "AComponent.h"

class ShimmerEffect : public AComponent
{
private:
    float fPhase;
    float fSpeed;
    float fCooldown;
    float fCooldownTimer;
    bool  bShimmering;
public:
    ShimmerEffect(float fSpeed, float cooldown, float fStartOffset);
       

    void perform() override;

    float getPhase() const; 
    bool  isShimmering() const;
};