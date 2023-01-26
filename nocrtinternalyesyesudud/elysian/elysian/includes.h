#pragma once

#include <iostream>
#include <windows.h>

namespace elysian
{
	std::uintptr_t game_base = std::uintptr_t(0);
}

#include <helper/str_encryption.h>
#include <helper/customs.h>

#include <memory/pattern.h>
#include <memory/spoof.h>
#include <memory/hook.h>

#include <sdk/1_sdk.h>

#include <helper/settings.h>

#include <input/input_handler.h>

#include <render/render.h>

#include <fortnite/draw_transition.h>