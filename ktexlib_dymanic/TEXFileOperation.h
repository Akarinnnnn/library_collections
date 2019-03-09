#pragma once
//�Ѿ��൱���洫�����ˣ������Զ
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <exception>
//���߳̿���̨���
#ifdef MULTI_THREAD_KTEXCONOUTPUT
#include <mutex>
#endif

#if KTEXLIBDYMANIC_EXPORTS
#define __API __declspec(dllexport)
#else
#define __API __declspec(dllimport)
#endif

namespace ktexlib
{
	namespace KTEXFileOperation
	{
		enum class  platfrm//platform
		{
			opengl = 12,
			xb360 = 11,
			ps3 = 10,
			unk = 0
		};
		enum class pixfrm //pixel form
		{
			 ARGB = 4,
			 DXT1 = 0,
			 DXT3 = 1,
			 DXT5 = 2,
			 unk = 7
		};
		enum class textyp //texture type
		{
			d1 = 1,//1d
			d2 = 2,//2d
			d3 = 3,//3d
			cube = 4//cubemap
		};
		typedef std::vector<unsigned char> uc_vector;

		class KTEXexception :public std::exception
		{
		public:
			__API ~KTEXexception() noexcept
			{
				delete[] msg;
			}
			__API KTEXexception(const char* msg ,int code) noexcept
			{
				this->msg = msg;
				this->_code = code;
			}
			__API char const* what() const noexcept
			{
				return msg;
			}
			__API int const code() noexcept
			{
				return _code;
			}
		private:
			char const* msg;
			int _code;
		};

		struct KTEXHeader
		{
			//CC4
			unsigned int cc4 = 0x5845544B;
			//��һ���ݿ�
			unsigned int firstblock = 0;
			//0xFFF 12bit, flags 2bit, mipscount 5bit, textype 4bit
			//pixelformat 5bit, platform 4bit
		};
		struct KTEXInfo
		{
			unsigned char flags = 0;
			unsigned short mipscount = 0;
			textyp texturetype = textyp::d1;
		 	pixfrm pixelformat = pixfrm::DXT5;
			platfrm platform = platfrm::opengl;
		};

		struct mipmap
		{
			unsigned short width = 0;
			unsigned short height = 0;
			unsigned short pitch = 1;//Z Axis
			uc_vector data;
		};

		struct RGBAv2
		{
			unsigned short width=0;
			unsigned short height=0;
			unsigned short pitch=0;
			uc_vector data;
		};

		struct mipmapv2
		{
			unsigned short width =0;
			unsigned short height=0;
			unsigned short pitch = 0;
			unsigned int size = 0;
			char* data = nullptr;
			__API ~mipmapv2();
		};
		
		typedef std::vector<mipmapv2> mipmaps;
		typedef std::vector<RGBAv2> imgs;

		class KTEX
		{
		public:
			__API void PushRGBA(RGBAv2 RGBA_array);
			__API void PushRGBA(RGBAv2 RGBA_array, unsigned int pitch);
			__API void Convert();
			__API void LoadKTEX(std::experimental::filesystem::path filepath);
			__API mipmapv2 GetMipmapByPitch(unsigned int pitch);
			__API mipmapv2 GetMipmap(size_t order);
			__API RGBAv2 GetImageFromMipmap(size_t order);
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