#include "MathUtils.h"

float MathUtils::toDegrees(float radians)
{
	return radians * (180 / 3.14159);
}

float MathUtils::toRadians(float degrees)
{
	return degrees * (3.14159 / 180);
}
