#include "pch.h"
#include "CppUnitTest.h"
#include "../ktexlib3/导入头/texfile.h"
#include "../ktexlib_dymanic/TEXFileOperation.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ktexlib
{
	TEST_CLASS(V3Test)
	{
	public:
		
		TEST_METHOD(GenerateTest)
		{
			using namespace ktexlib::v3;
			using ktexlib::KTEXFileOperation::platfrm;
			using ktexlib::KTEXFileOperation::pixfrm;

			Assert::IsTrue(ktexlib::v3::gen_bc3universal(L"test.jpg"));
			
			ktexlib::KTEXFileOperation::KTEX reader;
			reader.LoadKTEX(L"test.tex");

			Assert::AreEqual(11ui16, reader.Info.mipscount, L"MIPSCOUNT != 11");
			Assert::AreEqual(platfrm::unk, reader.Info.platform);
			Assert::AreEqual(pixfrm::DXT5, reader.Info.pixelformat);
		}
	};
}
