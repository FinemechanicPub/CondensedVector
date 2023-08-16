#include "pch.h"
#include "CppUnitTest.h"

#include "CondensedVector.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MicrosoftTests
{
	std::wstring vector_to_string(const std::vector<int>& v) {
		std::wostringstream ss;
		ss << '[';
		bool flag = false;
		for (const auto& item : v) {
			if (flag) {
				ss << ", ";
			}
			flag = true;
			ss << item;
		};
		ss << ']';
		return ss.str();
	}

	std::wstring message_for_testcase(const std::wstring& message, const std::vector<int>& testcase) {
		return message + L" for test case " + vector_to_string(testcase);
	}

	TEST_CLASS(MicrosoftTests)
	{
	public:
		
		TEST_METHOD(CompareVectors)
		{
			auto is_non_empty = [](const int& x) {
				return x != 0; 
			};
			std::vector<std::vector<int>> testcases = {
				{0, 0, 0, 0, 0, 0, 0},
				{0, 5, 5, 5, 5, 5, 5},
				{5, 5, 5, 5, 5, 5, 0},
				{5, 5, 5, 5, 5, 5, 5},
				{1, 3, 5, 7, 9, 11, 13},
				{0, 1, 3, 5, 7, 9, 11, 13},
				{1, 0, 3, 5, 7, 9, 11, 13},
				{1, 3, 5, 7, 9, 11, 0, 13},
				{1, 3, 5, 7, 9, 11, 13, 0},
				{1, 3, 5, 0, 0, 7, 9, 11, 13},
				{0, 0, 6, 8, 0, 3, 5, 0}
			};
			for (const auto& v: testcases) {
				CondensedVector<int> cv(v, is_non_empty);
				Assert::AreEqual(static_cast<size_t>(std::count_if(v.begin(), v.end(), is_non_empty)), cv.Count(), message_for_testcase(L"Wrong non-empty element count", v).c_str());				
				for (size_t i = 0; i < v.size(); ++i) {
					if (!is_non_empty(v[i])) continue;
					Assert::AreEqual(v[i], cv.At(i), message_for_testcase(L"Wrong element at position " + i, v).c_str());
				}
				auto nv = cv.ToVector(v.size());
				for (size_t i = 0; i < v.size(); ++i) {
					Assert::AreEqual(v[i], nv[i], message_for_testcase(L"Wrong ToVector result", v).c_str());
				}
			}

		}
	};
}
