#pragma once
#include "Anim.h"
namespace Anim
{
	class __declspec(dllexport) Martix33
	{
	public:
		Martix33 operator*(Martix33);
		Martix33 operator-();
		Martix33 operator-(Martix33);
		Martix33 operator+(Martix33);
		Martix33 operator/(Martix33);
		Martix33& get_translation();
		void set_translation(float, float);
	private:
		float data[3][3];
	};
	struct xy
	{
		float x = 0.0f;
		float y = 0.0f;

	};
}