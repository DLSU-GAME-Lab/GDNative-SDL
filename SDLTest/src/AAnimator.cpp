#include "AAnimator.h"

AAnimator::AAnimator(std::string strName) : AComponent(strName, ComponentType::ANIMATOR) {};

void AAnimator::addListener(IAnimatorListener* pListener)
{
	this->vecListener.push_back(pListener);
}

void AAnimator::removeListener(IAnimatorListener* pListener)
{
	int nIndex = -1;
	for (int i = 0; i < this->vecListener.size() && nIndex == -1; i++)
	{
		if (this->vecListener[i] == pListener) nIndex = i;
	}

	if (nIndex != -1) this->vecListener.erase(this->vecListener.begin() + nIndex);
}
