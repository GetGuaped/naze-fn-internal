#pragma once

#include <elysian/includes.h>

namespace elysian
{
	static auto start() -> void
	{
		auto game_name = Encrypt(L"FortniteClient-Win64-Shipping.exe");
		auto world_name = Encrypt(L"Frontend");
		auto font_name = Encrypt(L"DefaultRegularFont");
		auto stub_name = Encrypt("FF 27");

		game_base = customs::get_module(game_name.decrypt()), game_name.clear();

		stub.set_stub( pattern::find( stub_name.decrypt( ), game_base ) ), stub_name.clear( );

		if (!libs::init()) return;

		if (!fortnite::init()) return;

		if (!input::init()) return;

		font = uobject::find_object(font_name.decrypt(), reinterpret_cast<uobject*>(-1)), font_name.clear();

		if (auto world_lobby = uobject::find_object(world_name.decrypt(), reinterpret_cast<uobject*>(-1)))
		{
			world_name.clear();

			auto game_instance = *(ugameinstance**)(world_lobby + 0x1b8);
			auto local_player = (*(tarray<ulocalplayer*>*)(game_instance + 0x38))[0];
			auto game_viewport = *(ugameviewportclient**)(local_player + 0x78);

			fortnite::draw_transition_o = vmt(game_viewport).apply<decltype(fortnite::draw_transition_o)>(fortnite::draw_transition, 0x6E);
		}
	}
}

//todo: custom entry...
auto DllMain(void*, uint32_t call_reason, void*) -> bool
{
	if (call_reason == 1)
	{

		////creating console pretty much dtc
		//FILE* file = nullptr;
		//AllocConsole();
		//freopen_s(&file, "CONOUT$", "w", stdout);

		//MessageBoxA( 0, "e", 0, 0 );
		elysian::start();
	}

	return true;
}