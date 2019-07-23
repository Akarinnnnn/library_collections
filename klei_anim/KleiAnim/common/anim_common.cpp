#include "../../pch.h"
#include "anim_common.hpp"
#include <codecvt>
using std::string;
using std::exception;

unsigned int KleiAnim::Common::hash(std::string&& s)
{
	//������׼��Ĺ�ϣ��������Ϊ�����ص���size_t
	unsigned int hash = 0;
	for (const char v : s)
	{
		hash = (v + (hash << 6) + (hash << 16) - hash) & 0xFFFFFFFFU;
	}

	return hash;
}
