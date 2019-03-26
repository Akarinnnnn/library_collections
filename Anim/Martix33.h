#pragma once
#include "xywh.h"

namespace Anim
{
	class __declspec(dllexport) Martix33
	{
	public:
		Martix33();
		Martix33 Hadamart(const Martix33);
		Martix33 operator*(const Martix33&);
		Martix33& operator-();
		Martix33 operator-(const Martix33&);
		Martix33 operator+(const Martix33&);
		xy get_translation();
		Martix33& scale(float multiply);//平面缩放
		Martix33& set_translation(float x, float y);//投影？
		Martix33& set_translation(xy v);//投影？

		float data[3][3]=
		{
		{1,0,0},
		{0,1,0},
		{0,0,1}
		};//常规空间直角坐标系
	};
}