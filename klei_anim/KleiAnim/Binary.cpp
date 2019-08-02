#include "../pch.h"
#include "Binary.hpp"
#include "common/exceptions.hpp"
#include <memory>
#include <thread>
#include <combaseapi.h>
#include <mutex>
#include <chrono>
#include <iostream>

using namespace KleiAnim::Binary;
using namespace KleiAnim;
using std::unique_ptr;
using std::string;
using HashedStringTable = std::map<unsigned int, string>;
using std::filesystem::path;
using std::ios;

#define READ_INTO(x) (char*)&(x)

#ifndef KLEIANIM_USE_CHARLOG
#define LOG(str) L##str
#else
#define LOG(str) str
#endif // KLEIANIM_USE_CHARLOG

#ifdef ENABLE_TIME_RECORD
#define PRINT_TIME std::cout<< "T = " << std::chrono::high_resolution_clock::now().time_since_epoch().count() << "ns" << std::endl
#endif // DEBUG


//还不如st_read_elem，不信看看测试
//留着以后看看能不能改进
//定义MT_READ_ELEM以使用这个sb函数
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

std::vector<KleiAnim::Common::ElementNode> st_read_elem(std::ifstream& f, unsigned int count)
{
	using KleiAnim::Common::ElementNode;
	std::vector<ElementNode> ret;
	ret.resize(count);
	f.read((char*)ret.data(), static_cast<size_t>(count) * 40Ui64);
	return ret;
}

AnimationReader::AnimationReader(const std::filesystem::path & animpath) : 
	file(animpath, std::ios::binary | std::ios::in)
{
	using std::cout;
	using std::endl;

	{
		std::error_code ec;
		if (!std::filesystem::exists(animpath, ec))
			throw std::system_error(ec, "文件不存在");
	}

	if (!file.is_open())
	{
		throw std::invalid_argument("打开失败");
	}

	file.read(READ_INTO(cc4), 8);

	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("anim.bin不正确", cc4, version);
	}

	alignas(16) struct { unsigned int elem = 0, frame = 0, event = 0, anim = 0; } info;

	file.read(READ_INTO(info), 16);

	unsigned int elem_total = 0, frame_total = 0, event_total = 0;

	animations.reserve(info.anim);
	for (size_t i_anim = 0; i_anim < info.anim; i_anim++)
	{
		Common::AnimationNode anim;
		//第一段
#ifdef ENABLE_TIME_RECORD
		cout << "i_anim = " << i_anim << ',';
		cout << "read build_name,facing,hashes" << endl;
		PRINT_TIME;
#endif
		{
			anim.name = Common::read_str(file);
			file.read(READ_INTO(anim.facing), 1);

			if (anim.facing != Common::Facing::all)
			{
				KleiAnimLog::write() << LOG("警告：这不是一段全朝向的动画，可能会出现一些特殊的文件夹。");
			}

			file.read(READ_INTO(anim.rootsym_hash), 4);
			file.read(READ_INTO(anim.frame_rate), sizeof(float));
		}
#ifdef ENABLE_TIME_RECORD
		cout << "finished,";
		PRINT_TIME;
		cout << "read frames" << endl;
#endif 

		//frame
		{
			unsigned int frame_count = 0;
			file.read(READ_INTO(frame_count), 4);
			anim.frames.reserve(frame_count);

			for (size_t i = 0; i < frame_count; i++)
			{
#ifdef ENABLE_TIME_RECORD
				cout << "frame i = " << i;
				PRINT_TIME;
#endif 
				Common::AnimationFrameNode frame;
				unsigned int event_count = 0, elem_count = 0;

				file.read(READ_INTO(frame.x), 4 * sizeof(float));

				file.read(READ_INTO(event_count), 4);

				frame.events.resize(event_count);
				file.read((char*)(frame.events.data()), size_t(4) * (size_t)event_count);

				file.read(READ_INTO(elem_count), 4);
				frame.elements.reserve(elem_count);

				#ifndef MT_READ_ELEM
				frame.elements = std::move(st_read_elem(this->file, elem_count));
				#else
				mt_read_elem(elem_count, frame.elements, animpath, file.tellg());
				#endif

				anim.frames.push_back(std::move(frame));
				elem_total += elem_count;
				event_total += event_count;
#ifdef ENABLE_TIME_RECORD
				cout << "finished.";
				PRINT_TIME;
#endif 
			}
			frame_total += frame_count;

		}
#ifdef ENABLE_TIME_RECORD
		cout << "read anim finished.";
		PRINT_TIME;
#endif 
		animations.push_back(std::move(anim));
#ifdef ENABLE_TIME_RECORD
		cout << "push_back";
		PRINT_TIME;
#endif 
	}
	str_table = Common::read_strhashtable(file);

	if (elem_total != info.elem || frame_total != info.frame || event_total != info.event)
	{
		KleiAnimLog::write() << LOG("警告：元素数量/帧数量/事件数量与实际不符。\n") <<
			LOG("类型 预期数量 实际数量\n") <<
			LOG("元素 ") << info.elem << ' ' << elem_total << endl <<
			LOG("帧   ") << info.frame << ' ' << frame_total << endl <<
			LOG("事件 ") << info.event << ' ' << event_total << endl;
	}
}

