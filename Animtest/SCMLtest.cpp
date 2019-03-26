#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Anim/Anim.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Animtest
{		
	TEST_CLASS(SCMLtest)
	{
	public:
		
		TEST_METHOD(ReadTest)
		{
			auto r1 = Anim::SCML::ReadSCML(L".\\Fa.scml");
			auto r2 = Anim::SCML::ReadSCML(L".\\m16a4.scml");
			auto r3 = Anim::SCML::ReadSCML(L".\\swap_m16a4.scml");
			Assert::AreEqual(14Ui64, r1.folder.size());
			Assert::AreEqual(1Ui64, r1.folder.size());
			Assert::AreEqual(1Ui64, r1.folder.size());
		}
	};
}
/*int main()
{
	Animtest::SCMLtest().ReadTest();
}*/