#pragma once

#include <vector>
#include <filesystem>

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

		array_t::const_iterator begin() { return arr.begin(); }
		array_t::const_iterator end() { return arr.end(); }
	private:
		const array_t& arr;
	};

	namespace v3
	{
		/// <summary>
		/// 材质的像素格式
		/// </summary>
		enum class PixelFormat
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
		enum class Platform
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

		struct mipmap
		{
			mipmap(int32_t w,int32_t h,int32_t pitch):data(){}
			mipmap(const mipmap& other) :mipmap(other.width, other.height, other.pitch,data){}
			mipmap(const mipmap&& xval):mipmap(xval.width,xval.height,xval.pitch,std::move(xval.data)){}

			mipmap(const int32_t w, const int32_t h, const int32_t pitch, const std::vector<unsigned char>& data) :
				width(w), height(h), pitch(pitch)
			{
				this->data = data;
			}
			mipmap(const int32_t w, const int32_t h, const int32_t pitch, const std::vector<unsigned char>&& data) :
				width(w), height(h), pitch(pitch)
			{
				this->data = std::move(data);
			}

			int32_t width, height, pitch = 0;//pitch是一行的数据长度，以字节为单位
			std::vector<unsigned char> data;
		};

		using rgba_image = mipmap;

		class Ktex final
		{
		public:
			template<typename rgba_iterator>
			Ktex(rgba_iterator begin,rgba_iterator end):Mipmaps(this->mips)
			{
				range rgbas(begin, end);
				for (auto& i : rgbas)
				{
					mips.push_back(do_compress(i,true));
				}
			}

			ro_container_property<mipmap,std::vector<mipmap>> Mipmaps;

			void SaveFile(std::filesystem::path path);
		private:
			mipmap do_compress(const rgba_image& rgba, bool multithread, PixelFormat fmt = PixelFormat::dxt5);
			std::vector<mipmap> mips;
		};

		/// <summary>
		/// 输入文件名，直接转换
		/// </summary>
		/// <param name="filename">文件名</param>
		/// <created>Fa鸽,2019/11/1</created>
		/// <changed>Fa鸽,2019/11/15</changed>
		extern "C" EXPORT_API void gen_bc3universal(char8_t * filename);
	}
}