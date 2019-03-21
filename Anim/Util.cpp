#include "stdafx.h"
#include "Util.h"

float Anim::to_radius(float x)
{
	constexpr float pi = 3.141592653f;
	return (x / 180) * pi;
}
