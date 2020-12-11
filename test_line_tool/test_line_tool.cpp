#include "pch.h"
#include "CppUnitTest.h"
#include "../line_tool/line_tool.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testlinetool
{
	TEST_CLASS(testlinetool)
	{
	public:
		
		TEST_METHOD(test__file_open)
		{
			FILE* fp_input;
			char errmsgbuf[100];
			int err;
			fp_input = _file_open("../../test_line_tool/testdata/a.txt", "r", errmsgbuf, sizeof(errmsgbuf), &err);
			Assert::AreEqual(0, err);
		}

		TEST_METHOD(test__remove_newline_true)
		{
			char text[100] = "test line with newline at end\n";
			bool res = remove_newline(text);
			Assert::AreEqual(true, res);
		}
		TEST_METHOD(test__remove_newline_functionality)
		{
			char text[100] = "test line with newline at end\n";
			unsigned int len_prev = strlen(text);
			bool res = remove_newline(text);
			if (res == true) {
				Assert::AreNotEqual('\n', text[strlen(text) - 1]);
				Assert::AreEqual(len_prev, strlen(text) + 1);
			}
		}
		TEST_METHOD(test__remove_newline_false)
		{
			char text[100] = "test line with newline at end";
			bool res = remove_newline(text);
			Assert::AreEqual(false, res);
		}
		TEST_METHOD(test__remove_newline_emtpy)
		{
			char text[100] = "";
			bool res = remove_newline(text);
			Assert::AreEqual(false, res);
		}
		TEST_METHOD(test__remove_newline_NULL)
		{
			bool res = remove_newline(NULL);
			Assert::AreEqual(false, res);
		}
	};
}
