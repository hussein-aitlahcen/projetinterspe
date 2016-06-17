#include "animation.h"
#include <math.h>

void Animation::update(float dt)
{
	currentAngle = fmod(currentAngle + angle * dt, 360);
}
