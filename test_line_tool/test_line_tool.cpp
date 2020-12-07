#include "pch.h"
#include "CppUnitTest.h"
#include "../line_tool/line_tool.cpp""

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

		TEST_METHOD(test_open_files)
		{
			FILE* fp_input;
			FILE* fp_output;
			char* fname_i = "../../test_line_tool/testdata/a.txt";
			char* fname_o = "";
			Assert::AreEqual(0, open_files(fname_i, fname_o, &fp_input, &fp_output));
		}

		TEST_METHOD(test_parse_parameters)
		{
			char* n_argv[] = { "param0", "param1", "param2" };
			Assert::AreEqual(0, parse_parameters(2, n_argv));
		}

		TEST_METHOD(test_set_mode_string)
		{
			char mode_str[100] = "\0";
			Assert::AreNotEqual('\n', set_mode_string(0)[0]);
		}

		TEST_METHOD(test_print_header)
		{
			char* pb_line_prefix = "[PBTOOL - lines]";
			char* fname_i = "infile";
			char* fname_o = "outfile";
			char* mode_str = "mode";
			int empty = 0;
			Assert::AreEqual(0, print_header(pb_line_prefix, fname_i, fname_o, mode_str, empty));
		}
	};
}
