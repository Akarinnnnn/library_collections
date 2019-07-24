#include "../../pch.h"
#include "anim_common.hpp"
#include <codecvt>
#include <iostream>

using std::string;
using std::exception;

#define READ_INTO(x) (char*)&(x) 

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

string KleiAnim::Common::read_str(std::istream& f)
{
	unsigned int strsize;
	f.read(READ_INTO(strsize), 4);
	char* c_name = new char[strsize]{ 0 };
	f.read(c_name, strsize);
	string ret(c_name, c_name + strsize);
	delete[] c_name;
	return ret;
}

std::map<unsigned int,string> KleiAnim::Common::read_strhashtable(std::ifstream& file)
{
	std::map<unsigned int, string> ret;
	while (!file.eof())
	{
		unsigned int size = 0, hash = 0;
		file.read(READ_INTO(size), 8);

		string str(size, '\0');
		file.read(str.data(), size);

		ret.insert(std::make_pair(hash, str));
	}
	return ret;
}

KleiAnim::Common::CharLog::CharLog(std::ostream& output)
{
	stream = &output;
	in_use ? in_use : in_use = this;
}

std::ostream& KleiAnim::Common::CharLog::write()
{
	return *in_use->stream;
}

KleiAnim::Common::WideCharLog::WideCharLog(std::wostream& output)
{
	stream = &output;
	in_use ? in_use : in_use = this;
}

std::wostream& KleiAnim::Common::WideCharLog::write()
{
	return *in_use->stream;
}
