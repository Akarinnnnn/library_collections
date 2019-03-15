#pragma once
#include "Rectangle.h"
#include "xywh.h"
namespace Anim
{
	class Bounding_Box
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
		explicit Bounding_Box(Anim::Rectangle& const rect);
		explicit Bounding_Box(Anim::xy& xy, Anim::wh& wh);
		~Bounding_Box();
	};
}


