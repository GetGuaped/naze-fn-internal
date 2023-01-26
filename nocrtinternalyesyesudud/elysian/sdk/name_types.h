#pragma once

//size: 0x4
struct fname
{
	fname() : name_index() {}
	fname(std::uint32_t name_index) : name_index(name_index) {}

	operator bool			() { return bool(this->name_index); }
	friend bool operator	== (const fname& a, const fname& b) { return a.name_index == b.name_index; }
	friend bool operator	!= (const fname& a, const fname& b) { return !(a == b); }

	std::uint32_t name_index;
};