unsigned int KleiAnim::Binary::AnimationReader::anim_count() const
{
	return animations.size();
}

std::string KleiAnim::Binary::AnimationReader::de_hash(const unsigned int hash) const
{
	return str_table.at(hash);
}

const Common::AnimationNode& KleiAnim::Binary::AnimationReader::animation(const size_t i) const
{
	return animations[i];
}

const Common::AnimationNode& KleiAnim::Binary::AnimationReader::operator[](const size_t i) const
{
	return animations[i];
}

const Common::AnimationFrameNode& KleiAnim::Binary::AnimationReader::frame(const size_t anim, const size_t frame) const
{
	return animations[anim].frames[frame];
}

const std::vector<Common::EventNode>& KleiAnim::Binary::AnimationReader::events(const size_t anim, const size_t _frame) const
{
	return animations[anim].frames[_frame].events;
}

const std::vector<Common::ElementNode>& KleiAnim::Binary::AnimationReader::element_refs(const size_t anim, const size_t frame) const
{
	return animations[anim].frames[frame].elements;
}

BuildReader::BuildReader(const std::filesystem::path & buildpath) : 
	file(buildpath, std::ios::binary | std::ios::in)
{
	using std::cout;
	using std::endl;

	{
		std::error_code ec;
		if (!std::filesystem::exists(buildpath, ec))
			throw std::system_error(ec, "文件不存在");
	}

	if (!file.is_open())
	{
		throw std::invalid_argument("打开失败");
	}

	file.read(READ_INTO(cc4), 8);
	if (cc4 != valid_cc4 || version != cur_version)
	{
		throw Exception::invalid_file("应提供正确的build.bin", cc4, version);
	}
	
	file.read(READ_INTO(BuildBase::symbol_count), 8);
	build_name = std::move(Common::read_str(file));
	symbols.reserve(BuildBase::symbol_count);
	//atlas
	{
		unsigned int atlas_count = 0;
		file.read(READ_INTO(atlas_count), 4);
		atlases.reserve(atlas_count);
		for (unsigned int i = 0; i < atlas_count; i++)
		{
			atlases.push_back({ Common::read_str(file) });
		}
	}

	//symbol
	unsigned int frame_total = 0;
	{
		for (unsigned int i = 0; i < BuildBase::symbol_count; i++)
		{
			Common::SymbolNode symbol;
			Common::BuildFrameNode curframe;
			unsigned int cur_frame_count = 0;
			file.read(READ_INTO(symbol.name_hash), 4);
			file.read(READ_INTO(cur_frame_count), 4);

			for (unsigned int i = 0; i < cur_frame_count; i++)
			{
				file.read(READ_INTO(curframe), 32);
				symbol.frames.push_back(std::move(curframe));
			}
			frame_total += cur_frame_count;
			symbols.push_back(std::move(symbol));
		}
	}

	//alpha vertex
	{
		unsigned int vertex_count = 0;
		Common::AlphaVertexNode avn;
		file.read(READ_INTO(vertex_count), 4);
		BuildBase::vertices.reserve(vertex_count);
		for (unsigned int i = 0; i < vertex_count; i++)
		{
			file.read(READ_INTO(avn), 24);
			BuildBase::vertices.push_back(avn);
		}
	}

	str_table = Common::read_strhashtable(file);

	if (frame_total != frame_count)
	{
		KleiAnimLog::write() << LOG("警告：帧数量与实际不符。\n") <<
			LOG("预期数量 实际数量\n") <<
			frame_count << ' ' << frame_total << endl;
	}
}

