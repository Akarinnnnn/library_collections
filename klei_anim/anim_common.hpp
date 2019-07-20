#pragma once
//#include <type_traits>
//assert size
static_assert(sizeof(unsigned int) == 4);
static_assert(sizeof(unsigned short) == 2);

//assert LE(C++ 20)
//static_assert(std::endian::native == std::endian:little)

//static_assert(sizeof(unsigned long long) == 8);

namespace KleiAnim
{
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

		//static_assert(sizeof(bin_base) == 8);

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
		};

		//Aliases
		using facing = Facing;
	}
}