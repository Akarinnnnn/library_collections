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
		///<summary>ͼƬ����</summary>
		enum class Facing : unsigned char
		{
			/// <summary>��</summary>
			right = 1,

			/// <summary>��</summary>
			up = 1 << 1,

			/// <summary>��</summary>
			left = 1 << 2,

			/// <summary>��</summary>
			down = 1 << 3,

			/// <summary>����</summary>
			upright = 1 << 4,

			/// <summary>����</summary>
			upleft = 1 << 5,

			/// <summary>����</summary>
			downright = 1 << 6,

			/// <summary>����</summary>
			downleft = 1 << 7,

			/// <summary>��������</summary>
			all90 = up | left | right | down,

			/// <summary>�ĸ���</summary>
			all45 = upright | upleft | downleft | downright,

			/// <summary>���г���</summary>
			alldir = 0xFF,

			/// <summary>�ⲻ���ܣ���</summary>
			invalid = 0
		};

		/// <summary>
		/// ������binͷ�Ĺ��ò���
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

			/// <summary>��ϣ���ַ�����</summary>
			std::vector<hashed> str_table;
		};

		/// <summary>
		/// build.bin
		/// </summary>
		struct Build :bin_base
		{
			///<summary>�Ϲ��ļ���CC4��BILD</summary>
			static constexpr unsigned int valid_cc4 = 0x444C4942;
			///<summary>��ǰ�汾</summary>
			static constexpr unsigned short cur_version = 0x0006;

			unsigned int symbol_count;
			unsigned int frame_count;
			u8string name;


		};

		/// <summary>
		/// ���Žڵ�
		/// </summary>
		struct sym_node
		{
			unsigned int hash;

		};

		/// <summary>
		/// �����ڵ�
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
		/// ����ڵ�
		/// </summary>
		struct frame_node
		{
			float x, y, w, h;

			std::vector<event_node> events;//event count + event
			std::vector<elem_node> elements;//elem count + elems
		};

		/// <summary>
		/// �¼��ڵ�
		/// </summary>
		struct event_node
		{
			/// <summary>��ϣ</summary>
			unsigned int hash; //??????
		};

		/// <summary>
		/// Ԫ�ؽڵ�
		/// </summary>
		struct elem_node
		{
			/// <summary>��ϣ</summary>
			unsigned int name_hash;
			unsigned int frame;
			unsigned int layer_hash;

			float a, b, c, d;
			float tx, ty;
			float z;
		};

		/// <summary>
		/// ��ϣ�����ַ���
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