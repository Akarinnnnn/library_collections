/***************************
* Encoding: GB2312
***************************/
#include "pch.h"
#include "CppUnitTest.h"

#include "../klei_anim/KleiAnim/common/anim_common.hpp"
#include <filesystem>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <charconv>
static_assert(sizeof(unsigned long long) == 8);
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ktexlib_test
{
	namespace CompareUnion
	{
		union cmp_elemnode
		{
			unsigned long long a[5];
			::KleiAnim::Common::ElementNode elem;
		};
	}

	bool operator==(KleiAnim::Common::ElementNode& l, KleiAnim::Common::ElementNode& r)
	{
		for (unsigned char i = 0; i < 5; i++)
			if (reinterpret_cast<unsigned long long*>(&l)[i] != reinterpret_cast<unsigned long long*>(&r)[i]) 
				return false;

		return true;
	}

	TEST_CLASS(KleiAnimTest)
	{
	public:

		TEST_METHOD(MultiThreadElementRead)
		{
			using namespace ::KleiAnim::Common;
			//small:28 elem refs,28 frame, 0 event, 1 anim
			//anim #1: idle, facing: all, hash:20 00 00 EE 
			//frame count: 28,
			std::vector<ElementNode> actual;
			actual.reserve(20);

			//generated by TestElemGen.exe
			ElementNode excepted[]
			{
				{ 1,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 2,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 3,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 4,1,10000,0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f },
				{ 5,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 6,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 7,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 8,1,10000,1.1f,1.2f,1.3f,1.4f,1.5f,1.6f,1.7f },
				{ 9,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 10,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 11,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 12,1,10000,2.1f,2.2f,2.3f,2.4f,2.5f,2.6f,2.7f },
				{ 13,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 14,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 15,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 16,1,10000,3.1f,3.2f,3.3f,3.4f,3.5f,3.6f,3.7f },
				{ 17,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 18,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 19,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
				{ 20,1,10000,4.1f,4.2f,4.3f,4.4f,4.5f,4.6f,4.7f },
			};

			//std::ifstream fakebin(, std::ios::binary | std::ios::in);
			mt_read_elem(20, actual, "./elem_mtread.fakebin", 0);

			Assert::AreEqual(20Ui64,actual.size(),L"Size not match");

			std::sort(actual.begin(), actual.end(), [](const ElementNode& l,const ElementNode& r) -> bool
				{
					return l.name_hash < r.name_hash;
				});

			for (size_t i = 0; i < 20Ui64; i++)
			{
				Assert::IsTrue(excepted[i] == actual[i], L"Still not match after sorted");
			}
		}
	private:

		bool Contains(std::vector<KleiAnim::Common::ElementNode>& actual, KleiAnim::Common::ElementNode& excepted_elem)
		{
			for (auto& actual_elem : actual)
				if(excepted_elem == actual_elem)
					return true;

			return false;
		}

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
						//��ȡ
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
	};
}