#pragma once

namespace elysian_fn {
	namespace player {
		static int run(uobject* world, controller* controller, tarray<uobject*> actors) {
			int target_id = 1337;
			double closest_distance = DBL_MAX;

			for (int i = 0; i < actors.size(); i++) {
				if (!actors.valid(i)) continue;

				if (auto player = (playerpawn*)actors[i]) {
					if (player->is_dead()) continue;

					if (player->get_team() == variables::local_team) continue;

					auto mesh = player->get_mesh();
					if (!mesh) continue;

					auto state = player->get_state();

					bool success = true;
					fvector bottom, bottom_2, head, head_2, neck, chest, pelvis, rshoulder, relbow, rhand, rthigh, rknee, rfoot, rfootout, lshoulder, lelbow, lhand, lthigh, lknee, lfoot, lfootout;
					fvector2d s_bottom, s_bottom_2, s_head, s_head_2, s_neck, s_chest, s_pelvis, s_rshoulder, s_relbow, s_rhand, s_rthigh, s_rknee, s_rfoot, s_rfootout, s_lshoulder, s_lelbow, s_lhand, s_lthigh, s_lknee, s_lfoot, s_lfootout;

					bottom = mesh->get_bone_location(0);
					if (success && !controller->w2s(bottom, &s_bottom)) success = false;

					bottom_2 = bottom; bottom_2.z -= 10;
					if (success && !controller->w2s(bottom_2, &s_bottom_2)) success = false;

					head = mesh->get_bone_location(68);
					if (success && !controller->w2s(head, &s_head)) success = false;

					head_2 = head; head_2.z += 13;
					if (success && !controller->w2s(head_2, &s_head_2)) success = false;

					neck = mesh->get_bone_location(67);
					if (success && !controller->w2s(neck, &s_neck)) success = false;

					chest = mesh->get_bone_location(7); chest.z += 7;
					if (success && !controller->w2s(chest, &s_chest)) success = false;

					pelvis = mesh->get_bone_location(2);
					if (success && !controller->w2s(pelvis, &s_pelvis)) success = false;

					rshoulder = mesh->get_bone_location(38);
					if (success && !controller->w2s(rshoulder, &s_rshoulder)) success = false;

					relbow = mesh->get_bone_location(39);
					if (success && !controller->w2s(relbow, &s_relbow)) success = false;

					rhand = mesh->get_bone_location(40);
					if (success && !controller->w2s(rhand, &s_rhand)) success = false;

					rthigh = mesh->get_bone_location(78);
					if (success && !controller->w2s(rthigh, &s_rthigh)) success = false;

					rknee = mesh->get_bone_location(79);
					if (success && !controller->w2s(rknee, &s_rknee)) success = false;

					rfoot = mesh->get_bone_location(82);
					if (success && !controller->w2s(rfoot, &s_rfoot)) success = false;

					rfootout = mesh->get_bone_location(83);
					if (success && !controller->w2s(rfootout, &s_rfootout)) success = false;

					lshoulder = mesh->get_bone_location(9);
					if (success && !controller->w2s(lshoulder, &s_lshoulder)) success = false;

					lelbow = mesh->get_bone_location(10);
					if (success && !controller->w2s(lelbow, &s_lelbow)) success = false;

					lhand = mesh->get_bone_location(11);
					if (success && !controller->w2s(lhand, &s_lhand)) success = false;

					lthigh = mesh->get_bone_location(71);
					if (success && !controller->w2s(lthigh, &s_lthigh)) success = false;

					lknee = mesh->get_bone_location(72);
					if (success && !controller->w2s(lknee, &s_lknee)) success = false;

					lfoot = mesh->get_bone_location(75);
					if (success && !controller->w2s(lfoot, &s_lfoot)) success = false;

					lfootout = mesh->get_bone_location(76);
					if (success && !controller->w2s(lfootout, &s_lfootout)) success = false;

					if (!bottom || !head || !neck || !chest || !rshoulder || !relbow || !rhand || !lshoulder || !lelbow || !lhand || !rthigh || !rknee || !rfoot || !rfootout || !lthigh || !lknee || !lfoot || !lfootout)
						success = false;

					if (!s_bottom || !s_head || !s_neck || !s_chest || !s_rshoulder || !s_relbow || !s_rhand || !s_lshoulder || !s_lelbow || !s_lhand || !s_rthigh || !s_rknee || !s_rfoot || !s_rfootout || !s_lthigh || !s_lknee || !s_lfoot || !s_lfootout)
						success = false;
					if (!success) continue;

					auto max_distance = options::player::max_distance;
					double distance = math::vector_distance(bottom, variables::camera_location) * 0.01;
					if (distance > max_distance) continue;

					fvector2d bones_to_check[] = { s_bottom, s_head, s_head_2, s_neck, s_chest, s_pelvis, s_rshoulder, s_relbow, s_rhand, s_rthigh, s_rknee, s_rfoot, s_rfootout, s_lshoulder, s_lelbow, s_lhand, s_lthigh, s_lknee, s_lfoot, s_lfootout };

					auto most_left = DBL_MAX;
					auto most_right = DBL_MIN;
					auto most_top = DBL_MAX;
					auto most_bottom = DBL_MIN;

					for (int i = 0; i < 20; i++) {
						auto bone = bones_to_check[i];

						if (bone.x < most_left)
							most_left = bone.x;

						if (bone.x > most_right)
							most_right = bone.x;

						if (bone.y < most_top)
							most_top = bone.y;

						if (bone.y > most_bottom)
							most_bottom = bone.y;
					}

					auto actor_height = most_bottom - most_top;
					auto actor_width = most_right - most_left;

					auto calculated_distance = max_distance - distance;
					auto offset = calculated_distance * 0.025;
					auto corner_wide = actor_width / 3;
					auto corner_heit = actor_height / 3;

					auto text_offset = double(0);
					fvector2d bottom_middle = fvector2d(most_left + actor_width / 2, s_bottom_2.y);

					flinearcolor esp_color = flinearcolor(0.9f, 0.9f, 0.9f, 0.9f);

					if (options::player::skeleton) {
						render::line(s_head, s_neck, esp_color, 1.f);
						render::line(s_neck, s_chest, esp_color, 1.f);
						render::line(s_chest, s_pelvis, esp_color, 1.f);

						render::line(s_chest, s_rshoulder, esp_color, 1.f);
						render::line(s_rshoulder, s_relbow, esp_color, 1.f);
						render::line(s_relbow, s_rhand, esp_color, 1.f);
						render::line(s_pelvis, s_rthigh, esp_color, 1.f);
						render::line(s_rthigh, s_rknee, esp_color, 1.f);
						render::line(s_rknee, s_rfoot, esp_color, 1.f);
						render::line(s_rfoot, s_rfootout, esp_color, 1.f);

						render::line(s_chest, s_lshoulder, esp_color, 1.f);
						render::line(s_lshoulder, s_lelbow, esp_color, 1.f);
						render::line(s_lelbow, s_lhand, esp_color, 1.f);
						render::line(s_pelvis, s_lthigh, esp_color, 1.f);
						render::line(s_lthigh, s_lknee, esp_color, 1.f);
						render::line(s_lknee, s_lfoot, esp_color, 1.f);
						render::line(s_lfoot, s_lfootout, esp_color, 1.f);
					}

					if (options::player::box) {
						//top left
						render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_left - offset + corner_wide, most_top - offset), esp_color, 2.f);
						render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_left - offset, most_top - offset + corner_heit), esp_color, 2.f);

