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

			std::vector<Common::AtlasNode> atlases;
			std::vector<Common::SymbolNode> symbols;
		};

		/// <summary>
		/// anim.bin��ȡ��
		/// </summary>
		class EXPORT_API AnimationReader : protected AnimationBase
		{
		public:
			AnimationReader() = delete;
			AnimationReader(const std::filesystem::path & animpath);

			unsigned int anim_count() const;
			
			/// <summary>
			/// ����hash��Ӧ���ַ�����������std::moveʹ��
			/// </summary>
			/// <example><c>string name = std::move(anim.de_hash(name_hash));</c></example>
			/// <param name="hash">��ϣֵ</param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			std::string de_hash(const unsigned int hash) const;

			/// <summary>
			/// ��ȡi�Ŷ���
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const Common::AnimationNode& animation(const size_t i) const;

			/// <summary>
			/// ��ȡi�Ŷ�������animation(size_t i)��ͬ
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const Common::AnimationNode& operator[] (const size_t i) const;

			/// <summary>
			/// ��ȡ��Ӧ�����ĵ�frame֡
			/// </summary>
			/// <param name="anim">�������</param>
			/// <param name="i">֡���</param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const Common::AnimationFrameNode& frame(const size_t anim, const size_t frame) const;
		
			/// <summary>
			/// ��ȡanim�Ŷ�������frame֡���¼���һ��Ķ���û�У�
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const std::vector<Common::EventNode>& events(const size_t anim, const size_t frame) const;		

			/// <summary>
			/// ��ȡanim�Ŷ�������frame֡��Ԫ������
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa��,2019/7/29</created>
			/// <changed>Fa��,2019/7/29</changed>
			const std::vector<Common::ElementNode>& element_refs(const size_t anim, const size_t frame) const;
		private:
			std::ifstream file;
		};

		/// <summary>
		/// build.bin��ȡ��
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