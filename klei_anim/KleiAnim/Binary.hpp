/***************************
* Encoding: GB2312
* 对二进制的动画进行读写
***************************/
#pragma once
#include "common/anim_common.hpp"
#include "common/export.h"
#include <filesystem>
#include <fstream>
namespace KleiAnim
{
	namespace Binary
	{
		class EXPORT_API AnimationBase: protected virtual Common::BinaryFileBase
		{
		protected:
			///<summary>合规文件的CC4，ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>当前版本</summary>
			static constexpr unsigned short cur_version = 0x0004;

			/// <summary>animation</summary>
			std::vector<Common::AnimationNode> animations;
		};

		class EXPORT_API BuildBase : protected virtual Common::BinaryFileBase
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
		class EXPORT_API AnimationReader : protected AnimationBase
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
		class EXPORT_API BuildReader :protected BuildBase
		{
		public:
			BuildReader() = delete;
			BuildReader(std::filesystem::path& buildpath);

		private:
			std::ifstream file;
		};
	}
}