#pragma once

#if defined(KTEXLIB3_EXPORTS) 
#if defined(_MSC_VER)
#define	KTEXLIB3_EXPORT __declspec(dllexport)
#else
#define KTEXLIB3_EXPORT __declspec(dllimport)
#endif
#else
#define KTEXLIB3_EXPORT
#endif 

#include <vector>
#include <filesystem>
#include <fstream>
//#define PARALLEL_COMPRESS //去掉注释，在windows下进行多线程bc压缩

namespace ktexlib
{
	template<typename iter_beg,typename iter_end>
	struct range
	{
		iter_beg _begin;
		iter_end _end;

		range(iter_beg begin, iter_end end):_begin(begin),_end(end) {}

		iter_beg begin() { return _begin; }
		iter_end end() { return _end; }
	};

	template<typename T,typename array_t>
	struct ro_container_property
	{
		ro_container_property(const array_t& arr):arr(arr) {}

		const T& operator() (size_t i)
		{
			return (arr)[i];
		}

		const T& operator[] (size_t i)
		{
			return (arr)[i];
		}

		auto begin() { return arr.cbegin(); }
		auto end() { return arr.cend(); }
	private:
		const array_t& arr;
	};

	template<typename T>
	struct ro_property
	{
		ro_property(T& val) :val(val)
		{

		}

		operator T()
		{
			return val;
		}
		T operator() ()
		{
			return val;
		}
		T& val;
	};

	namespace v3detail
	{
		/// <summary>
		/// 材质的像素格式
		/// </summary>
		enum class PixelFormat :unsigned short
		{
			/// <summary>未知格式</summary>
			unknown = 0,
			/// <summary>bc1</summary>
			dxt1 = 1,
			/// <summary>bc2</summary>
			dxt3 = 2,
			/// <summary>最常用,bc3</summary>
			dxt5 = 3,
			/// <summary>高保真，大小很大</summary>
			rgba = 4,
			/// <summary>滤镜用的</summary>
			r8g8b8 = 7,
		};

		/// <summary>
		/// 材质对应平台
		/// </summary>
		enum class Platform : unsigned char
		{
			/// <summary>OpenGL，适合饥荒PC版，可以不生成mipmap</summary>
			opengl = 12,
			/// <summary>XBox360，要求未知</summary>
			xb360 = 11,
			/// <summary>PS3，要求未知</summary>
			ps3 = 10,
			/// <summary>适合任意平台，必须生成mipmap</summary>
			universal = 0
		};

		/// <summary>
		/// 材质种类
		/// </summary>
		enum class textureType : unsigned char
		{
			/// <summary>1D</summary>
			d1 = 1,
			/// <summary>2D</summary>
			d2 = 2,
			/// <summary>3D</summary>
			d3 = 3,
			/// <summary>Cubemap，适合2D游戏</summary>
			cubemap = 4
		};

		struct Mipmap
		{
			Mipmap(int32_t w, int32_t h, int32_t pitch);
			Mipmap(const Mipmap& other);
			Mipmap(Mipmap&& xval);

			Mipmap(const int32_t w, const int32_t h, const int32_t pitch, const std::vector<unsigned char>& data);
			Mipmap(const int32_t w, const int32_t h, const int32_t pitch, std::vector<unsigned char>&& data);

			int32_t width, height, pitch = 0;//pitch是一行的数据长度，以字节为单位
			std::vector<unsigned char> data;
		};

		struct RgbaImage : Mipmap {};

		struct KTEXInfo
		{
			uint8_t flags = 3ui8;
			textureType textureType = textureType::d2;
			PixelFormat pixelFormat = PixelFormat::dxt5;
			Platform platform = Platform::universal;
		};

		class Ktex
		{
		public:
			Ktex() :Mipmaps(this->mips) {}

			template<typename mips_iterator>
			Ktex(mips_iterator begin, mips_iterator end): mips(begin, end),Mipmaps(this->mips) {}

			void AddMipmap(const Mipmap& mipmap);
			void AddMipmap(Mipmap&& mipmap);

			ro_container_property<Mipmap, std::vector<Mipmap>> Mipmaps;
			KTEXInfo info;

			void SaveFile(std::filesystem::path path);
		private:
			std::vector<Mipmap> mips;

		};

		KTEXLIB3_EXPORT Mipmap convert(const RgbaImage& image, bool multithread, PixelFormat fmt);
	}

	namespace v3
	{
		/// <summary>
		/// 指定文件名，转换到同一目录
		/// </summary>
		/// <param name="filename">文件名</param>
		/// <created>Fa鸽,2019/11/1</created>
		/// <changed>Fa鸽,2019/11/15</changed>
		extern "C" KTEXLIB3_EXPORT void gen_bc3universal(const char8_t* pngpath);

		/// <summary>
		/// 指定UTF16文件名，转换到同一目录
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		/// <created>Fa鸽,2019/11/23</created>
		/// <changed>Fa鸽,2019/11/23</changed>
		KTEXLIB3_EXPORT bool gen_bc3universal(const char16_t * path);

		/// <summary>
		/// 指定宽字符文件名，转换到同一目录
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		/// <created>Fa鸽,2019/11/23</created>
		/// <changed>Fa鸽,2019/11/23</changed>
		KTEXLIB3_EXPORT bool gen_bc3universal(const wchar_t * path);

		/// <summary>
		/// 加载一个tex文件
		/// </summary>
		/// <param name="path">生存期由调用方管理</param>
		/// <returns></returns>
		/// <exception cref="std::invalid_argument">不是ktex</exception>
		/// <created>Fa鸽,2019/11/23</created>
		/// <changed>Fa鸽,2019/11/23</changed>
		KTEXLIB3_EXPORT ktexlib::v3detail::Ktex load_ktex(const char8_t* path);

		/// <summary>
		/// 加载一个tex文件
		/// </summary>
		/// <param name="path">生存期由调用方管理</param>
		/// <returns></returns>
		/// <exception cref="std::invalid_argument">不是ktex</exception>
		/// <created>Fa鸽,2019/11/23</created>
		/// <changed>Fa鸽,2019/11/23</changed>
		KTEXLIB3_EXPORT ktexlib::v3detail::Ktex load_ktex(const wchar_t* path);

		/// <summary>
		/// 加载一个tex文件
		/// </summary>
		/// <param name="file">生存期由调用方管理</param>
		/// <exception cref="std::invalid_argument">不是ktex</exception>
		/// <returns></returns>
		/// <created>Fa鸽,2019/11/23</created>
		/// <changed>Fa鸽,2019/11/23</changed>
		KTEXLIB3_EXPORT ktexlib::v3detail::Ktex load_ktex(std::ifstream& file);
	}

}