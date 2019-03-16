#include "stdafx.h"
#include <algorithm>
#include "Bounding_Box.h"
using namespace Anim;

Anim::Bounding_Box::Bounding_Box()
{
	x, y, w, h = 0;
}

Anim::Bounding_Box::Bounding_Box(float x, float y, unsigned int w, unsigned int h)
{
	set(x, y, w, h);
}

Anim::Bounding_Box::Bounding_Box(const Anim::Rectangle & rect)
{
	x, y = rect.x1, rect.y1;
	w = std::ceil((unsigned int)std::abs(rect.x1 - rect.x2));
	h = std::ceil((unsigned int)std::abs(rect.y1 - rect.y2));
}

Anim::Bounding_Box::Bounding_Box(const Anim::xy & xy, const Anim::wh & wh)
{
	x, y = xy.x, xy.y;
	w, h = wh.w, wh.h;
}

void Anim::Bounding_Box::scale(float mp)
{
	w*=mp; h*=mp;
	x*=mp; y*=mp;
	//没问题的
}

void Anim::Bounding_Box::spilt(Anim::xy & xy, Anim::wh & wh)
{
	xy = { x,y };
	wh = { w,h };
}



Bounding_Box::~Bounding_Box()
{

}
