#include "pch.h"

#include <iostream>
#include <numeric>
#include <vector>

#include "CondensedVector.h"


int read_me() {
	{
		std::vector v = { 1.0, 0.0, 1.5 };
		cvn::CondensedVector cv(v, cvn::non_zero);

		if (v != cv.ToVector(v.size())) return 1;
	}
	{
		cvn::CondensedVector<double> cv;
		cv.Put(3, 1.0);
		cv.Put(5, 2.0);
		std::vector v = { 0.0, 0.0, 0.0, 1.0, 0.0, 2.0 };
		std::cout << (v == cv.ToVector());

		if (v != cv.ToVector()) return 1;
	}
	{
		cvn::CondensedVector<double> cv;
		cv[3] = 1.0;
		cv[5] = 2.0;
		std::vector v = { 0.0, 0.0, 0.0, 1.0, 0.0, 2.0 };
		std::cout << (v == cv.ToVector());

		if (v != cv.ToVector()) return 1;
	}
	{
		std::vector v = { 1.0, 0.0, 1.5 };
		cvn::CondensedVector cv(v, cvn::non_zero);
		v[2] *= 3;
		auto* ptr = cv.GetPointer(2);
		*(ptr) *= 3;
		std::cout << (v == cv.ToVector());

		if (v != cv.ToVector(v.size())) return 1;
	}
	{
		std::vector v = { 1.0, 0.0, 1.5 };
		cvn::CondensedVector cv(v, cvn::non_zero);
		v.insert(v.begin() + 2, { 0.0, 0.0, 0.0 });
		cv.Insert(2, 3);
		std::cout << (v == cv.ToVector());

		if (v != cv.ToVector(v.size())) return 1;
	}
	{
		std::vector v = { 1.0, 0.0, 1.5 };
		cvn::CondensedVector cv(v, cvn::non_zero);
		v.erase(v.begin() + 1, v.begin() + 3);
		cv.Delete(1, 2);
		std::cout << (v == cv.ToVector());

		if (v != cv.ToVector(v.size())) return 1;
	}
	{
		{
			std::vector v = { 1.5, 0.0, 1.0 };
			cvn::CondensedVector cv(v, cvn::non_zero);
			std::cout << *std::max_element(cv.begin(), cv.end()) << std::endl;
			std::cout << std::accumulate(cv.begin(), cv.end(), 0.0) << std::endl;

			if (*std::max_element(v.begin(), v.end()) != *std::max_element(cv.begin(), cv.end())) return 1;
			if (std::accumulate(v.begin(), v.end(), 0.0) != std::accumulate(cv.begin(), cv.end(), 0.0)) return 1;
		}
	}

	return 0;
}