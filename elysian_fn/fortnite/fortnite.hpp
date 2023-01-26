#pragma once


namespace elysian_fn {
	namespace hooks {
		void draw_transition_hk(game_viewport* viewport, ucanvas* canvas) {
			defines::draw_transition_o(viewport, canvas);

			ZeroGUI::SetupCanvas( canvas );
			ZeroGUI::Input::Handle( );

			auto world = viewport->get_world();
			if (!world) return;

			auto game_instance = game_statics::get_game_instance(world);
			if (!game_instance) return;

			auto controller = game_statics::get_player_controller(world);
			if (!controller) return;

			auto camera_manager = game_statics::get_camera_manager(world);
			if (!camera_manager) return;

			auto local_pawn = (playerpawn*)controller->get_pawn();

			if (local_pawn) {
				variables::local_team = local_pawn->get_team();
			}
			else {
				variables::local_team = 1337;
			}

			handle::run(controller);

			variables::canvas = canvas;
			variables::camera_location = camera_manager->get_camera_location();
			variables::camera_rotation = camera_manager->get_camera_rotation();
			variables::camera_fov = camera_manager->get_fov_angle();
			variables::screen_size = fvector2d(double(variables::canvas->get_clip_x()), double(variables::canvas->get_clip_y()));
			variables::screen_center = fvector2d(double(variables::screen_size.x / 2), double(variables::screen_size.y / 2));
			if ( controller->was_input_key_down( keys::defines::insert ) & 1 ) menu_open = !menu_open; //Our menu key

			//render::testfilled(fvector2d(100, 100), fvector2d(400, 400), flinearcolor(1.f, 0.f, 0.f, 1.f), render::orounding, render::orounding_step);
			menu::run( );

			tarray<uobject*> player_array = game_statics::get_all_actors_of_class(world, classes::defines::player_class);
			int aimbot_id = player::run(world, controller, player_array);

			if (options::pickup::enable) {
				tarray<uobject*> pickup_array = game_statics::get_all_actors_of_class(world, classes::defines::pickup_class);
				pickup::run(world, controller, pickup_array);
			}
			
			if (options::aimbot::enable) {
				if (options::aimbot::show_fov) {
					render::circle(variables::screen_center, flinearcolor(1.f, 1.f, 1.f, 1.f), options::aimbot::fov, 128);
				}

				if (aimbot_id != 1337) {
					if (auto player = (playerpawn*)player_array[aimbot_id]) {
						if (auto mesh = player->get_mesh()) {
							auto aim_location = mesh->get_bone_location(68);
							fvector2d aim_2d = fvector2d();

							if (controller->w2s(aim_location, &aim_2d) && customs::in_circle(variables::screen_center.x, variables::screen_center.y, options::aimbot::fov, aim_2d.x, aim_2d.y)) {
								if (right_mouse_down) {
									aimbot::set_aim(world, controller, aim_location, options::aimbot::interpolate, options::aimbot::constant, options::aimbot::speed);
								}
							}
						}
					}
				}
			}

		}

		static bool run(game_viewport* viewport) {
			keys::init();

			//auto function = elysian_fn::vmt::shadow::bind_function_to_cave(elysian_fn::game_base + 0x6451F1 + 0x100, draw_transition_hk);
			auto function = std::uintptr_t(draw_transition_hk);
			elysian_fn::defines::draw_transition_o = elysian_fn::vmt::shadow(viewport).apply<decltype(elysian_fn::defines::draw_transition_o)>(function, 0x6E);

			return true;
		}
	}
}