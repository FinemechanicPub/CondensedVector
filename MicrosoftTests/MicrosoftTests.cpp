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

	std::wstring message_for_testcase(
		const std::wstring& message,
		const std::vector<int>& testcase,
		const std::vector<int>& expected,
		const std::vector<int>& actual) 
	{
		return message + L" Test case: " + vector_to_string(testcase)
			+  L" Expected: " + vector_to_string(expected) 
			+ L", Actual: " + vector_to_string(actual);
	}

	TEST_CLASS(MicrosoftTests)
	{
	public:
		
		TEST_METHOD(CompareVectors)
		{
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
				CondensedVector<int> cv(v, non_zero);
				Assert::AreEqual(static_cast<size_t>(std::count_if(v.begin(), v.end(), non_zero<int>)), cv.Count(), message_for_testcase(L"Wrong non-empty element count", v).c_str());				
				for (size_t i = 0; i < v.size(); ++i) {
					if (!non_zero(v[i])) continue;
					Assert::AreEqual(v[i], cv.At(i), message_for_testcase(L"Wrong element at position " + i, v).c_str());
				}
				auto nv = cv.ToVector(v.size());
				Assert::AreEqual(v.size(), nv.size(), message_for_testcase(L"ToVector produced vector of wrong length", v).c_str());
				for (size_t i = 0; i < v.size(); ++i) {
					Assert::AreEqual(v[i], nv[i], message_for_testcase(L"Wrong ToVector result", v).c_str());
				}
				nv.clear();
				cv.MoveToVector(nv, v.size());
				Assert::AreEqual(v.size(), nv.size(), message_for_testcase(L"MoveToVector produced vector of wrong length", v).c_str());
				for (size_t i = 0; i < v.size(); ++i) {
					Assert::AreEqual(v[i], nv[i], message_for_testcase(L"Wrong MoveToVector result", v).c_str());
				}
			}

		}
		TEST_METHOD(InsertRemoveElements) {
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
			for (const auto& testcase : testcases) {
				auto v = testcase;
				CondensedVector<int> cv(v, non_zero);
				cv.Insert(0, 5);
				v.insert(v.begin(), 5, 0);
				Assert::IsTrue(v == cv.ToVector(v.size()), message_for_testcase(std::wstring(L"Insert into beginning error."), testcase, v, cv.ToVector(v.size())).c_str());
			}
			for (const auto& testcase : testcases) {
				auto v = testcase;
				CondensedVector<int> cv(v, non_zero);
				cv.Insert(v.size(), 5);
				v.insert(v.end(), 5, 0);
				Assert::IsTrue(v == cv.ToVector(v.size()), message_for_testcase(std::wstring(L"Append to end error."), testcase, v, cv.ToVector(v.size())).c_str());
			}
			for (const auto& testcase : testcases) {
				auto v = testcase;
				CondensedVector<int> cv(v, non_zero);
				cv.Delete(0, 5);
				v.erase(v.begin(), v.begin() + 5);
				Assert::IsTrue(v == cv.ToVector(v.size()), message_for_testcase(std::wstring(L"Delete from the beginning error."), testcase, v, cv.ToVector(v.size())).c_str());
			}
			for (const auto& testcase : testcases) {
				auto v = testcase;
				CondensedVector<int> cv(v, non_zero);
				cv.Delete(testcase.size() - 5, 5);
				v.erase(v.end() - 5, v.end());
				Assert::IsTrue(v == cv.ToVector(v.size()), message_for_testcase(std::wstring(L"Delete from the end error."), testcase , v, cv.ToVector(v.size())).c_str());
			}
		}

		TEST_METHOD(ConstructWithPut) {
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
			for (const auto& v : testcases) {
				CondensedVector<int> cv;
				for (size_t i = 0; i < v.size(); ++i) {
					if (non_zero(v[i])) cv.Put(i, v[i]);
				}				
				Assert::IsTrue(v == cv.ToVector(v.size()), message_for_testcase(std::wstring(L"Error when constructing with Put."), v, v, cv.ToVector(v.size())).c_str());
			}
		}
	};
}
