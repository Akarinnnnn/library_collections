/***************************
* Encoding: GB2312
***************************/
#pragma once

#include <string>
#include <vector>
//#include <type_traits>
//assert size
static_assert(sizeof(unsigned int) == 4);
static_assert(sizeof(unsigned short) == 2);

//assert LE(C++ 20)
//static_assert(std::endian::native == std::endian:little)

//static_assert(sizeof(unsigned long long) == 8);

namespace KleiAnim
{
	using u8string = std::string;
	namespace Shared
	{
		///<summary>图片朝向</summary>
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
			alldir = 0xFF,

			/// <summary>这不可能！！</summary>
			invalid = 0
		};

		/// <summary>
		/// 动画用bin头的共用部分
		/// </summary>
		struct bin_base
		{
			//member
			unsigned int cc4 = 0;
			signed int version = 0;
		};

		/// <summary>
		/// anim.bin
		/// </summary>
		struct Animation :bin_base
		{
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

			/// <summary>哈希化字符串表</summary>
			std::vector<hashed> str_table;
		};

		/// <summary>
		/// build.bin
		/// </summary>
		struct Build :bin_base
		{
			///<summary>合规文件的CC4，BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>当前版本</summary>
			static constexpr unsigned short cur_version = 0x0006;

			unsigned int symbol_count;
			unsigned int frame_count;
			u8string name;


		};

		/// <summary>
		/// 符号节点
		/// </summary>
		struct sym_node
		{
			unsigned int hash;

		};

		/// <summary>
		/// 动画节点
		/// </summary>
		struct anim_node
		{
			u8string name;
			Facing facing;
			unsigned int root_hash;
			float frame_rate;

			std::vector<frame_node> frames;
		};

		/// <summary>
		/// 画框节点
		/// </summary>
		struct frame_node
		{
			float x, y, w, h;

			std::vector<event_node> events;//event count + event
			std::vector<elem_node> elements;//elem count + elems
		};

		/// <summary>
		/// 事件节点
		/// </summary>
		struct event_node
		{
			/// <summary>哈希</summary>
			unsigned int hash; //??????
		};

		/// <summary>
		/// 元素节点
		/// </summary>
		struct elem_node
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
		/// 哈希化的字符串
		/// </summary>
		struct hashed
		{
			unsigned int hash;
			u8string original;
		};

		//Aliases
		using facing = Facing;
		using char8_t = unsigned char;
	}
}