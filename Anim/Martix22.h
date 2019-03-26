#pragma once
#include "xywh.h"
namespace Anim
{
	class __declspec(dllexport) Martix22
	{
	public:
		explicit Martix22();
		explicit Martix22(float d[2][2]);
		float data[2][2] = 
		{ 
			{1,0},
			{0,1} 
		};
	};
	//Martix22 operator*(const Martix22 & l, const Martix22 & r);
	Martix22 operator+(const Martix22 & l, const Martix22 & r);
}