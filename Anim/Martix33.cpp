#include "stdafx.h"
#include "Martix33.h"
//#include <array>
using namespace Anim;
Anim::Martix33::Martix33()
{

}
Martix33 Anim::Martix33::Hadamart(const Martix33 a)
{
	Martix33 ret;
	for (int i = 0; i < 9; i++)
	{
		*(float*)(ret.data + i) = *((float*)this->data + i) * *((float*)a.data + i);
	}
	return ret;
}

Martix33 Anim::Martix33::operator*(const Martix33& a)
{
	Martix33 ret;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ret.data[i][j] = data[i][1] * a.data[1][j] + data[i][2] * a.data[2][j] + data[i][3] * a.data[3][j];
			//来自百毒百科，估计有更NB的算法。
	return ret;
}


Martix33& Anim::Martix33::operator-()
{
	for (int i=0;i<9;i++)
	{
		float& elem = *((float*)(data)+i);
		elem = -elem;
	}
	return *this;
}

Martix33 Anim::Martix33::operator-(const Martix33& a)
{
	Martix33 ret;
	for (int i = 0; i < 9; i++)
	{
		*(float*)(ret.data + i) = *((float*)this->data + i) - *((float*)a.data + i);
	}
	return ret;
}

Martix33 Anim::Martix33::operator+(const Martix33& a)
{
	Martix33 ret;
	for (int i = 0; i < 9; i++)
	{
		*(float*)(ret.data + i) = *((float*)this->data + i) + *((float*)a.data + i);
	}
	return ret;
}

xy Anim::Martix33::get_translation()
{
	return xy() = { data[0][2],data[1][2] };
}

Martix33 & Anim::Martix33::scale(float mp)
{
	data[0][0] *= mp;
	data[1][1] *= mp;
	return *this;
}

Martix33& Anim::Martix33::set_translation(float x, float y)
{
	data[0][2] = x;
	data[1][2] = y;
	return *this;
}

Martix33 & Anim::Martix33::set_translation(xy v)
{
	data[0][2] = v.x;
	data[1][2] = v.y;
	return *this;
}
