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

Anim::Bounding_Box::Bounding_Box(Anim::Rectangle & const rect)
{
	x, y = rect.x1, rect.y1;
	w = std::ceil((unsigned int)std::abs(rect.x1 - rect.x2));
	h = std::ceil((unsigned int)std::abs(rect.y1 - rect.y2));
}

Anim::Bounding_Box::Bounding_Box(Anim::xy & xy, Anim::wh & wh)
{
	x, y = xy.x, xy.y;
	w, h = wh.x, wh.y;
}


Bounding_Box::~Bounding_Box()
{

}
