#pragma once

namespace elysian_fn {
	namespace sdk {
		namespace keys {
			namespace defines {
				static fkey left_mouse_button = fkey(0);
				static fkey right_mouse_button = fkey(0);
				static fkey insert = fkey(0);
			}

			static void init() {
				defines::left_mouse_button = fkey{ string::string_to_name(L"LeftMouseButton"), std::uint32_t(0) };
				defines::right_mouse_button = fkey{ string::string_to_name(L"RightMouseButton"), std::uint32_t(0) };
				defines::insert = fkey{ string::string_to_name(L"Insert"), std::uint32_t(0) };
			}
		}
	}
}