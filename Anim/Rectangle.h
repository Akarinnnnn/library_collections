#pragma once
namespace Anim
{
	class Rectangle
	{
	public:
		Rectangle();
		~Rectangle();
		float x1, x2, y1, y2;
	};



	Rectangle::Rectangle()
	{
		x1 = 0.0f;
		x2 = 0.0f;
		y1 = 0;
		y2 = 0;
	}


	Rectangle::~Rectangle()
	{

	}
}
