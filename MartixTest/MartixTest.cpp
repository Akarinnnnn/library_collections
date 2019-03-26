#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Anim/Martix22.h"
#include "../Anim/Martix33.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MartixTest
{		
	TEST_CLASS(MartixTest)
	{
	public:
		Anim::Martix22 m22;
		Anim::Martix33 m33;
		TEST_METHOD(m33scale)
		{
			auto ret = m33.scale(2.0);
			Assert::AreEqual(2.0f, ret.data[0][0], 0.1f);
			Assert::AreEqual(2.0f, ret.data[1][1], 0.1f);
		}
		TEST_METHOD(m33scalethis)
		{
			m33.scale(2.0);
			Assert::AreEqual(2.0f, m33.data[0][0], 0.1f,L"00");
			Assert::AreEqual(2.0f, m33.data[1][1], 0.1f,L"11");
		}
	};
}