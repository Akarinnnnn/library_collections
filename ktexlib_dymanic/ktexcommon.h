#pragma once
#ifndef KTEX_COMMON
#define KTEX_COMMON
//#define _HAS_CXX17 1
#include <filesystem>
#include <vector>

namespace ktexlib
{
	typedef std::vector<unsigned char> uc_vector;
	typedef std::vector<uc_vector> datavec;
	struct b_box//boundry box
	{
		unsigned short w;	//width
		unsigned short h;	//height
		double x;			//x offset
		double y;			//y offset
	};
	struct mipmapv2
	{
		unsigned short w;
		unsigned short h;
		unsigned short pitch;
		unsigned int size;
		unsigned char* data;
		inline ~mipmapv2()
		{
			if (data != nullptr)
			{
				delete[] data;
				data = nullptr;
				size = 0;
			}
		}
	};
	struct rgbav2
	{
		unsigned short w;
		unsigned short h;
		unsigned short pitch;
		uc_vector data;
	};
}
#endif