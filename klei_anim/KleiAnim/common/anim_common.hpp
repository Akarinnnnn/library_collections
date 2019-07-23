/***************************
* Encoding: GB2312
* KleiAnim�Ĺ��ò��֣������ṹ�嶨�弰���ú���
***************************/
#pragma once

#include <string>
#include <vector>
#include <typeinfo>
#include <map>
#include "NoConstructAllocator.h"
//#include <type_traits>
//assert size
static_assert(sizeof(unsigned int) == 4);
static_assert(sizeof(unsigned short) == 2);

//assert LE(C++ 20)
//ȷ��Ŀ�������С�˻�
//static_assert(std::endian::native == std::endian::little)

//static_assert(sizeof(unsigned long long) == 8);

namespace KleiAnim
{
	namespace Common
	{
		///<summary>
		///ͼƬ����
		///</summary>
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
		/// ������bin�ļ��Ĺ��ò���
		/// </summary>
		struct BinaryFileBase
		{
			unsigned int cc4 = 0;
			signed int version = 0;

			/// <summary>��ϣ���ַ�����</summary>
			std::map<unsigned int, std::string> str_table;
		};



		struct Vertex
		{
			float x, y, z, u, v, w;
		};

		/// <summary>
		/// Build�еĻ���ڵ�
		/// </summary>
		struct BuildFrameNode
		{
			/// <summary>���</summary>
			unsigned int sn;
			/// <summary>���</summary>
			unsigned int duration;

			/// <summary>bbox����</summary>
			float x, y, w, h;


			unsigned int vert_index;

			unsigned int vert_count;
		};

		/// <summary>
		/// ���Žڵ�
		/// </summary>
		struct SymbolNode
		{
			unsigned int name_hash;
			std::vector<BuildFrameNode> frames;
		};

		/// <summary>
		/// �¼��ڵ�
		/// </summary>
		struct EventNode
		{
			/// <summary>���ƵĹ�ϣ</summary>
			unsigned int name_hash;

			operator unsigned int()
			{
				return name_hash;
			}

			EventNode(unsigned int h)
			{
				name_hash = h;
			}
		};

		/// <summary>
		/// Ԫ�ؽڵ�
		/// </summary>
		struct ElementNode
		{
			/// <summary>��ϣ</summary>
			unsigned int name_hash;
			unsigned int frame;
			unsigned int layer_hash;

			float u1, u2, v1, v2;
			float tx, ty;
			float z;
		};



		/// <summary>
		/// Animation�еĻ���ڵ�
		/// </summary>
		struct AnimationFrameNode
		{
			float x, y, w, h;

			std::vector<EventNode> events;//event count + event
			std::vector<ElementNode> elements;//elem count + elems
		};

		/// <summary>
		/// �����ڵ�
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
		/// �����ַ����Ĺ�ϣֵ
		/// </summary>
		/// <param name="s">�������ϣ���ַ���</param>
		/// <returns>��ϣֵ</returns>
		/// <created>Fa��,2019/7/22</created>
		/// <changed>Fa��,2019/7/22</changed>
		unsigned int hash(std::string&& s);
	}
}