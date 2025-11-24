#include "TweenAnimator.h"
#include "AGameObject.h"

TweenAnimator::TweenAnimator() : AAnimator("TweenAnimator")
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
		if (!this->tweenPos.isFinished())
		{
			std::array<float, 2> pos = this->tweenPos.step(ms);
			this->pOwner->setPos(Vector2D(pos[0], pos[1]));
		}

		if (this->tweenPos.isFinished())
		{
			switch (this->EType)
			{
			case AnimationType::ONCE:
				this->stop();
				for (auto pListener : this->vecListener)
				{
					pListener->onAnimationFinished();
				}
				break;

			case AnimationType::LOOP:
				this->tweenPos.seek(0.0f);
				break;

			case AnimationType::YOYO:
				if (this->bIsReverse)
				{
					this->bIsReverse = false;
					this->tweenPos.forward();
					std::array<float, 2> pos = this->tweenPos.seek(0.0f);
					this->pOwner->setPos(Vector2D(pos[0], pos[1]));
				}
				else
				{
					this->bIsReverse = true;
					this->tweenPos.backward();
					std::array<float, 2> pos = this->tweenPos.seek(0.99f);
					this->pOwner->setPos(Vector2D(pos[0], pos[1]));
				}
				break;

			case AnimationType::YOYO_ONCE:
				if (this->bIsReverse)
				{
					this->bIsPlaying = false;
					for (auto pListener : this->vecListener)
					{
						pListener->onAnimationFinished();
					}
				}
				else
				{
					this->bIsReverse = true;
					this->tweenPos.backward();
					std::array<float, 2> pos = this->tweenPos.seek(0.99f);
					this->pOwner->setPos(Vector2D(pos[0], pos[1]));
				}
				break;

			}
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

	//this->tweenScale.seek(0);
	//this->tweenRot.seek(0);
}

void TweenAnimator::setAnimationType(AnimationType EType)
{
	this->EType = EType;

	//switch (this->EType)
	//{
	//case AnimationType::LOOP:
	//	this->tweenPos.onStep(loop<float,float>());
	//	break;

	//case AnimationType::YOYO:
	//	this->tweenPos.onStep(yoyo<float, float>());
	//	break;

	//case AnimationType::YOYO_ONCE:
	//	this->tweenPos.onStep(yoyoOnce<float, float>());
	//	break;

	//}

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

template<typename... Ts>
bool TweenAnimator::loop<Ts...>::operator()(tweeny::tween<Ts...>& t, Ts...)
{
	if (t.progress() < 1.0f) return false;
	t.seek(0);
	return false;
}

template<typename... Ts>
bool TweenAnimator::yoyo<Ts...>::operator()(tweeny::tween<Ts...>& t, Ts...)
{
	if (t.progress() <= 0.001f) { t.forward(); }
	if (t.progress() >= 1.0f) { t.backward(); }
	return false;
}
template<typename... Ts>
bool TweenAnimator::yoyoOnce<Ts...>::operator()(tweeny::tween<Ts...>& t, Ts...)
{
	if (t.progress() <= 0.001f) { return true; }
	if (t.progress() >= 1.0f) { t.backward(); }
	return false;
}
