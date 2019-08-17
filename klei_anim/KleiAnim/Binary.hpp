/***************************
* Encoding: GB2312
* 对二进制的动画进行读写
***************************/
#pragma once
#include "common/anim_common.hpp"
#include "common/export.h"
#include <filesystem>
#include <fstream>
#include <array>

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

			unsigned int symbol_count = 0;
			unsigned int frame_count = 0;
			std::string build_name;

			std::vector<Common::AtlasNode> atlases;
			std::vector<Common::SymbolNode> symbols;
			std::vector<Common::AlphaVertexNode> vertices;
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
			
			std::vector<Common::AnimationNode>::const_iterator begin() const;
			std::vector<Common::AnimationNode>::const_iterator end() const;


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
		};

		/// <summary>
		/// build.bin读取器
		/// </summary>
		class EXPORT_API BuildReader :protected BuildBase
		{
		public:
			BuildReader() = delete;
			BuildReader(const std::filesystem::path & buildpath);

			std::vector<Common::SymbolNode>::const_iterator begin() const;
			std::vector<Common::SymbolNode>::const_iterator end() const;

			unsigned int symbol_count() const;
			unsigned int atlas_count() const;
			unsigned int vertex_count() const;

			
			/// <summary>
			/// 名称
			/// </summary>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			std::string name() const;

			/// <summary>
			/// 获取 i 号 符号
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/7/31</created>
			/// <changed>Fa鸽,2019/7/31</changed>
			const Common::SymbolNode& symbol(const size_t i) const;

			/// <summary>
			/// 获取 i 号 符号
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::SymbolNode& operator[] (const size_t i) const;

			/// <summary>
			/// 获取 i 号atlas
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::AtlasNode& atlas(const size_t i) const;

			/// <summary>
			/// 获取 i 号顶点三角形
			/// </summary>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::AlphaVertexNode& vertex(const size_t i) const;

			/// <summary>
			/// 获取第i组顶点三角形
			/// </summary>
			/// <returns>std::array，不是内置数组</returns>
			/// <example><c>for(auto&amp; vertex: bin.vertices(0))</c></example>
			/// <param name="start">第i组</param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			std::array<Common::AlphaVertexNode, 6> vertices(const unsigned int start) const;

			/// <summary>
			/// 获取sym 符号的 i 帧
			/// </summary>
			/// <param name="sym"></param>
			/// <param name="i"></param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			const Common::BuildFrameNode& frame(const size_t sym, const size_t i) const;			
		};

		class EXPORT_API AnimationWriter :AnimationBase
		{
		public:

			/// <summary>
			/// 以指定的输出路径构造AnimationWriter
			/// </summary>
			/// <param name="out">输出的路径，含文件名</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			AnimationWriter(const std::filesystem::path& out);
			AnimationWriter(const std::filesystem::path& out, const AnimationBase & base);
			~AnimationWriter();

			/// <summary>
			/// 将内容写入到文件
			/// </summary>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void writefile();

			/// <summary>
			/// 将内容写入到流
			/// </summary>
			/// <param name="out"></param>
			/// <created>Fa鸽,2019/8/17</created>
			/// <changed>Fa鸽,2019/8/17</changed>
			void writestream(std::ostream& out);

			/// <summary>
			/// 添加一段动画
			/// </summary>
			/// <param name="anim">待添加的动画</param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::AnimationNode& anim);

			/// <summary>输出文件的路径</summary>
			std::filesystem::path out;
		};

		class EXPORT_API BuildWriter : BuildBase
		{
		public:
			/// <summary>
			/// 以指定的输出路径构造BuildWriter
			/// </summary>
			/// <param name="out">输出的路径，含文件名</param>
			/// <returns></returns>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			BuildWriter(const std::filesystem::path& out);
			BuildWriter(const std::filesystem::path & out, const BuildBase & base);
			~BuildWriter();

			/// <summary>
			/// 将内容写入到文件
			/// </summary>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void writefile();

			/// <summary>
			/// 将内容写入到流
			/// </summary>
			/// <param name="out"></param>
			/// <created>Fa鸽,2019/8/17</created>
			/// <changed>Fa鸽,2019/8/17</changed>
			void writestream(std::ostream& out);

			/// <summary>
			/// 添加一个符号
			/// </summary>
			/// <param name="frame"></param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::SymbolNode& sym);

			/// <summary>
			/// 添加一个顶点三角形
			/// </summary>
			/// <param name="vert"></param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::AlphaVertexNode& vert);

			/// <summary>
			/// 添加一个atlas
			/// </summary>
			/// <param name="atlas"></param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(Common::AtlasNode& atlas);

			/// <summary>
			/// 添加一组（6个）顶点三角形
			/// </summary>
			/// <param name="vertices">数组</param>
			/// <created>Fa鸽,2019/8/2</created>
			/// <changed>Fa鸽,2019/8/2</changed>
			void add(const std::array<Common::AlphaVertexNode,6>& vertices);

			/// <summary>输出文件的路径</summary>
			std::filesystem::path out;
		};
	}
}
