#include "ShimmerEffect.h"



ShimmerEffect::ShimmerEffect(float fSpeed, float cooldown, float fStartOffset) : AComponent("ShimmerEffect", ComponentType::SCRIPT)
{
    this->fSpeed = fSpeed;
    this->fCooldown = cooldown;
    this->fPhase = fStartOffset;
    this->fCooldownTimer = fStartOffset;
    this->bShimmering = false;
}

void ShimmerEffect::perform()
{

    float dt = this->getDeltaTime();

    if (!bShimmering) {
        this->fCooldownTimer += dt;
        if (fCooldownTimer >= fCooldown) {
            fCooldownTimer = 0.0f;
            bShimmering = true;
            fPhase = 0.0f;
        }
    }
    else {
        fPhase += fSpeed * dt;
        if (fPhase >= 1.0f) {
            fPhase = 0.0f;
            bShimmering = false;
        }
    }

}

float ShimmerEffect::getPhase() const
{
    return this->fPhase;
}
bool ShimmerEffect::isShimmering() const
{
    return bShimmering;
}