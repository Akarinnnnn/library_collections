/***************************
* Encoding: GB2312
* �Զ����ƵĶ������ж�д
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
			///<summary>�Ϲ��ļ���CC4��ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>��ǰ�汾</summary>
			static constexpr unsigned short cur_version = 0x0004;

			/// <summary>animation</summary>
			std::vector<Common::AnimationNode> animations;
		};

		class EXPORT_API BuildBase : protected virtual Common::BinaryFileBase
		{
		protected:
			///<summary>�Ϲ��ļ���CC4��BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>��ǰ�汾</summary>
			static constexpr unsigned short cur_version = 0x0006;

			unsigned int symbol_count;
			unsigned int frame_count;
			std::string name;

		};

		/// <summary>
		/// anim.bin��ȡ
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
		/// build.bin��ȡ
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