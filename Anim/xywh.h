#pragma once
namespace Anim
{
	template <typename T>
	struct pt2
	{
		T x, y = 0;
	};
	typedef pt2<float> xy;
	typedef pt2<int> wh;
}