/***************************
* Encoding: GB2312
* 对二进制的动画进行读写
***************************/
#pragma once
#include "Shared/anim_common.hpp"
#include <filesystem>
#include <fstream>
namespace KleiAnim
{
	namespace Binary
	{
		class AnimationBase: protected virtual Shared::BinaryFileBase
		{
		protected:
			///<summary>合规文件的CC4，ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>当前版本</summary>
			static constexpr unsigned short cur_version = 0x0004;

			/// <summary>元素数量</summary>
			unsigned int elem_count = 0;

			/// <summary>帧数量</summary>
			unsigned int frame_count = 0;

			/// <summary>事件数量</summary>
			unsigned int event_count = 0;

			/// <summary>animation数量</summary>
			unsigned int anim_count = 0;
		};

		class BuildBase : protected virtual Shared::BinaryFileBase
		{
		protected:
			///<summary>合规文件的CC4，BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>当前版本</summary>
			static constexpr unsigned short cur_version = 0x0006;

			unsigned int symbol_count;
			unsigned int frame_count;
			std::string name;

		};

		/// <summary>
		/// anim.bin读取
		/// </summary>
		class AnimationReader : protected AnimationBase
		{
		public:
			AnimationReader() = delete;
			AnimationReader(std::filesystem::path& animpath);
		private:
			std::ifstream file;
		};

		/// <summary>
		/// build.bin读取
		/// </summary>
		class BuildReader :protected BuildBase
		{
		public:
			BuildReader() = delete;
			BuildReader(std::filesystem::path& buildpath);

		private:
			std::ifstream file;
		};
	}
}