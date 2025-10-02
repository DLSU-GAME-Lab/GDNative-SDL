#include "SpriteAnimator.h"

#ifdef __ANDROID__
#include "spdlog/spdlog.h"
#include "spdlog/sinks/android_sink.h"
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
#else
// on windows/linux: just no-op or printf
#include <cstdio>
#define LOG_DEBUG(...) printf(__VA_ARGS__); printf("\n")
#define LOG_ERROR(...) printf(__VA_ARGS__); printf("\n")
#endif

SpriteAnimator::SpriteAnimator(SpriteRenderer* pSpriteRenderer, std::vector<SDL_Texture*> vecTexture, unsigned int nFrameRate)
	: AComponent("SpriteAnimator", ComponentType::SCRIPT)
{
	this->pSpriteRenderer = pSpriteRenderer;
	this->vecTexture = vecTexture;
	this->nFrameRate = nFrameRate;

	this->bIsPlaying = false;
	this->bIsReverse = false;
	this->EType = AnimationType::ONCE;
	this->nFrameIndex = 0;
	this->fTicks = 0;
	this->fTicksPerFrame = nFrameRate / 60.0f;
}

SpriteAnimator::~SpriteAnimator()
{

}

void SpriteAnimator::perform()
{
    LOG_DEBUG("SpriteAnimator::perform() called, bIsPlaying={}", this->bIsPlaying);

    if (this->vecTexture.empty()) {
        LOG_ERROR("SpriteAnimator::perform() - no textures, animator stopped");
        this->bIsPlaying = false;
        return;
    }

    if (this->bIsPlaying)
    {
        this->fTicks += fDeltaTime;
        if (this->fTicks >= this->fTicksPerFrame)
        {
            this->fTicks -= this->fTicksPerFrame;

            if (!this->bIsReverse) this->nFrameIndex++;
            else this->nFrameIndex--;

            switch (this->EType)
            {
            case AnimationType::ONCE:
                if (this->nFrameIndex >= (int)this->vecTexture.size()) {
                    LOG_DEBUG("SpriteAnimator: out of range index={} size={}", this->nFrameIndex, this->vecTexture.size());
                    this->stop();
                    return;
                }
                break;

            case AnimationType::LOOP:
                if (!this->vecTexture.empty())
                    this->nFrameIndex %= (int)this->vecTexture.size();
                break;

            case AnimationType::PINGPONG:
                if (!this->vecTexture.empty() &&
                    (this->nFrameIndex == (int)this->vecTexture.size() - 1 || this->nFrameIndex == 0))
                {
                    this->bIsReverse = !this->bIsReverse;
                }
                break;

            default:
                break;
            }

            LOG_DEBUG("SpriteAnimator: setting frame index={} (size={})", this->nFrameIndex, this->vecTexture.size());
            this->pSpriteRenderer->setTexture(this->vecTexture[this->nFrameIndex]);
        }
    }
}

void SpriteAnimator::stop()
{
	this->bIsPlaying = false;
	this->nFrameIndex = 0;
	this->fTicks = 0;
}

void SpriteAnimator::play()
{
	if (this->bIsPlaying) this->stop();
	this->bIsPlaying = true;
    LOG_DEBUG("SpriteAnimator::play() called, animation started");
}

void SpriteAnimator::setAnimationType(AnimationType EType)
{
	this->EType = EType;
}

AnimationType SpriteAnimator::getAnimationType() const
{
	return this->EType;
}
