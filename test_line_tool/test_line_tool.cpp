#include "pch.h"
#include "CppUnitTest.h"
#include "../line_tool/line_tool.cpp""

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testlinetool
{
	TEST_CLASS(testlinetool)
	{
	public:
		
		TEST_METHOD(fileExists)
		{
			char errmsgbuf[100];
			int rv = file_open("../../test_line_tool/testdata/a.txt", "r", &fp_input, errmsgbuf, sizeof(errmsgbuf));
			Assert::AreEqual(0, rv);
		}
	};
}
