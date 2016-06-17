#include "animation.h"
#include <math.h>

void Animation::update()
{
	currentAngle = fmod(currentAngle + angle, 360);
}
