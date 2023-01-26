#pragma once

namespace elysian
{
	namespace input
	{
		namespace keys
		{
			fkey left_mouse_button;
			fkey right_mouse_button;
			fkey f2, f8;
			fkey insert;
			fkey w, a, s, d;
			fkey space;
			fkey shift;
		}

		aplayercontroller* controller = nullptr;

		static auto init() -> bool
		{
			auto left_name = Encrypt(L"LeftMouseButton");
			auto right_name = Encrypt(L"RightMouseButton");
			auto insert_name = Encrypt(L"Insert");
			auto f2_name = Encrypt(L"F2");
			auto f8_name = Encrypt(L"F8");
			auto w_name = Encrypt(L"W");
			auto a_name = Encrypt(L"A");
			auto s_name = Encrypt(L"S");
			auto d_name = Encrypt(L"D");
			auto space_name = Encrypt(L"SpaceBar");
			auto shift_name = Encrypt(L"LeftShift");

			keys::left_mouse_button.name = libs::string->string_to_name(left_name.decrypt()), left_name.clear();
			keys::right_mouse_button.name = libs::string->string_to_name(right_name.decrypt()), right_name.clear();
			keys::insert.name = libs::string->string_to_name(insert_name.decrypt()), insert_name.clear();
			keys::f2.name = libs::string->string_to_name(f2_name.decrypt()), f2_name.clear();
			keys::f8.name = libs::string->string_to_name(f8_name.decrypt()), f8_name.clear();
			keys::w.name = libs::string->string_to_name(w_name.decrypt()), w_name.clear();
			keys::a.name = libs::string->string_to_name(a_name.decrypt()), a_name.clear();
			keys::s.name = libs::string->string_to_name(s_name.decrypt()), s_name.clear();
			keys::d.name = libs::string->string_to_name(d_name.decrypt()), d_name.clear();
			keys::space.name = libs::string->string_to_name(space_name.decrypt()), space_name.clear();
			keys::shift.name = libs::string->string_to_name(shift_name.decrypt()), shift_name.clear();

			return true;
		}

		auto down(fkey key) -> bool
		{
			if (controller)
			{
				return controller->is_input_key_down(key);
			}

			return false;
		}

		auto clicked(fkey key) -> bool
		{
			if (controller)
			{
				return controller->was_input_key_just_pressed(key);
			}

			return false;
		}

		auto mouse_position() -> fvector2d
		{
			if (controller)
			{
				return controller->get_mouse_position();
			}

			return fvector2d();
		}
	}
}

using namespace elysian::input::keys;