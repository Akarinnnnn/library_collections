#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Anim/Anim.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Animtest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			auto r1 = Anim::SCML::ReadSCML(L".\\Fa.scml");
			auto r2 = Anim::SCML::ReadSCML(L".\\m16a4.scml");
			auto r3 = Anim::SCML::ReadSCML(L".\\swap_m16a4.scml");
			system("pause");
		}
	};
}