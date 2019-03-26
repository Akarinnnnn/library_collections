#pragma once
#include "Rectangle.h"
#include "xywh.h"
namespace Anim
{
	 class __declspec(dllexport) Bounding_Box
	{
	private:
		float x, y;
		unsigned int w, h;
		inline void set(float x, float y, unsigned int w, unsigned int h)
		{
			this->x, this->y = x, y;
			this->w, this->h = w, h;
		}
	public:
		Bounding_Box();
		Bounding_Box(float x, float y, unsigned int w, unsigned int h);
		Bounding_Box(const Anim::Rectangle& rect);
		Bounding_Box(const Anim::xy& xy,const Anim::wh& wh);

		void scale(float muliply);
		void spilt(Anim::xy& xy, Anim::wh& wh);

		~Bounding_Box();
	};
}


