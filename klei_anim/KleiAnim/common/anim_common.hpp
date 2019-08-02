/***************************
* Encoding: GB2312
* KleiAnim�Ĺ��ò��֣������ṹ�嶨�弰���ú���
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
//ȷ��Ŀ�������С�˻�
//static_assert(std::endian::native == std::endian::little)


namespace KleiAnim
{
	namespace Common
	{
		/// <summary>
		/// ��Ҫ�ı�KleiAnim����־�����ֻ���ڵ���KleiAnim֮ǰ����һ��ʵ��
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
		/// ��Ҫ�ı�KleiAnim����־�����ֻ���ڵ���KleiAnim֮ǰ����һ��ʵ��
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
			all = 0xFFUi8,

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

		/// <summary>
		/// OpenGL����
		/// </summary>
		struct AlphaVertexNode
		{
			float x, y, z, u, v, w;
		};

		/// <summary>
		/// Build�е�֡�ڵ�
		/// </summary>
		struct BuildFrameNode
		{
			/// <summary>֡��</summary>
			unsigned int frame_number;

			/// <summary>����ʱ��</summary>
			unsigned int duration;

			/// <summary>bbox xywh����</summary>
			float x, y, w, h;


			unsigned int alpha_index;
			unsigned int alpha_count;
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
		struct EXPORT_API EventNode
		{
			/// <summary>���ƵĹ�ϣ</summary>
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
		/// Ԫ�ؽڵ�
		/// </summary>
		struct ElementNode
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
		/// Animation�е�֡�ڵ�
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

			/// <summary>����</summary>
			Facing facing;

			/// <summary>�����Ź�ϣ</summary>
			unsigned int rootsym_hash;

			/// <summary>֡��</summary>
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
		/// �����ַ����Ĺ�ϣֵ
		/// </summary>
		/// <param name="s">�������ϣ���ַ���</param>
		/// <returns>��ϣֵ</returns>
		/// <created>Fa��,2019/7/22</created>
		/// <changed>Fa��,2019/7/22</changed>
		unsigned int EXPORT_API hash(std::string&& s);

		/// <summary>
		/// ��ȡbin�ļ��е�һ���ַ������ǹ�ϣ���ַ�����
		/// </summary>
		/// <param name="f">���������н����ַ������ڵ�λ��</param>
		/// <returns>��ȡ�����ַ���</returns>
		/// <created>Fa��,2019/7/24</created>
		/// <changed>Fa��,2019/7/24</changed>
		std::string read_str(std::istream& f);

		/// <summary>
		/// ��ȡ��ϣ�����ַ�����
		/// </summary>
		/// <param name="f">�ļ����������н����ַ���������λ��</param>
		/// <returns>�ַ�����</returns>
		/// <created>Fa��,2019/7/24</created>
		/// <changed>Fa��,2019/7/24</changed>
		std::map<unsigned int, std::string> read_strhashtable(std::ifstream& f);

		/// <summary>
		/// ��ȡԪ�ؽڵ���ַ���(<see cref="std::wstring"/>)��ʾ����֧��wstring
		/// </summary>
		/// <param name="elem">Ԫ�ؽڵ�</param>
		/// <returns>����<see cref="std::wstring"/>���������std::moveʹ���Ա��⹹�쿪��</returns>
		/// <example><c>wstring elem_str = std::move(ToString(elem));</c></example>
		/// <created>Fa��,2019/7/27</created>
		/// <changed>Fa��,2019/7/27</changed>
		std::wstring EXPORT_API ToString(const ElementNode& elem);

		/// <summary>
		/// �Ƚ�����ElementNode�Ƿ����
		/// </summary>
		/// <param name="l">1</param>
		/// <param name="r">2</param>
		/// <returns>��ȷ���true������ȷ���false</returns>
		/// <created>Fa��,2019/7/27</created>
		/// <changed>Fa��,2019/7/27</changed>
		bool EXPORT_API operator==(const ElementNode& l, const ElementNode& r);
	}
}