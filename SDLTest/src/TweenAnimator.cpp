#include "TweenAnimator.h"
#include "AGameObject.h"

TweenAnimator::TweenAnimator() : AComponent("TweenAnimator", ComponentType::SCRIPT)
{
	this->EType = AnimationType::ONCE;
	this->bIsPlaying = false;
	this->bIsReverse = false;
}

TweenAnimator::~TweenAnimator()
{

}

void TweenAnimator::perform()
{
	if (this->bIsPlaying)
	{
		int ms = fDeltaTime * 1000;
		if (this->tweenPos.isFinished())
		{
			switch (this->EType)
			{
			case AnimationType::ONCE:
				this->stop();
				break;

			case AnimationType::LOOP:
				this->tweenPos.seek(0.0f);
				break;

			case AnimationType::PINGPONG:
				if (this->bIsReverse)
				{
					this->bIsReverse = false;
					this->tweenPos.forward();
					this->tweenPos.seek(0.0f);
				}
				else
				{
					this->bIsReverse = true;
					this->tweenPos.backward();
					this->tweenPos.seek(0.98f);
				}
				break;
			}
		}
		else
		{
			std::array<float, 2> pos = this->tweenPos.step(ms);
			this->pOwner->setPos(Vector2D(pos[0], pos[1]));
		}


		//if (!this->tweenScale.isFinished())
		//{
		//	std::array<float, 2> scale = this->tweenScale.step(ms);
		//	this->pOwner->setScale(Vector2D(scale[0], scale[1]));
		//}

		//if (!this->tweenRot.isFinished())
		//{
		//	float rot = this->tweenRot.step(ms);
		//	this->pOwner->setRot(rot);
		//}

		//if (this->tweenPos.isFinished() &&
		//	this->tweenScale.isFinished() &&
		//	this->tweenRot.isFinished())
		//{
		//	switch (this->EType)
		//	{
		//	case AnimationType::ONCE:
		//		this->stop();
		//		break;

		//	case AnimationType::LOOP:
		//		this->tweenPos.jump(0);
		//		this->tweenScale.jump(0);
		//		this->tweenRot.jump(0);
		//		break;

		//	case AnimationType::PINGPONG:
		//		if (this->bIsReverse)
		//		{
		//			this->bIsReverse = false;
		//			this->tweenPos.forward();
		//			this->tweenScale.forward();
		//			this->tweenRot.forward();
		//		}
		//		else
		//		{
		//			this->bIsReverse = false;
		//			this->tweenPos.backward();
		//			this->tweenScale.backward();
		//			this->tweenRot.backward();
		//		}
		//		break;

		//	default:
		//		break;
		//	}
		//}
	}
}

void TweenAnimator::play()
{
	this->bIsPlaying = true;
}

void TweenAnimator::pause()
{
	this->bIsPlaying = false;
}

void TweenAnimator::stop()
{
	this->bIsPlaying = false;
	this->tweenPos.seek(0);
	this->tweenScale.seek(0);
	this->tweenRot.seek(0);
}

void TweenAnimator::setAnimationType(AnimationType EType)
{
	this->EType = EType;
}

void TweenAnimator::setTweenPos(const Tween2D& tweenPos)
{
	this->tweenPos = tweenPos;
}

void TweenAnimator::setTweenScale(const Tween2D& tweenScale)
{
	this->tweenScale = tweenScale;
}

void TweenAnimator::setTweenRot(const Tween & tweenRot)
{
	this->tweenRot = tweenRot;
}
