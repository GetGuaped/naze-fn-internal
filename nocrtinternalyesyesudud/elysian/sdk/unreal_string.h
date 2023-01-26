#pragma once

//size: 0x10
struct fstring : private tarray<wchar_t>
{
	fstring() { };
	fstring(wchar_t* other)
	{
		this->max = this->count = *other ? static_cast<int>(elysian::customs::wcslen(other)) + 1 : 0;

		if (this->count)
			this->data = const_cast<wchar_t*>(other);
	};

	wchar_t* c_str()
	{
		return this->data;
	}

	bool valid()
	{
		return this->data != nullptr;
	}
};