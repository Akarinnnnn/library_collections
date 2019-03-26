#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Anim/Anim.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Animtest
{		
	TEST_CLASS(SCMLtest)
	{
	public:
		
		TEST_METHOD(SizeTest)
		{
			auto r1 = Anim::SCML::ReadSCML(L".\\Fa.scml");
			auto r2 = Anim::SCML::ReadSCML(L".\\m16a4.scml");
			auto r3 = Anim::SCML::ReadSCML(L".\\swap_m16a4.scml");//27ms
			Assert::AreEqual(12Ui64, r1.folder.size());
			Assert::AreEqual(1Ui64, r2.folder.size());
			Assert::AreEqual(1Ui64, r3.folder.size());
		}
		TEST_METHOD(SCMLReadTest)
		{
			auto m16 = Anim::SCML::ReadSCML(L".\\m16a4.scml");
			auto& entity = m16.entity[0];
			auto& anims = entity.animations;
			Assert::AreEqual(entity.name.c_str(), L"m16a4", false, L"Name´íÎó");
			Assert::AreEqual(entity.id, 0Ui32, L"id´íÎó");
		}
	};
}
