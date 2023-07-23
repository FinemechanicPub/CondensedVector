#include "pch.h"
#include "CppUnitTest.h"

#include "CondensedVector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MicrosoftTests
{
	TEST_CLASS(MicrosoftTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::vector<int> v = {5, 3, 0, 2};
			auto is_non_empty = [](int const& x) { return x != 0; };
			CondensedVector<int> cv(v, is_non_empty);
			Assert::AreEqual(static_cast<size_t>(std::count_if(v.begin(), v.end(), is_non_empty)), cv.Count(), L"Wrong non-empty element count");
			Assert::AreEqual(v.size(), cv.ExpandedSize(), L"Wrong total element count");
		}
	};
}
