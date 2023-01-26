#pragma once

//size: 0x28
struct uobject
{
	static auto find_object(const wchar_t* name, uobject* outer = nullptr) -> uobject*
	{
		return uobject::static_find_object(nullptr, outer, name);
	}

	static auto static_find_object(uobject* klass, uobject* outer, const wchar_t* name) -> uobject*
	{
		return stub.callstack<uobject*>(reinterpret_cast<uobject * (*)(uobject*, uobject*, void*)>(elysian::game_base + 0x4824120), klass, outer, name);
	}

	auto process_event(uobject* function, void* params) -> void
	{
		return stub.callstack<void>(reinterpret_cast<void (*)(uobject*, uobject*, void*)>(elysian::game_base + 0xD38314), this, function, params);
	}

	auto class_private() -> uobject*
	{
		return *(uobject**)(this + 0x10);
	}

	auto get_name() -> fname
	{
		return *(fname*)(this + 0x18);
	}
};

//size: 0x30
struct ufield : uobject
{

};

//size: 0x30
struct ustruct : uobject
{

};

//size: 0x230
struct uclass : ustruct
{

};

//size: 0x68
struct uenum : ufield
{

};

//size: 0xe0
struct ufunction : ustruct
{

};