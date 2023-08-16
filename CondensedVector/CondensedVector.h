#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <optional>
#include <cassert>
#include <limits>

template<class T, class I = int>
class CondensedVector {
public:
	struct Element{
		I idx;
		T value;
	};

private:
	struct ElementComp {
		bool operator() (const Element& e, I i) { return e.idx < i; }
		bool operator() (I i, const Element& e) { return i < e.idx; }
	};

	I from_sizet(size_t arg) {
		assert(static_cast<size_t>(std::numeric_limits<I>::max()) >= arg);
		return static_cast<I>(arg); 
	}

	size_t to_sizet(I arg) {
		assert(arg >= 0);		
		return static_cast<size_t>(arg);
	}

	std::vector<Element> elements;


	

public:
	CondensedVector() { }
	CondensedVector(std::vector<T> v);
	CondensedVector(std::vector<T> v, bool(*is_not_empty)(const T&));
	CondensedVector(CondensedVector&& other) noexcept {
		std::swap(elements, other.elements);
	}
	CondensedVector& operator=(CondensedVector&& other) noexcept {		
		std::swap(elements, other.elements);
		return *this;
	}
	
	T& operator[](I i);	
	T& At(I i);
	T* GetPointer(I i);
	T* Put(I i, T v);
	bool Contains(I i) { return GetPointer(i); }
	void Insert(I before, I count);
	void Delete(I first, I count);
	size_t Count() { return elements.size(); }
	void Reserve(size_t count) { elements.reserve(count); }
	std::optional<I> MaxIndex() { return elements.empty() ? std::nullopt : std::make_optional(elements.back().idx); }
	size_t ExpandedSize() { if (elements.empty()) { return 0; } else { return to_sizet(MaxIndex()) + 1; } }		
	void MoveToVector(std::vector<T>& v, I length = I(), I origin = I());
	std::vector<T> ToVector(I length = I(), I origin = I());

	auto begin() { return elements.begin(); }
	auto end() { return elements.end(); }
	auto begin() const { return elements.begin(); }
	auto end() const  { return elements.end(); }
	auto rbegin() { return elements.rbegin(); }
	auto rend() { return elements.rend(); }
	auto rbegin() const { return elements.rbegin(); }
	auto rend() const { return elements.rend(); }
	auto index_upper_bound(I index) { return std::upper_bound(elements.begin(), elements.end(), index, ElementComp{}); }
	auto index_lower_bound(I index) { return std::lower_bound(elements.begin(), elements.end(), index, ElementComp{}); }
	auto index_equal_range(I index) { return std::equal_range(elements.begin(), elements.end(), index, ElementComp{}); }
};




template<class T, class I>
CondensedVector<T, I>::CondensedVector(std::vector<T> v)
{
	elements.reserve(v.size());
	for (I i = 0; i < from_sizet(v.size()); ++i) {
		elements.push_back({ i, std::move(v[i]) });
	}
}

template<class T, class I>
CondensedVector<T, I>::CondensedVector(std::vector<T> v, bool(*is_not_empty)(const T&))
{
	for (I i = 0; i < from_sizet(v.size()); ++i) {
		if (is_not_empty(v[i])) {
			elements.push_back({ i, std::move(v[i]) });
		}		
	}
}

template<class T, class I>
inline T& CondensedVector<T, I>::operator[](I i)
{
	if (T* value = GetPointer(i)) {
		return *value;
	}
	else {
		return *Put(i, T());
	}

}

template<class T, class I>
T& CondensedVector<T, I>::At(I i)
{		
	//auto [it1, it2] = std::equal_range(elements.begin(), elements.end(), i, ElementComp{});
	auto [it1, it2] = index_equal_range(i);
	if (it1 != it2) { // подходящий элемент найден		
		return elements[std::distance(elements.begin(), it1)].value;
	}
	else {
		throw std::out_of_range("No such index in the condensed vector");
	}
}

template<class T, class I>
T* CondensedVector<T, I>::GetPointer(I i)
{	
	//auto [it1, it2] = std::equal_range(elements.begin(), elements.end(), i, ElementComp{});
	auto [it1, it2] = index_equal_range(i);
	if (it1 != it2) { // подходящий элемент найден		
		return &elements[std::distance(elements.begin(), it1)].value;
	}
	else {
		return nullptr;
	}	
}

template<class T, class I>
T* CondensedVector<T, I>::Put(I i, T v)
{	
	//auto [it1, it2] = std::equal_range(elements.begin(), elements.end(), i, ElementComp{});
	auto [it1, it2] = index_equal_range(i);
	if (it1 != it2) { // подходящий элемент найден
		size_t i_element = std::distance(elements.begin(), it1);
		elements[i_element].value = std::move(v);
		return &elements[i_element].value;
	}
	else {
		if (it1 == elements.end()) {			
			elements.push_back({i, std::move(v) });
			return &elements.back().value; 
		}
		else {
			auto i_insertion = std::distance(elements.begin(), it1);
			elements.resize(elements.size() + 1); // relocation
			
			auto insertion_point = elements.begin() + i_insertion;
			std::move_backward(insertion_point, std::prev(elements.end()), elements.end());
			
			*insertion_point = {i, std::move(v) };
			

			return &(*insertion_point).value;
		}
	}
}

template<class T, class I>
inline void CondensedVector<T, I>::Insert(I before, I count)
{	
	//const auto it_before = std::lower_bound(elements.begin(), elements.end(), before, ElementComp{});
	const auto it_before = index_lower_bound(before);
	for (auto it = it_before; it != elements.end()/*idx.end()*/; ++it) {		
		(*it).idx += count;
	}
}

template<class T, class I>
inline void CondensedVector<T, I>::Delete(I first, I count)
{
	const auto it_first = index_lower_bound(first);
	const auto it_last = index_upper_bound(first + count - 1);
	//const auto it_first = std::lower_bound(elements.begin(), elements.end(), first, ElementComp{});
	//const auto it_last = std::upper_bound(elements.begin(), elements.end(), first + count - 1, ElementComp{});
	size_t i_first = it_first - elements.begin();
	size_t i_last = it_last - elements.begin();
	for (auto it = it_last; it != elements.end(); ++it) {
		(*it).idx -= count;
	}	
	std::move(elements.begin() + i_last, elements.end(), elements.begin() + i_first);	
	elements.resize(elements.size() - (i_last - i_first));
}


template<class T, class I>
void CondensedVector<T, I>::MoveToVector(std::vector<T>& v, I length, I origin)
{
	v.clear();
	if (elements.size()) v.reserve(std::max(length, MaxIndex().value_or(I())));
	for (size_t i = 0; i < elements.size(); ++i) {
		while (v.size() < to_sizet(elements[i].idx - origin)) {
			v.emplace_back();
		}
		v.push_back(std::move(elements[i].value));
	}
	while (v.size() < to_sizet(length)) {
		v.emplace_back();
	}
}

template<class T, class I>
std::vector<T> CondensedVector<T, I>::ToVector(I length, I origin)
{
	std::vector<T> result;
	result.reserve(std::max(length, MaxIndex().value_or(I())));
	for (size_t i = 0; i < elements.size(); ++i) {
		while (result.size() < to_sizet(elements[i].idx - origin)) {
			result.emplace_back();
		}		
		result.push_back(elements[i].value);
	}
	while (result.size() < to_sizet(length)) {
		result.emplace_back();
	}
	return result;
}

