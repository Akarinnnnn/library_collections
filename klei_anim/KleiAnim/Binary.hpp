/***************************
* Encoding: GB2312
* �Զ����ƵĶ������ж�д
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
			///<summary>�Ϲ��ļ���CC4��ANIM</summary>
			static constexpr unsigned int valid_cc4 = 0x4D494E41;
			///<summary>��ǰ�汾</summary>
			static constexpr unsigned short cur_version = 0x0004;

			/// <summary>Ԫ������</summary>
			unsigned int elem_count = 0;

			/// <summary>֡����</summary>
			unsigned int frame_count = 0;

			/// <summary>�¼�����</summary>
			unsigned int event_count = 0;

			/// <summary>animation����</summary>
			unsigned int anim_count = 0;
		};

		class BuildBase : protected virtual Shared::BinaryFileBase
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
		class AnimationReader : protected AnimationBase
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