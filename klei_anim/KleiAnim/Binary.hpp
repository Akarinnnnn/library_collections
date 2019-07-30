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

			std::vector<Common::AtlasNode> atlases;
			std::vector<Common::SymbolNode> symbols;
		};

		/// <summary>
		/// anim.bin读取器
		/// </summary>
		class EXPORT_API AnimationReader : protected AnimationBase
		{
		public:
			AnimationReader() = delete;
			AnimationReader(const std::filesystem::path & animpath);

			unsigned int anim_count() const;
			
			/// <summary>
			/// 返回hash对应的字符串，建议结合std::move使用
			/// </summary>
			/// <example><c>string name = std::move(anim.de_hash(name_hash));</c></example>
			/// <param name="hash">哈希值</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			std::string de_hash(const unsigned int hash) const;

			/// <summary>
			/// 获取i号动画
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const Common::AnimationNode& animation(const size_t i) const;

			/// <summary>
			/// 获取i号动画，与animation(size_t i)相同
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const Common::AnimationNode& operator[] (const size_t i) const;

			/// <summary>
			/// 获取相应动画的第frame帧
			/// </summary>
			/// <param name="anim">动画序号</param>
			/// <param name="i">帧序号</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const Common::AnimationFrameNode& frame(const size_t anim, const size_t frame) const;
		
			/// <summary>
			/// 获取anim号动画，第frame帧的事件（一般的动画没有）
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const std::vector<Common::EventNode>& events(const size_t anim, const size_t frame) const;		

			/// <summary>
			/// 获取anim号动画，第frame帧的元素引用
			/// </summary>
			/// <param name="anim"></param>
			/// <param name="frame"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/29</created>
			/// <changed>Fa鸽,2019/7/29</changed>
			const std::vector<Common::ElementNode>& element_refs(const size_t anim, const size_t frame) const;
		private:
			std::ifstream file;
		};

		/// <summary>
		/// build.bin读取器
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