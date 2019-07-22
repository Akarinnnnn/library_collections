#include "../../pch.h"
#include "exceptions.hpp"
#include <cstdio>

using namespace KleiAnim::Exception;
invalid_file::invalid_file() : std::exception("�ļ���ʽ����ȷ������ԭ��δ֪")
{

}

KleiAnim::Exception::invalid_file::invalid_file(const char* message)
{
	char formatted[32]{ 0 };
	sprintf_s(formatted, "�ļ���ʽ����ȷ��%s", message);
	invalid_file::exception(formatted);
}

invalid_file::invalid_file(const char* message, unsigned int cc4, unsigned short version) :cc4(cc4),version(version)
{
	char formatted[60]{ 0 };
	sprintf_s(formatted, "�ļ���ʽ����ȷ��%s��cc4 = 0x%08X,�汾Ϊ%d", message, cc4, version);
	invalid_file::exception(formatted);
}

invalid_file::~invalid_file()
{

}

unsigned int KleiAnim::Exception::invalid_file::get_cc4()
{
	return this->cc4;
}

unsigned int KleiAnim::Exception::invalid_file::get_version()
{
	return this->version;
}
