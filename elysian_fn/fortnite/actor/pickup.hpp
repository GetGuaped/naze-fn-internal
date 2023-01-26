#pragma once

namespace elysian_fn {
	namespace pickup {
		static void run(uobject* world, controller* controller, tarray<uobject*> actors) {
			for (int i = 0; i < actors.size(); i++) {
				if (!actors.valid(i)) continue;

				if (auto pickupactor = (pickuppawn*)actors[i]) {
					auto screen_location = fvector2d();
					auto world_location = pickupactor->get_actor_location();
					auto distance =math::vector_distance(variables::camera_location, world_location) * 0.01;

					if (distance > options::environment::max_distance) continue;

					if (controller->w2s(world_location, &screen_location)) {
						if (auto definition = pickupactor->get_item_definition()) {
							auto display_name = definition->display_name();
							if (display_name.data && display_name.c_str()) {
								auto tier = definition->get_tier();

								auto render_color = flinearcolor(0.f, 0.f, 0.f, 0.f);
								if (options::pickup::common && tier == fort_item_tier::I) render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
								else if (options::pickup::uncommon && tier == fort_item_tier::II) render_color = flinearcolor(0.2f, 0.8f, 0.4f, 1.f);
								else if (options::pickup::rare && tier == fort_item_tier::III) render_color = flinearcolor(0.f, 0.4f, 0.8f, 1.f);
								else if (options::pickup::epic && tier == fort_item_tier::IV) render_color = flinearcolor(1.f, 0.f, 1.f, 1.f);
								else if (options::pickup::legendary && tier == fort_item_tier::V) render_color = flinearcolor(0.7f, 0.7f, 0.f, 1.f);
								else if (options::pickup::mythic && tier == fort_item_tier::VI) render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);

								if (render_color.red && render_color.green && render_color.blue && render_color.alpha) {
									if (options::environment::show_distance) {
										render::text(string::build_string_double(display_name.c_str(), L" [", std::round(distance), L"m]"), screen_location, render_color, true, false, false);
									}
									else {
										render::text(display_name.c_str(), screen_location, render_color, true, false, false);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}