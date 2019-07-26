#include "../pch.h"
#include "Binary.hpp"
#include "common/exceptions.hpp"
#include <memory>
#include <thread>
#include <combaseapi.h>
#include <mutex>
#include <chrono>

using namespace KleiAnim::Binary;
using namespace KleiAnim;
using std::unique_ptr;
using std::string;
using HashedStringTable = std::map<unsigned int, string>;
using std::filesystem::path;

#define READ_INTO(x) (char*)&(x)

#ifndef KLEIANIM_USE_CHARLOG
#define LOG(str) L##str
#endif // KLEIANIM_USE_CHARLOG

//v1，将重构 
void mt_read_elem(const unsigned int count,
	std::vector<::KleiAnim::Common::ElementNode>& out,
	const std::filesystem::path& path,
	const size_t pos)
{
	using std::thread;
	using std::ios;
	std::mutex mtx_fin;
	std::mutex mtx_out;
	static const unsigned int t_limit = thread::hardware_concurrency();
	unsigned int finished = 0;
	for (unsigned int t_index = 0; t_index < t_limit; t_index++)
		//unsigned int t_index = 0;
	{
		thread(
			[&out, count, pos, path, &finished, &mtx_fin, &mtx_out]
		(unsigned int cur_tid)
			{
				thread_local std::ifstream file(path, ios::binary | ios::in);
				thread_local size_t begin_pos = pos + (40Ui64 * count * cur_tid) / t_limit;
				::KleiAnim::Common::ElementNode read_out{ 0,0,0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };

				file.seekg(begin_pos);
				//读取
				for (size_t i = 0; i < count / t_limit; i++)
				{
					file.read((char*)(&read_out), 40);
					mtx_out.lock();
					out.push_back(read_out);
					mtx_out.unlock();
				}

				file.close();

				mtx_fin.lock();
				finished++;
				mtx_fin.unlock();
			},
			t_index).detach();
	}

	while (finished != t_limit)
	{
		using namespace std::chrono;
		std::this_thread::sleep_for(100ms);
	}
}

AnimationReader::AnimationReader(std::filesystem::path& animpath) : 
	file(animpath, std::ios::binary | std::ios::in)
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
			anim.name = Common::read_str(file);
			file.read(READ_INTO(anim.facing), 1);

			if (anim.facing != Common::Facing::alldir)
			{
				KleiAnimLog::write() << LOG("警告：这不是一段全朝向的动画，可能会出现一些特殊的文件夹。");
			}

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

				file.read(READ_INTO(elem_count), 4);
				frame.elements.reserve(elem_count);

				anim.frames.push_back(frame);
				elem_total += elem_count;
				event_total += event_count;
			}

			frame_total += frame_count;

		}

		animations.push_back(anim);
	}

	str_table = Common::read_strhashtable(file);
}

BuildReader::BuildReader(std::filesystem::path& buildpath) : 
	file(buildpath, std::ios::binary | std::ios::in)
{
	file.read(READ_INTO(cc4), 6);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应提供正确的build.bin", cc4, version);
	}



	str_table = Common::read_strhashtable(file);
}
