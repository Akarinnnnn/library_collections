#include "stdafx.h"
#include <algorithm>
#include "Martix22.h"
using namespace Anim;
using namespace std;
Anim::Martix22::Martix22()
{
}
Anim::Martix22::Martix22(float d[2][2])
{
	for (int i = 0; i < 4; i++)
	{
		*(float*)(d + i) = *(float*)(data + i);
	}
}

Martix22 Anim::operator+(const Martix22 & l, const Martix22 & r)
{
	float d[2][2] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		*(float*)(d+i) = *(float*)(l.data + i) + *(float*)(r.data + i);
	}
	return Martix22(d);
}
