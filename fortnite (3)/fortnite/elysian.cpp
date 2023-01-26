#include "elysian/includes.h"

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {

        main_hooking::initialize( );
	}

    return TRUE;
}