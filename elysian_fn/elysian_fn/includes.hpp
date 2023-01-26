#pragma once

#include <iostream>
#include <windows.h>

namespace elysian_fn {
	std::uintptr_t game_base = std::uintptr_t(0);
}

#include <memory/memory.hpp>
#include <memory/importer.hpp>
#include <memory/hook.hpp>

#include <engine/customs.hpp>
#include <engine/offsets.hpp>
#include <engine/unreal.hpp>
#include <engine/sdk_structs.hpp>
#include <engine/sdk.hpp>
#include <engine/keys.hpp>

#include <fortnite/defines.hpp>

#include <menu/options.hpp>
#include <menu/render.hpp>
#include <menu/input.hpp>
#include <engine/ZeroInput.h>
#include <engine/ZeroGUI.h>
#include <menu/menu.hpp>

#include <fortnite/actor/actor_classes.hpp>
#include <fortnite/actor/container.hpp>
#include <fortnite/actor/pickup.hpp>
#include <fortnite/actor/player.hpp>
#include <fortnite/actor/vehicle.hpp>
#include <fortnite/actor/weakspot.hpp>
#include <fortnite/view_hooks.hpp>
#include <fortnite/aimbot.hpp>
#include <fortnite/fortnite.hpp>