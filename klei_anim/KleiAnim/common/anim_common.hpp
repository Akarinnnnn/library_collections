/***************************
* Encoding: GB2312
* KleiAnim的共用部分，包含结构体定义及共用函数
***************************/
#pragma once

#include <string>
#include <vector>
//#include <typeinfo>
#include <fstream>
#include <map>
#include "export.h"
#include <sstream>
#include <functional>


#ifndef KLEIANIM_USE_CHARLOG
#define KleiAnimLog KleiAnim::Common::WideCharLog
#else
#define KleiAnimLog KleiAnim::Common::CharLog
#endif // KLEIANIM_USE_CHLOG



//#include <type_traits>
//assert size
static_assert(sizeof(unsigned int) == 4,"unsigned int must be 4 bytes long");
static_assert(sizeof(unsigned short) == 2,"unsigned short must be 2 bytes long");
static_assert(sizeof(float) == 4,"float must be 4 bytes long");
//static_assert(sizeof(KleiAnim::Common::ElementNode) == 40);

//assert align
static_assert(alignof(unsigned short) == 2,"unsigned short must be 2-byte aligned");
static_assert(alignof(float) == 4,"float must be 4-byte aligned");
static_assert(alignof(unsigned int) == 4,"unsigned int must be 4-byte aligned");


//assert LE(C++ 20)
//确保目标机器是小端机
//static_assert(std::endian::native == std::endian::little)


namespace KleiAnim
{
	namespace Common
	{
		/// <summary>
		/// 若要改变KleiAnim的日志输出，只需在调用KleiAnim之前构造一个实例
		/// </summary>
		class CharLog
		{
		public:
			EXPORT_API CharLog(std::ostream& output);
			static std::ostream& write();
		private:
			inline static CharLog* in_use = nullptr;
			std::ostream* stream;
		};

		/// <summary>
		/// 若要改变KleiAnim的日志输出，只需在调用KleiAnim之前构造一个实例
		/// </summary>
		class WideCharLog
		{
		public:
			EXPORT_API WideCharLog(std::wostream& output);
			static std::wostream& write();
		private:
			inline static WideCharLog* in_use = nullptr;
			std::wostream* stream;
		};

		///<summary>
		///图片朝向
		///</summary>
		enum class Facing : unsigned char
		{
			/// <summary>右</summary>
			right = 1,

			/// <summary>上</summary>
			up = 1 << 1,

			/// <summary>左</summary>
			left = 1 << 2,

			/// <summary>下</summary>
			down = 1 << 3,

			/// <summary>右上</summary>
			upright = 1 << 4,

			/// <summary>左上</summary>
			upleft = 1 << 5,

			/// <summary>右下</summary>
			downright = 1 << 6,

			/// <summary>左下</summary>
			downleft = 1 << 7,

			/// <summary>上下左右</summary>
			all90 = up | left | right | down,

			/// <summary>四个角</summary>
			all45 = upright | upleft | downleft | downright,

			/// <summary>所有朝向</summary>
			all = 0xFFUi8,

			/// <summary>这不可能！！</summary>
			invalid = 0
		};

		/// <summary>
		/// 动画用bin文件的共用部分
		/// </summary>
		struct BinaryFileBase
		{
			unsigned int cc4 = 0;
			signed int version = 0;

			/// <summary>哈希化字符串表</summary>
			std::map<unsigned int, std::string> str_table;
		};

		/// <summary>
		/// OpenGL顶点
		/// </summary>
		struct AlphaVertexNode
		{
			float x, y, z, u, v, w;
		};

		/// <summary>
		/// Build中的帧节点
		/// </summary>
		struct BuildFrameNode
		{
			/// <summary>帧号</summary>
			unsigned int frame_number;

			/// <summary>持续时间</summary>
			unsigned int duration;

			/// <summary>bbox xywh参数</summary>
			float x, y, w, h;


			unsigned int alpha_index;
			unsigned int alpha_count;
		};

		/// <summary>
		/// 符号节点
		/// </summary>
		struct SymbolNode
		{
			unsigned int name_hash;
			std::vector<BuildFrameNode> frames;
		};

		/// <summary>
		/// 事件节点
		/// </summary>
		struct EXPORT_API EventNode
		{
			/// <summary>名称的哈希</summary>
			unsigned int name_hash;

			operator unsigned int()
			{
				return name_hash;
			}

			EventNode()
			{
				name_hash = 0;
			}

			EventNode(unsigned int h)
			{
				name_hash = h;
			}
		};

		/// <summary>
		/// 元素节点
		/// </summary>
		struct ElementNode
		{
			/// <summary>哈希</summary>
			unsigned int name_hash;
			unsigned int frame;
			unsigned int layer_hash;

			float a, b, c, d;
			float tx, ty;
			float z;
		};

		/// <summary>
		/// Animation中的帧节点
		/// </summary>
		struct AnimationFrameNode
		{
			float x, y, w, h;

			std::vector<EventNode> events;//event count + event
			std::vector<ElementNode> elements;//elem count + elems
		};

		/// <summary>
		/// 动画节点
		/// </summary>
		struct AnimationNode
		{
			std::string name;
			Facing facing;
			unsigned int rootsym_hash;
			float frame_rate;

			std::vector<AnimationFrameNode> frames;
		};

		/// <summary>
		/// Atlas
		/// </summary>
		struct AtlasNode
		{
			std::string name;
		};

		/// <summary>
		/// 计算字符串的哈希值
		/// </summary>
		/// <param name="s">将计算哈希的字符串</param>
		/// <returns>哈希值</returns>
		/// <created>Fa鸽,2019/7/22</created>
		/// <changed>Fa鸽,2019/7/22</changed>
		unsigned int EXPORT_API hash(std::string&& s);

		/// <summary>
		/// 读取bin文件中的一条字符串（非哈希化字符串）
		/// </summary>
		/// <param name="f">流，必须行进到字符串所在的位置</param>
		/// <returns>读取出的字符串</returns>
		/// <created>Fa鸽,2019/7/24</created>
		/// <changed>Fa鸽,2019/7/24</changed>
		std::string read_str(std::istream& f);

		/// <summary>
		/// 读取哈希化的字符串表
		/// </summary>
		/// <param name="f">文件流，必须行进到字符串表所在位置</param>
		/// <returns>字符串表</returns>
		/// <created>Fa鸽,2019/7/24</created>
		/// <changed>Fa鸽,2019/7/24</changed>
		std::map<unsigned int, std::string> read_strhashtable(std::ifstream& f);

		/// <summary>
		/// 获取元素节点的字符串(<see cref="std::wstring"/>)表示，仅支持wstring
		/// </summary>
		/// <param name="elem">元素节点</param>
		/// <returns>返回<see cref="std::wstring"/>，建议配合std::move使用以避免构造开销</returns>
		/// <example><c>wstring elem_str = std::move(ToString(elem));</c></example>
		/// <created>Fa鸽,2019/7/27</created>
		/// <changed>Fa鸽,2019/7/27</changed>
		std::wstring EXPORT_API ToString(const ElementNode& elem);

		/// <summary>
		/// 比较两个ElementNode是否相等
		/// </summary>
		/// <param name="l">1</param>
		/// <param name="r">2</param>
		/// <returns>相等返回true，不相等返回false</returns>
		/// <created>Fa鸽,2019/7/27</created>
		/// <changed>Fa鸽,2019/7/27</changed>
		bool EXPORT_API operator==(const ElementNode& l, const ElementNode& r);
	}
}