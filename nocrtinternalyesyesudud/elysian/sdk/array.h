#pragma once

//size: 0x10
template <class type>
struct tarray
{
	tarray() { this->data = nullptr; this->count = this->max = std::uint32_t(0); };

	type* data;
	std::uint32_t count, max;

	type& operator[](int i) { return this->data[i]; };

	int size() { return this->count; }

	bool valid(int i) { return bool(i < this->count); }

	//todo: add
};