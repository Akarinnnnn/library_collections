#include "../pch.h"
#include "Binary.hpp"
#include "common/exceptions.hpp"
#include <memory>
using namespace KleiAnim::Binary;
using namespace KleiAnim;
using std::unique_ptr;
using std::string;
using HashedStringTable = std::map<unsigned int, string>;

#define READ_INTO(x) (char*)&(x) 

string read_str(std::istream& f)
{
	unsigned int strsize;
	f.read(READ_INTO(strsize), 4);
	unique_ptr<char[]> c_name(new char[strsize + 1] {0});
	f.read(c_name.get(), strsize);
	return string(c_name.get());
}

HashedStringTable read_strhashtable(std::ifstream& file)
{
	std::map<unsigned int, string> ret;
}

AnimationReader::AnimationReader(std::filesystem::path& animpath) : file(animpath)
{
	file.read(READ_INTO(cc4), 6);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应提供正确的anim.bin", cc4, version);
	}

	struct { unsigned int sym = 0, frame = 0, event = 0, anim = 0; } info;

	file.read(READ_INTO(info), 16);

	unsigned int elem_total = 0, frame_total = 0, event_total = 0;

	animations.reserve(info.anim);
	for (size_t i_anim = 0; i_anim < info.anim; i_anim++)
	{
		Common::AnimationNode anim;
		//第一段
		{
			anim.name = read_str(file);
			file.read(READ_INTO(anim.facing), 1);
			file.read(READ_INTO(anim.rootsym_hash), 4);
			file.read(READ_INTO(anim.frame_rate), sizeof(float));
		}
		//frame
		{
			unsigned int frame_count = 0;
			file.read(READ_INTO(frame_count), 4);
			anim.frames.reserve(frame_count);

			for (size_t i = 0; i < frame_count; i++)
			{
				Common::AnimationFrameNode frame;
				unsigned int event_count = 0, elem_count = 0;

				file.read(READ_INTO(frame.x), 4 * sizeof(float));

				file.read(READ_INTO(event_count), 4);

				frame.events.resize(event_count);
				file.read((char*)(frame.events.data()), 4 * event_count);

				anim.frames.push_back(frame);
			}

			frame_total += frame_count;
			animations.push_back(anim);
		}

		
	}
}

BuildReader::BuildReader(std::filesystem::path& buildpath): file(buildpath)
{
	file.read(READ_INTO(cc4), 6);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应提供正确的build.bin", cc4, version);
	}
}
