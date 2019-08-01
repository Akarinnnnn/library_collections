#include "../../pch.h"
#include "anim_common.hpp"
#include <codecvt>
#include <iostream>

using std::string;
using std::exception;

#define READ_INTO(x) (char*)&(x) 

unsigned int KleiAnim::Common::hash(std::string&& s)
{
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
		file.read(READ_INTO(size), 4);//这里MSVC debug x64的对齐策略导致size和hash不连续
		file.read(READ_INTO(hash), 4);
		string str(size, '\0');
		file.read(str.data(), size);

		ret.insert(std::make_pair(hash, str));
	}
	return ret;
}

std::wstring KleiAnim::Common::ToString(const ElementNode& elem)
{
	std::wostringstream _s;
	_s << L"Name Hash = " << elem.name_hash << '\n'
		<< L"frame = " << elem.frame << '\n'
		<< L"uv bounds: " << elem.a << ' ' << elem.b << ' ' << elem.c << ' ' << elem.d << '\n'
		<< L"tx ty: " << elem.tx << ' ' << elem.ty
		<< L"Z index = " << elem.z;
	return _s.str();
}

bool KleiAnim::Common::operator==(const ElementNode& l, const ElementNode& r)
{
	using namespace KleiAnim::Common;
#ifdef _AMD64_
	//这么大一个寄存器不用白不用
	for (unsigned char i = 0; i < 5; i++)
		if (reinterpret_cast<unsigned long long*>(const_cast<ElementNode*>(&l))[i] != reinterpret_cast<unsigned long long*>(const_cast<ElementNode*>(&r))[i])
			return false;
#elif _X86_
	for (unsigned char i = 0; i < 10; i++)
		if (reinterpret_cast<unsigned int*>(const_cast<ElementNode*>(&l))[i] != reinterpret_cast<unsigned int*>(const_cast<ElementNode*>(&r))[i])
			return false;
#endif
	return true;
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
	try
	{
		stream->imbue(std::locale("chs"));//忽略C26444，std::locale有不同的析构逻辑
	}
	catch (const std::runtime_error& e)
	{
		stream->imbue(std::locale("C"));
		std::cerr << e.what() << ",imbued \"C\" locale" << std::endl;
	}
	in_use ? in_use : in_use = this;
}

std::wostream& KleiAnim::Common::WideCharLog::write()
{
	return *in_use->stream;
}
