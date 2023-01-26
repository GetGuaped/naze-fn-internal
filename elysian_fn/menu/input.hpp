#pragma once

namespace elysian_fn {
	namespace input {
		controller* handler;

		static bool menu_open = false;

		bool left_mouse_down;
		bool left_mouse_down_already;
		bool left_mouse_clicked;

		bool right_mouse_down;
		bool right_mouse_down_already;
		bool right_mouse_clicked;

		bool insert_down;
		bool insert_down_already;
		bool insert_clicked;

		fvector2d mouse_position;

		static class handle {
		public:
			static void handle_left_mouse_clicked() {
				if (left_mouse_down) {
					if (left_mouse_down_already)
						left_mouse_clicked = false;
					else {
						left_mouse_clicked = true;
						left_mouse_down_already = true;
					}
				}
				else {
					left_mouse_clicked = false;
					left_mouse_down_already = false;
				}
			}

			static void handle_right_mouse_clicked() {
				if (right_mouse_down) {
					if (right_mouse_down_already)
						right_mouse_clicked = false;
					else {
						right_mouse_clicked = true;
						right_mouse_down_already = true;
					}
				}
				else {
					right_mouse_clicked = false;
					right_mouse_down_already = false;
				}
			}

			static void handle_insert_clicked() {
				if (insert_down) {
					if (insert_down_already)
						insert_clicked = false;
					else {
						insert_clicked = true;
						insert_down_already = true;
					}
				}
				else {
					insert_clicked = false;
					insert_down_already = false;
				}
			}

			static void get_keys_down() {
				left_mouse_down = handler->is_input_key_down(keys::defines::left_mouse_button);
				right_mouse_down = handler->is_input_key_down(keys::defines::right_mouse_button);
				insert_down = handler->is_input_key_down(keys::defines::insert);
			}

			static void get_keys_clicked() {
				handle::handle_left_mouse_clicked();
				handle::handle_right_mouse_clicked();
				handle::handle_insert_clicked();
			}

			static void get_mouse_position() {
				mouse_position = handler->get_mouse_position();
			}

			static void run(controller* controller) {
				handler = controller;

				handle::get_keys_down();
				handle::get_keys_clicked();
				handle::get_mouse_position();
			}
		};
	}
}

using namespace elysian_fn::input;