						//top right
						render::line(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset - corner_wide, most_top - offset), esp_color, 2.f);
						render::line(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset, most_top - offset + corner_heit), esp_color, 2.f);

						//bottom left
						render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_left - offset + corner_wide, most_bottom + offset), esp_color, 2.f);
						render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_left - offset, most_bottom + offset - corner_heit), esp_color, 2.f);

						//bottom right
						render::line(fvector2d(most_right + offset, most_bottom + offset), fvector2d(most_right + offset - corner_wide, most_bottom + offset), esp_color, 2.f);
						render::line(fvector2d(most_right + offset, most_bottom + offset), fvector2d(most_right + offset, most_bottom + offset - corner_heit), esp_color, 2.f);
					}

					double text_top = 13;
					double text_bottom = 1;

					if (options::player::platform && state) {
						auto object_name = system::get_object_name(player);
						if (auto name = state->get_platform().c_str()) {

							render::text(name, fvector2d(most_left + (actor_width / 2), most_top - offset - text_top), flinearcolor(0.f, 0.5f, 1.f, 1.f), true, false, false);

							text_top += 12;
						}
						else {
							render::text(L"Bot/AI", fvector2d(most_left + (actor_width / 2), most_top - offset - text_top), flinearcolor(0.f, 0.5f, 1.f, 1.f), true, false, false);

							text_top += 12;
						}
					}

					if (options::player::distance) {
						fstring string_distance = string::build_string_double(L"", L"", std::round(distance), L"m");
						render::text(string_distance.c_str(), fvector2d(most_left + (actor_width / 2), most_top - offset - text_top), flinearcolor(1.f, 0.f, 0.f, 1.f), true, false, false);
						
						text_top += 12;
					}

					if (options::player::username) {
						fstring name = fort::get_player_name_safe(player);
						render::text(name.c_str(), fvector2d(most_left + (actor_width / 2), most_bottom + offset + text_bottom), flinearcolor(1.f, 1.f, 0.f, 1.f), true, false, false);
						
						text_bottom += 12;
					}

					if (options::player::active_item) {
						if (auto current_weapon = player->get_weapon()) {
							if (auto weapon_data = current_weapon->get_weapon_data()) {
								auto display_name = weapon_data->display_name();
								if (display_name.data && display_name.c_str()) {
									auto tier = weapon_data->get_tier();
									
									auto render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
									if (tier == fort_item_tier::I) render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
									else if (tier == fort_item_tier::II) render_color = flinearcolor(0.2f, 0.8f, 0.4f, 1.f);
									else if (tier == fort_item_tier::III) render_color = flinearcolor(0.f, 0.4f, 0.8f, 1.f);
									else if (tier == fort_item_tier::IV) render_color = flinearcolor(1.f, 0.f, 1.f, 1.f);
									else if (tier == fort_item_tier::V) render_color = flinearcolor(0.7f, 0.7f, 0.f, 1.f);
									else if (tier == fort_item_tier::VI) render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);
									
									if (current_weapon->is_reloading()) {
										render::text(L"Reloading..", fvector2d(most_left + (actor_width / 2), most_bottom + offset + text_bottom), flinearcolor(1.f, 0.f, 0.f, 1.f), true, false, false);
									}
									else {
										auto a = string::build_string_int(display_name.c_str(), L" [", current_weapon->get_magazine_ammo_count(), L"");
										auto b = string::build_string_int(L"", L"/", current_weapon->get_bullets_per_clip(), L"]");
										auto c = string::concat(a, b);

										render::text(c.c_str(), fvector2d(most_left + (actor_width / 2), most_bottom + offset + text_bottom), render_color, true, false, false);
									}
									
									text_bottom += 12;
								}
							}
						}
					}

					if (options::player::snapline) {
						render::line(fvector2d(variables::screen_center.x, variables::screen_size.y), fvector2d(most_left + (actor_width / 2), most_bottom + offset + text_bottom), esp_color, 1.f);
					}

					double screen_distance = math::distance_2d(s_head, variables::screen_center);
					if (screen_distance < closest_distance) {
						target_id = i;
						closest_distance = screen_distance;
					}
				}
			}

			return target_id;
		}
	}
}