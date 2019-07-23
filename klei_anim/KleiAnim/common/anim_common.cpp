#include "../../pch.h"
#include "anim_common.hpp"
#include <codecvt>
using std::string;
using std::exception;

unsigned int KleiAnim::Common::hash(std::string&& s)
{
	//放弃标准库的哈希函数，因为它返回的是size_t
	unsigned int hash = 0;
	for (const char v : s)
	{
		hash = (v + (hash << 6) + (hash << 16) - hash) & 0xFFFFFFFFU;
	}

	return hash;
}
