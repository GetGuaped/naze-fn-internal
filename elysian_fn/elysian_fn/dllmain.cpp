#pragma once

#include <elysian_fn/includes.hpp>

namespace elysian_fn {
	static void init() {
		//FILE* pFile = nullptr;
		//IFH(AllocConsole)();
		//freopen_s(&pFile, "CONOUT$", "w", stdout);


		/*
		std::uintptr_t dll_base = elysian_fn::customs::get_module(L"CoreMessaging.dll");
		std::uintptr_t dll_cave = dll_base + 0x8BD71;

		printf("\ndll_base: %p", dll_base);
		MessageBoxA(0, "ok", "ok", 0);

		 for (int i = 0; i < 40; i++) {
			 printf("%X ", *(BYTE*)(dll_cave + i - 5));
		 }*/

		//printf("\ndll_base: %p", dll_base);
		//printf("\ndll_cave: %p", dll_cave);
		//for (int i = 0; i < 40; i++) {
		//	printf("%X ", *(BYTE*)(dll_cave + i - 5));
		//}

		
		elysian_fn::game_base = elysian_fn::customs::get_module(L"FortniteClient-Win64-Shipping.exe");		
		elysian_fn::sdk::classes::init();
		
		if (auto world = uobject::find_object(L"Frontend", reinterpret_cast<uobject*>(-1))) {

			auto game_instance = game_statics::get_game_instance(world);
			auto local_player = game_instance->get_local_players()[0];
			auto viewport = local_player->get_viewport();

			hooks::run(viewport);
		}
	}
}

bool DllMain(void*, uint32_t call_reason, void*) {

	if (call_reason == DLL_PROCESS_ATTACH) {
		MessageBoxA( 0, "injection worked", 0, 0 );
	elysian_fn::init();
	}

	return true;
}