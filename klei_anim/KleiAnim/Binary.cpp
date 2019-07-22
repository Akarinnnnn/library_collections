#include "../pch.h"
#include "Binary.hpp"
#include "Shared/exceptions.hpp"
using namespace KleiAnim::Binary;
using namespace KleiAnim;

#define READ_INTO(x) (char*)&(x) 

AnimationReader::AnimationReader(std::filesystem::path& animpath) : file(animpath)
{
	file.read(READ_INTO(cc4), 6);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应输入正确的anim.bin", cc4, version);
	}
}

BuildReader::BuildReader(std::filesystem::path& buildpath): file(buildpath)
{
	file.read(READ_INTO(cc4), 6);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应输入正确的build.bin", cc4, version);
	}

}
