#include "stdafx.h"
#include "Util.h"

float Anim::to_radius(float x)
{
	constexpr static float pi = 3.141593f;
	return (x / 180) * pi;
}
