#include "KTEXAtlas.h"
#include <pugiconfig.hpp>
#include <pugixml.hpp>//vcpkg了解一下？
#include <algorithm>
using namespace std;
using namespace ktexlib::Atlas;
using namespace ktexlib;

unsigned short inline next2n(unsigned short a)
{
	return a + (a % 2);
}

bool inline operator<= (b_box l, b_box r)
{
	return (
		l.x <= r.w + r.x || l.y <= r.y + r.h
		);
}

ktexlib::Atlas::atlas::atlas(std::filesystem::path path, std::wstring outputname, vector<ktexlib::rgbav2> imgs)
{
	this->path = path.wstring();
	for (auto img : imgs)
	{
		b_box tmp = 
		{
			next2n(img.w),
			next2n(img.h),
			0.5,
			0.5
		};
		bboxes.push_back(tmp);
	}
}

ktexlib::Atlas::atlas::atlas(std::filesystem::path path, std::wstring outputname, vector<ktexlib::mipmapv2> m)
{
	this->path = path.wstring();
	for (auto mipmap : m)
	{
		b_box tmp = 
		{
			next2n(mipmap.w),
			next2n(mipmap.h),
			0.5,
			0.5
		};
		bboxes.push_back(tmp);
	}
}

void ktexlib::Atlas::atlas::xmlgen()
{
	double uvs[4] = { 0.0 };
}
