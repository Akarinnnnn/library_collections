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
		Martix33& scale(float multiply);//ƽ������
		Martix33& set_translation(float x, float y);//ͶӰ��
		Martix33& set_translation(xy v);//ͶӰ��

		float data[3][3]=
		{
		{1,0,0},
		{0,1,0},
		{0,0,1}
		};//����ռ�ֱ������ϵ
	};
}