std::vector<Common::SymbolNode>::const_iterator KleiAnim::Binary::BuildReader::begin() const
{
	return symbols.begin();
}

std::vector<Common::SymbolNode>::const_iterator KleiAnim::Binary::BuildReader::end() const
{
	return symbols.end();
}

unsigned int KleiAnim::Binary::BuildReader::symbol_count() const
{
	return symbols.size();
}

unsigned int KleiAnim::Binary::BuildReader::atlas_count() const
{
	return atlases.size();
}

unsigned int KleiAnim::Binary::BuildReader::vertex_count() const
{
	return BuildBase::vertices.size();
}

std::string KleiAnim::Binary::BuildReader::name() const
{
	return build_name;
}

const Common::SymbolNode& KleiAnim::Binary::BuildReader::symbol(const size_t i) const
{
	return symbols.at(i);
}

const Common::SymbolNode& KleiAnim::Binary::BuildReader::operator[](const size_t i) const
{
	return symbols[i];
}

const Common::AtlasNode& KleiAnim::Binary::BuildReader::atlas(const size_t i) const
{
	return atlases.at(i);
}

const Common::AlphaVertexNode& KleiAnim::Binary::BuildReader::vertex(const size_t i) const
{
	return BuildBase::vertices.at(i);
}

std::array<Common::AlphaVertexNode, 6> KleiAnim::Binary::BuildReader::vertices(const unsigned int start) const
{
	if (BuildBase::vertices.size() < (start * 6 + 6))
		throw std::out_of_range("并没有这么多组");

	std::array<Common::AlphaVertexNode, 6> ret;
	auto beg = BuildBase::vertices.data() + 6 * start;
	auto d = ret.data();
	for (unsigned int i = 0; i < 6; i++)
		d[i] = beg[i];
	return ret;
}

const Common::BuildFrameNode& KleiAnim::Binary::BuildReader::frame(const size_t sym, const size_t i) const
{
	return symbols.at(sym).frames.at(i);
}

KleiAnim::Binary::AnimationWriter::AnimationWriter(const std::filesystem::path& out) :output(std::ofstream(out, ios::binary | ios::trunc))
{
	
}

KleiAnim::Binary::AnimationWriter::AnimationWriter(const std::filesystem::path& out, AnimationBase& base):
	output(std::ofstream(out, ios::binary | ios::trunc)),
	AnimationBase(base)
{
}

KleiAnim::Binary::AnimationWriter::~AnimationWriter()
{
	
}

void KleiAnim::Binary::AnimationWriter::writefile()
{
	output.write("ANIM", 4);
	output.write((char*)(&AnimationBase::cur_version), 4);


}

void KleiAnim::Binary::AnimationWriter::add(Common::AnimationNode& anim)
{
	animations.push_back(anim);
}

KleiAnim::Binary::BuildWriter::BuildWriter(const std::filesystem::path& out) :output(std::ofstream(out, ios::binary | ios::trunc))
{
	cc4 = valid_cc4;
	version = cur_version;
}

KleiAnim::Binary::BuildWriter::~BuildWriter()
{
}
