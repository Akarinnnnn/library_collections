#pragma once
#include "ktexcommon.h"
namespace ktexlib
{
	namespace Atlas
	{
		class atlas
		{
		public:
			atlas(std::filesystem::path path, std::wstring outputname, std::vector<rgbav2> images);
			atlas(std::filesystem::path path, std::wstring outputname, std::vector<mipmapv2> mipmaps);
			void xmlgen();
		private:
			std::vector<b_box> bboxes;
			std::wstring path;
			std::wstring texfilename;
		};
		
		/*template <typename T>
		class martix33
		{
		private:
			T data[3][3];
		public:
			martix33<T> operator* (martix33<T> R)
			{
				for (int i = 0; i < 9; i++)
					(*(this->data + i))*(*(R.data + i));
			}
			martix33<T> operator+ (martix33<T> R)
			{
				for (int i = 0; i < 9; i++)
					(*(this->data + i)) + (*(R.data + i));
			}
			martix33<T> operator-(martix33<T> R)
			{
				for (int i = 0; i < 9; i++)
					(*(this->data + i)) - (*(R.data + i))
			}
		};*/
	}
}