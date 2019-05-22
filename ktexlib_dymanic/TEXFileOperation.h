#pragma once
//�Ѿ��൱���洫�����ˣ������Զ
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <exception>
#include "ktexcommon.h"
//���߳̿���̨���
#ifdef MULTI_THREAD_KTEXCONOUTPUT
#include <mutex>
#endif


namespace ktexlib
{
	namespace KTEXFileOperation
	{

		class KTEX
		{
		public:
			__API void PushRGBA(RGBAv2 RGBA_array);
			__API void PushRGBA(RGBAv2 RGBA_array, unsigned int pitch);
			__API void Convert();
			__API void LoadKTEX(std::experimental::filesystem::path filepath);
			__API mipmapv2 GetMipmapByPitch(unsigned int pitch);
			__API mipmapv2 GetMipmap(unsigned int pitch);
			__API RGBAv2 GetImageFromMipmap(unsigned int pitch);
			__API RGBAv2 GetImageArray(unsigned int pitch);
			__API void clear();
			__API KTEX();
			__API ~KTEX();
			//__API friend void ktexlib::KTEXFileOperation::KTEX2PNG(KTEX target);
			__API void operator+=(RGBAv2 src);
			__API RGBAv2* operator[](int i);

			KTEXInfo Info;
			std::wstring output;
		private:
			mipmaps mipmaps;
			KTEXHeader Header;
			imgs RGBA_vectors;
		};
		//__API void KTEX2PNG(KTEX target);
		__API KTEX operator+(KTEX dest, RGBAv2 src);
	}
}