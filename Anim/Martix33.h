#pragma once
#include "xywh.h"

namespace Anim
{
	class __declspec(dllexport) Martix33
	{
	public:
		Martix33& Hadamart(Martix33&);
		Martix33& operator*(Martix33&);
		Martix33& operator-();
		Martix33& operator-(Martix33);
		Martix33& operator+(Martix33&);
		Martix33& operator/(Martix33);
		xy& get_translation();
		Martix33& set_translation(float, float);
		float data[3][3];
	};
}