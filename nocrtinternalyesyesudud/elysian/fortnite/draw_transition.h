#pragma once

namespace elysian
{
	namespace fortnite
	{
		uobject* player_class = nullptr;
		uobject* projectile_class = nullptr;
		uobject* vehicle_class = nullptr;
		uobject* pickup_class = nullptr;
		uobject* container_class = nullptr;
		uobject* weakspot_class = nullptr;
		
		static auto init() -> bool
		{
			auto player_name = Encrypt(L"FortniteGame.FortPlayerPawn");
			auto projectile_name = Encrypt(L"FortniteGame.FortProjectileBase");
			auto vehicle_name = Encrypt(L"FortniteGame.FortAthenaVehicle");
			auto pickup_name = Encrypt(L"FortniteGame.FortPickup");
			auto container_name = Encrypt(L"FortniteGame.BuildingContainer");
			auto weakspot_name = Encrypt(L"FortniteGame.BuildingWeakSpot");

			player_class = uobject::find_object(player_name.decrypt()), player_name.clear();
			projectile_class = uobject::find_object(projectile_name.decrypt()), projectile_name.clear();
			vehicle_class = uobject::find_object(vehicle_name.decrypt()), vehicle_name.clear();
			pickup_class = uobject::find_object(pickup_name.decrypt()), pickup_name.clear();
			container_class = uobject::find_object(container_name.decrypt()), container_name.clear();
			weakspot_class = uobject::find_object(weakspot_name.decrypt()), weakspot_name.clear();

			return true;
		}

		static auto set_aim(uobject* world, aplayercontroller* controller, frotator camera_rotation, frotator aim_rotation, bool interpolate, bool constant, double speed) -> void
		{
			if (world && controller)
			{
				auto world_delta_seconds = libs::statics->get_world_delta_seconds(world);
				if (world_delta_seconds)
				{
					auto result = aim_rotation;
					if (interpolate)
					{
						if (constant)
							result = libs::math->interpolate_constant(camera_rotation, aim_rotation, world_delta_seconds, speed);
						else
							result = libs::math->interpolate(camera_rotation, aim_rotation, world_delta_seconds, speed);
					}

					result.pitch = (result.pitch - camera_rotation.pitch) / *(float*)(controller + 0x534);
					result.yaw = (result.yaw - camera_rotation.yaw) / *(float*)(controller + 0x530);

					controller->add_pitch_input((float)result.pitch);
					controller->add_yaw_input((float)result.yaw);
				}
			}
		}

		flinearcolor get_item_color_by_tier(uint8_t tier)
		{
			if (tier == 0 || tier == 1)
			{
				return flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
			}
			else if (tier == 2)
			{
				return flinearcolor(0.2f, 1.f, 0.2f, 1.f);
			}
			else if (tier == 3)
			{
				return flinearcolor(0.2f, 0.2f, 1.f, 1.f);
			}
			else if (tier == 4)
			{
				return flinearcolor(1.f, 0.2f, 1.f, 1.f);
			}
			else if (tier == 5)
			{
				return flinearcolor(1.f, 1.f, 0.2f, 1.f);
			}
			else if (tier == 6)
			{
				return flinearcolor(1.f, 1.f, 0.f, 1.f);
			}

			return flinearcolor(0.f, 0.f, 0.f, 0.f);
		}

		bool in_area(fvector2d target, fvector2d pos, fvector2d size)
		{
			if (target.x > pos.x && target.y > pos.y)
				if (target.x < pos.x + size.x && target.y < pos.y + size.y)
					return true;

			return false;
		}

		double degree_to_radian(double degree)
		{
			return degree * (3.141592653 / 180);
		}

		fvector angle_rotation(frotator angle)
		{
			double sp, sy, cp, cy;

			sp = libs::math->sin(degree_to_radian(angle.pitch));
			cp = libs::math->cos(degree_to_radian(angle.pitch));

			sy = libs::math->sin(degree_to_radian(angle.yaw));
			cy = libs::math->cos(degree_to_radian(angle.yaw));

			return fvector(cp * cy, cp * sy, -sp);
		}

		void(__fastcall* get_player_view_point_o)(aplayercontroller*, fvector*, frotator*);

		auto get_player_view_point(aplayercontroller* controller, fvector* location, frotator* rotation) -> void
		{
			stub.callstack<void>(get_player_view_point_o, controller, location, rotation);

			settings::cheat_vars::original_location = *location;
			settings::cheat_vars::original_rotation = *rotation;

			if (settings::aimbot::enable && settings::cheat_vars::target_location)
			{
				*rotation = libs::math->find_look_at_rotation(*location, settings::cheat_vars::target_location);
				*location = settings::cheat_vars::target_location;
			}
		}

		void(__fastcall* get_view_point_o)(ulocalplayer*, fminimalviewinfo*, uint8_t);

		auto get_view_point(ulocalplayer* player, fminimalviewinfo* out_view_info, uint8_t stereo) -> void
		{
			stub.callstack<void>(get_view_point_o, player, out_view_info, stereo);

			if (settings::exploits::camera_fov_changer)
			{
				out_view_info->fov = float(settings::exploits::camera_fov); //or just call controller->fov
			}

			if (settings::aimbot::enable && settings::cheat_vars::target_location)
			{
				out_view_info->location = settings::cheat_vars::original_location;
				out_view_info->rotation = settings::cheat_vars::original_rotation;
			}
		}

		void(__fastcall* draw_transition_o)(ugameviewportclient*, ucanvas*);

		auto draw_transition(ugameviewportclient* viewport, ucanvas* canvas) -> void
		{
			if (!render::render_canvas) render::render_canvas = canvas;

			do {
				auto world = *(uobject**)(viewport + 0x78);
				if (!world) continue;

				auto game_instance = *(ugameinstance**)(world + 0x1b8);
				if (!game_instance) continue;

				auto local_player = (*(tarray<ulocalplayer*>*)(game_instance + 0x38))[0];
				if (!local_player) continue;

				auto controller = *(aplayercontroller**)(local_player + 0x30);
				if (!controller) continue;

				auto camera = *(aplayercameramanager**)(controller + 0x340);
				if (!camera) continue;

				if (local_player != settings::cheat_vars::old_player)
				{
					get_view_point_o = vmt(local_player).apply<decltype(get_view_point_o)>(get_view_point, 0x57);
					settings::cheat_vars::old_player = local_player;

					continue;
				}

				if (controller != settings::cheat_vars::old_controller)
				{
					get_player_view_point_o = vmt(controller).apply<decltype(get_player_view_point_o)>(get_player_view_point, 0xF7);
					settings::cheat_vars::old_controller = controller;

					continue;
				}

				auto camera_location = camera->get_camera_location();
				if (!camera_location) continue;

				auto camera_rotation = camera->get_camera_rotation();
				if (!camera_rotation) continue;

				auto camera_fov = camera->get_fov_angle();
				if (!camera_fov) continue;

				input::controller = controller;

				fvector local_location = fvector();
				fvector local_head = fvector();
				bool local_holding_pickaxe = false;
				char local_team = 1337;

				auto pawn = static_cast<afortplayerpawnathena*>(controller->k2_getpawn());

				if (pawn)
				{
					local_location = pawn->k2_get_actor_location();

					if (auto local_mesh = *(uskeletalmeshcomponent**)(pawn + 0x310))
					{
						local_head = local_mesh->get_bone_location(68);
					}

					if (auto local_weapon = *(afortweapon**)(pawn + 0x8d8))
					{
						auto weapon_data = *(uobject**)(local_weapon + 0x3f0);
						if (weapon_data)
						{
							auto display_name = *(ftext*)(weapon_data + 0x90);

							auto name = display_name.to_string();

							if (name && name != L"(null)")
							{
								local_holding_pickaxe = libs::string->contains(name, L"Harvesting Tool", false);
							}
						}
					}

					local_team = pawn->get_team();
				}

				float clip_x = *(float*)(canvas + 0x30);
				if (!clip_x) continue;

				float clip_y = *(float*)(canvas + 0x34);
				if (!clip_y) continue;

				fvector2d screen = fvector2d(double(clip_x), double(clip_y));
				fvector2d center = fvector2d(screen.x / 2, screen.y / 2);

				double aim_radius = (settings::aimbot::fov * center.x / camera_fov) / 2;

				double closest_distance = DBL_MAX;
				double closest_crosshair_distance = DBL_MAX;

				fvector temp_silent_location = fvector();
				afortplayerpawnathena* closest_player_by_distance = nullptr;
				afortplayerpawnathena* closest_player_by_crosshair = nullptr;

				if (settings::player::enable)
				{
					auto player_list = libs::statics->get_all_actors_of_class(world, player_class);
					for (int u = 0x0; u < player_list.size(); u++)
					{
						auto player = static_cast<afortplayerpawnathena*>(player_list[u]);
						if (!player) continue;

						bool local = player == pawn;
						if (local && !settings::player::local) continue;

						auto mesh = *(uskeletalmeshcomponent**)(player + 0x310);
						if (!mesh) continue;

						bool dead = player->is_dead();
						if (dead) continue;

						bool wounded = player->is_dbno();
						if (wounded && !settings::player::wounded) continue;

						char team = player->get_team();
						bool teammate = team == local_team;
						if (teammate && !local && !settings::player::teammate) continue;

						bool success = true;
						fvector bottom, bottom_2, head, head_2, neck, chest, pelvis, rshoulder, relbow, rhand, rthigh, rknee, rfoot, rfootout, lshoulder, lelbow, lhand, lthigh, lknee, lfoot, lfootout;
						fvector2d s_bottom, s_bottom_2, s_head, s_head_2, s_neck, s_chest, s_pelvis, s_rshoulder, s_relbow, s_rhand, s_rthigh, s_rknee, s_rfoot, s_rfootout, s_lshoulder, s_lelbow, s_lhand, s_lthigh, s_lknee, s_lfoot, s_lfootout;

						head = mesh->get_bone_location(68);
						bottom = mesh->get_bone_location(0);

						auto max_distance = settings::player::distance_limit;
						double distance = libs::math->vector_distance(bottom, camera_location) * 0.01;
						if (distance > max_distance) continue;

						float skeleton_thickness = 1.f;
						float box_thickness = 1.f;
						flinearcolor skeleton_color = flinearcolor(1.f, 1.f, 1.f, 1.f);
						flinearcolor box_color = flinearcolor(1.f, 1.f, 1.f, 1.f);
						flinearcolor username_color = flinearcolor(1.f, 1.f, 0.f, 1.f);

						bool visible = true; //gotta get raycasttest working :c

						if (teammate)
						{
							skeleton_color = flinearcolor(0.f, 0.f, 1.f, 1.f);
							box_color = flinearcolor(0.f, 0.f, 1.f, 1.f);
						}
						else if (wounded)
						{
							skeleton_color = flinearcolor(1.f, 0.5f, 0.f, 1.f);
							box_color = flinearcolor(1.f, 0.5f, 0.f, 1.f);
						}

						if (!teammate && !wounded && distance < closest_distance)
						{
							closest_player_by_distance = player;
							closest_distance = distance;
						}

						if (success && !controller->world_to_screen(bottom, &s_bottom)) success = false;

						bottom_2 = bottom; bottom_2.z -= 10;
						if (success && !controller->world_to_screen(bottom_2, &s_bottom_2)) success = false;

						if (success && !controller->world_to_screen(head, &s_head)) success = false;
						
						head_2 = head; head_2.z += 13;
						if (success && !controller->world_to_screen(head_2, &s_head_2)) success = false;

						neck = mesh->get_bone_location(67);
						if (success && !controller->world_to_screen(neck, &s_neck)) success = false;

						chest = mesh->get_bone_location(7); chest.z += 7;
						if (success && !controller->world_to_screen(chest, &s_chest)) success = false;

						pelvis = mesh->get_bone_location(2);
						if (success && !controller->world_to_screen(pelvis, &s_pelvis)) success = false;

						rshoulder = mesh->get_bone_location(38);
						if (success && !controller->world_to_screen(rshoulder, &s_rshoulder)) success = false;

						relbow = mesh->get_bone_location(39);
						if (success && !controller->world_to_screen(relbow, &s_relbow)) success = false;

						rhand = mesh->get_bone_location(40);
						if (success && !controller->world_to_screen(rhand, &s_rhand)) success = false;

						rthigh = mesh->get_bone_location(78);
						if (success && !controller->world_to_screen(rthigh, &s_rthigh)) success = false;

						rknee = mesh->get_bone_location(79);
						if (success && !controller->world_to_screen(rknee, &s_rknee)) success = false;

						rfoot = mesh->get_bone_location(82);
						if (success && !controller->world_to_screen(rfoot, &s_rfoot)) success = false;

						rfootout = mesh->get_bone_location(83);
						if (success && !controller->world_to_screen(rfootout, &s_rfootout)) success = false;

						lshoulder = mesh->get_bone_location(9);
						if (success && !controller->world_to_screen(lshoulder, &s_lshoulder)) success = false;

						lelbow = mesh->get_bone_location(10);
						if (success && !controller->world_to_screen(lelbow, &s_lelbow)) success = false;

						lhand = mesh->get_bone_location(11);
						if (success && !controller->world_to_screen(lhand, &s_lhand)) success = false;

						lthigh = mesh->get_bone_location(71);
						if (success && !controller->world_to_screen(lthigh, &s_lthigh)) success = false;

						lknee = mesh->get_bone_location(72);
						if (success && !controller->world_to_screen(lknee, &s_lknee)) success = false;

						lfoot = mesh->get_bone_location(75);
						if (success && !controller->world_to_screen(lfoot, &s_lfoot)) success = false;

						lfootout = mesh->get_bone_location(76);
						if (success && !controller->world_to_screen(lfootout, &s_lfootout)) success = false;

						if (!bottom || !head || !neck || !chest || !rshoulder || !relbow || !rhand || !lshoulder || !lelbow || !lhand || !rthigh || !rknee || !rfoot || !rfootout || !lthigh || !lknee || !lfoot || !lfootout)
							success = false;

						if (!s_bottom || !s_head || !s_neck || !s_chest || !s_rshoulder || !s_relbow || !s_rhand || !s_lshoulder || !s_lelbow || !s_lhand || !s_rthigh || !s_rknee || !s_rfoot || !s_rfootout || !s_lthigh || !s_lknee || !s_lfoot || !s_lfootout)
							success = false;
						
						if (!success) continue;

						if (!in_area(s_bottom, fvector2d(50, 50), fvector2d(screen.x - (50 * 2), screen.y - (50 * 2)))) continue;

						double crosshair_distance = libs::math->vector_distance_2d(center, s_head);

						if (!teammate && !wounded && crosshair_distance < closest_crosshair_distance)
						{
							closest_player_by_crosshair = player;
							closest_crosshair_distance = crosshair_distance;
						}

						if (settings::player::skeleton)
						{
							render::line(s_head, s_neck, skeleton_color, skeleton_thickness);
							render::line(s_neck, s_chest, skeleton_color, skeleton_thickness);
							render::line(s_chest, s_pelvis, skeleton_color, skeleton_thickness);

							render::line(s_chest, s_rshoulder, skeleton_color, skeleton_thickness);
							render::line(s_rshoulder, s_relbow, skeleton_color, skeleton_thickness);
							render::line(s_relbow, s_rhand, skeleton_color, skeleton_thickness);
							render::line(s_pelvis, s_rthigh, skeleton_color, skeleton_thickness);
							render::line(s_rthigh, s_rknee, skeleton_color, skeleton_thickness);
							render::line(s_rknee, s_rfoot, skeleton_color, skeleton_thickness);
							render::line(s_rfoot, s_rfootout, skeleton_color, skeleton_thickness);

							render::line(s_chest, s_lshoulder, skeleton_color, skeleton_thickness);
							render::line(s_lshoulder, s_lelbow, skeleton_color, skeleton_thickness);
							render::line(s_lelbow, s_lhand, skeleton_color, skeleton_thickness);
							render::line(s_pelvis, s_lthigh, skeleton_color, skeleton_thickness);
							render::line(s_lthigh, s_lknee, skeleton_color, skeleton_thickness);
							render::line(s_lknee, s_lfoot, skeleton_color, skeleton_thickness);
							render::line(s_lfoot, s_lfootout, skeleton_color, skeleton_thickness);
						}

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

						auto calculated_distance = 225 - distance;
						auto offset = calculated_distance * 0.025;
						auto corner_wide = actor_width / 3;
						auto corner_heit = actor_height / 3;

						auto text_offset = double(0);
						fvector2d bottom_middle = fvector2d(most_left + actor_width / 2, s_bottom_2.y);

						if (settings::player::box_mode == 1)
						{
							//top line
							render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_right + offset, most_top - offset), box_color, box_thickness);

							//bottom line
							render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_right + offset, most_bottom + offset), box_color, box_thickness);

							//left line
							render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_left - offset, most_top - offset), box_color, box_thickness);

							//right line
							render::line(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset, most_bottom + offset), box_color, box_thickness);
						}
						else if (settings::player::box_mode == 2)
						{
							//top left
							render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_left - offset + corner_wide, most_top - offset), box_color, box_thickness);
							render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_left - offset, most_top - offset + corner_heit), box_color, box_thickness);

							//top right
							render::line(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset - corner_wide, most_top - offset), box_color, box_thickness);
							render::line(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset, most_top - offset + corner_heit), box_color, box_thickness);

							//bottom left
							render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_left - offset + corner_wide, most_bottom + offset), box_color, box_thickness);
							render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_left - offset, most_bottom + offset - corner_heit), box_color, box_thickness);

							//bottom right
							render::line(fvector2d(most_right + offset, most_bottom + offset), fvector2d(most_right + offset - corner_wide, most_bottom + offset), box_color, box_thickness);
							render::line(fvector2d(most_right + offset, most_bottom + offset), fvector2d(most_right + offset, most_bottom + offset - corner_heit), box_color, box_thickness);
						}

						int bottom_text_offset = 0;

						if (settings::player::weapon)
						{
							auto weapon = *(afortweapon**)(player + 0x8d8);
							if (weapon)
							{
								auto weapon_data = *(uobject**)(weapon + 0x3f0);
								if (weapon_data)
								{
									auto display_name = *(ftext*)(weapon_data + 0x90);

									auto name = display_name.to_string();

									if (name && name != L"(null)")
									{
										auto tier = *(uint8_t*)(weapon_data + 0x73);
										auto render_color = get_item_color_by_tier(tier);

										if (render_color)
										{
											render::text(name, fvector2d(bottom_middle.x, most_bottom + offset + bottom_text_offset), render_color, true, false, true);

											bottom_text_offset += 14;
										}
									}
								}
							}
						}

						if (settings::player::username)
						{
							auto name = libs::fort->get_player_name_safe(player);
							render::text(name.c_str(), fvector2d(bottom_middle.x, most_bottom + offset + bottom_text_offset), username_color, true, false, true);

							bottom_text_offset += 14;
						}

						if (settings::player::line_mode == 1)
						{
							render::line(fvector2d(center.x, screen.y), fvector2d(bottom_middle.x, most_bottom + offset + bottom_text_offset), box_color, 1.f);
						}
						else if (settings::player::line_mode == 2)
						{
							render::line(center, fvector2d(bottom_middle.x, most_bottom + offset + bottom_text_offset), box_color, 1.f);
						}
						else if (settings::player::line_mode == 3)
						{
							render::line(fvector2d(center.x, 0), fvector2d(bottom_middle.x, most_bottom + offset + bottom_text_offset), box_color, 1.f);
						}
					}
				}

				//if (settings::environment::enable && pawn)
				//{
				//	if (settings::environment::pickups)
				//	{
				//		auto pickup_list = libs::statics->get_all_actors_of_class(world, pickup_class);
				//		for (int u = 0x0; u < pickup_list.size(); u++)
				//		{
				//			auto pickup = static_cast<aactor*>(pickup_list[u]);
				//			if (!pickup) continue;

				//			auto screen_location = fvector2d();
				//			auto world_location = pickup->k2_get_actor_location();
				//			
				//			auto max_distance = settings::environment::distance_limit;
				//			double distance = libs::math->vector_distance(world_location, camera_location) * 0.01;
				//			if (distance > max_distance) continue;

				//			if (controller->world_to_screen(world_location, &screen_location) && screen_location)
				//			{
				//				if (!in_area(screen_location, fvector2d(50, 50), fvector2d(screen.x - (50 * 2), screen.y - (50 * 2)))) continue;

				//				auto item_definition = *(uobject**)(pickup + 0x318 + 0x18); //PrimaryPickupItemEntry.ItemDefinition
				//				if (!item_definition) continue;

				//				auto display_name = *(ftext*)(item_definition + 0x90);

				//				auto name = display_name.to_string();

				//				if (name && name != L"(null)")
				//				{
				//					auto tier = *(uint8_t*)(item_definition + 0x73);

				//					auto min_tier = (uint8_t)(settings::environment::pickup_min_tier_mode + 1);

				//					if (tier < min_tier) continue;

				//					auto render_color = get_item_color_by_tier(tier);

				//					if (render_color)
				//					{
				//						render::text(name, screen_location, render_color, true, true, true);
				//					}
				//				}
				//			}
				//		}
				//	}

				//	if (settings::environment::containers)
				//	{
				//		auto container_list = libs::statics->get_all_actors_of_class(world, container_class);
				//		for (int u = 0x0; u < container_list.size(); u++)
				//		{
				//			auto container = static_cast<aactor*>(container_list[u]);
				//			if (!container) continue;

				//			auto screen_location = fvector2d();
				//			auto world_location = container->k2_get_actor_location();

				//			auto max_distance = settings::environment::distance_limit;
				//			double distance = libs::math->vector_distance(world_location, camera_location) * 0.01;
				//			if (distance > max_distance) continue;

				//			if (controller->world_to_screen(world_location, &screen_location) && screen_location)
				//			{
				//				if (!in_area(screen_location, fvector2d(50, 50), fvector2d(screen.x - (50 * 2), screen.y - (50 * 2)))) continue;

				//				bool already_searched = *(char*)(container + 0x1032) & (1 << 3);
				//				if (already_searched) continue;

				//				auto object_name = libs::system->get_object_name(container);
				//				if (libs::string->contains(object_name.c_str(), L"Tiered_Chest", false))
				//				{
				//					render::text(L"Chest", screen_location, flinearcolor(1.f, 1.f, 0.f, 1.f), true, true, true);
				//				}
				//				else if (libs::string->contains(object_name.c_str(), L"Tiered_Ammo", false))
				//				{
				//					render::text(L"Ammo box", screen_location, flinearcolor(0.4f, 0.4f, 0.4f, 1.f), true, true, true);
				//				}
				//				//else //unknown container
				//				//{
				//				//	//render::text(object_name.c_str(), screen_location, flinearcolor(1.f, 0.f, 0.f, 1.f), true, true, true);
				//				//}
				//			}
				//		}
				//	}
				//}

				////shit way ik
				//if (settings::exploits::rapid_fire && pawn)
				//{
				//	if (auto local_weapon = *(afortweapon**)(pawn + 0x8d8))
				//	{
				//		auto last_fire_time = *(float*)(local_weapon + 0xab0);
				//		auto last_fire_time_verified = *(float*)(local_weapon + 0xab4);

				//		*(float*)(local_weapon + 0xab0) = last_fire_time + last_fire_time_verified - 10;
				//	}
				//}

				if (settings::aimbot::enable && pawn)
				{
					render::circle(center, flinearcolor(1.f, 1.f, 1.f, 1.f), aim_radius, 128);

					afortplayerpawnathena* target_player = closest_player_by_crosshair;

					if (target_player && !local_holding_pickaxe)
					{
						if (auto target_mesh = *(uskeletalmeshcomponent**)(target_player + 0x310))
						{
							auto target_location = target_mesh->get_bone_location(68);
							auto target_screen = fvector2d();

							if ((controller->world_to_screen(target_location, &target_screen) && target_screen && customs::in_circle(center.x, center.y, aim_radius, target_screen.x, target_screen.y)))
							{
								if (target_screen)
								{
									render::line(center, target_screen, flinearcolor(0.f, 1.f, 1.f, 1.f), 1.f);
								}

								auto aimbot_valid = target_player && (input::down(right_mouse_button) || input::down(left_mouse_button));

								if (aimbot_valid)
								{
									temp_silent_location = target_location;
								}
							}
						}
					}
				}

				//if (settings::exploits::bullet_teleport && pawn)
				//{
				//	auto projectile_list = libs::statics->get_all_actors_of_class(world, projectile_class);
				//	for (int u = 0x0; u < projectile_list.size(); u++)
				//	{
				//		auto projectile = static_cast<afortprojectilebase*>(projectile_list[u]);
				//		if (!projectile) continue;

				//		auto world_location = projectile->k2_get_actor_location();
				//		auto screen_location = fvector2d();
				//		if (controller->world_to_screen(world_location, &screen_location) && screen_location)
				//		{
				//			auto object_name = libs::system->get_object_name(projectile).c_str();
				//			render::text(libs::system->get_object_name(projectile).c_str(), screen_location, flinearcolor(1.f, 0.f, 0.f, 1.f), true, true, true);

				//			if (libs::string->contains(object_name, L"Bullet", false) || libs::string->contains(object_name, L"Arrow", false))
				//			{
				//				if (closest_player_by_distance)
				//				{
				//					if (auto target_mesh = *(uskeletalmeshcomponent**)(closest_player_by_distance + 0x310))
				//					{
				//						projectile->k2_set_actor_location(target_mesh->get_bone_location(68), false, false);
				//					}
				//				}
				//			}
				//		}
				//	}
				//}

				settings::cheat_vars::target_location = temp_silent_location;

				if (pawn)
				{
					auto current_vehicle = *(afortvehicle**)(pawn + 0x23d8);
					auto vehicle_name = libs::system->get_object_name(current_vehicle);
					bool is_boat = libs::string->contains(vehicle_name.c_str(), L"Meatball", false);

					//infinite boost
					if (current_vehicle)
					{
						if (auto fuel_component = *(ufortvehiclefuelcomponent**)(current_vehicle + 0x10a0))
						{
							auto max_fuel = fuel_component->get_fuel_capacity();

							if (*(float*)(fuel_component + 0xf0) != max_fuel)
							{
								*(float*)(fuel_component + 0xf0) = max_fuel;
							}
						}
					}
					
					//pasted from uc cuz i like it
					if (settings::exploits::boat_speedhack && current_vehicle && is_boat && input::down(shift))
					{
						*(float*)(current_vehicle + 0x7ac) = float(settings::exploits::boat_speed);
						*(float*)(current_vehicle + 0xcb4) = float(settings::exploits::boat_speed);
						*(float*)(current_vehicle + 0x918) = float(settings::exploits::boat_speed);
					}

					//if ( input::down( f8 ) )
					//{
					//	//settings::cheat_vars::fly = true;
					//	//current_vehicle->k2_set_actor_location(closest_player_by_distance->k2_get_actor_location(), false, false);
					//}


					if (settings::exploits::vehicle_teleport && current_vehicle && closest_player_by_distance)
					{
						render::text(L"TELEPORT READY [F2]", fvector2d(center.x, 500), flinearcolor(1.f, 1.f, 0.f, 1.f), true, true, true);

						auto vehicle_location = current_vehicle->k2_get_actor_location();
						auto target_location = closest_player_by_distance->k2_get_actor_location();

						fvector2d vehicle_screen, target_screen;

						if (controller->world_to_screen(vehicle_location, &vehicle_screen) && vehicle_screen && controller->world_to_screen(target_location, &target_screen) && target_screen)
						{
							render::line(vehicle_screen, target_screen, flinearcolor(0.f, 1.f, 1.f, 1.f), 1.f);
						}

						if (input::down(f2))
						{
							//settings::cheat_vars::fly = true;
							current_vehicle->k2_set_actor_location(closest_player_by_distance->k2_get_actor_location(), false, false);
						}
					}

					if (settings::exploits::vehicle_flight && current_vehicle)
					{
						for (int i = 0; i < settings::exploits::flight_speed; i++)
						{
							auto current_location = current_vehicle->k2_get_actor_location();
							auto current_rotation = camera_rotation;

							double angle = current_rotation.yaw * (3.141592653 / 180.0);

							double sy = libs::math->sin(angle);
							double cy = libs::math->cos(angle);

							angle = -current_rotation.pitch * (3.141592653 / 180.0);
							double sp = libs::math->sin(angle);
							double cp = libs::math->cos(angle);

							if (input::down(w))
							{
								current_vehicle->k2_set_actor_location_and_rotation(current_location + fvector{ cp * cy , cp * sy, -sp }, current_rotation, false, false);
							}
							else
							{
								current_vehicle->k2_set_actor_location_and_rotation(current_location, current_rotation, false, false);
							}
						}
					}
				}

				flinearcolor crosshair_color = flinearcolor(1.f, 0.f, 0.f, 1.f);
				double crosshair_outer = double(14);
				double crosshair_inner = double(8);

				render::line(fvector2d(center.x - 10, center.y), fvector2d(center.x + 10, center.y), crosshair_color, 1.f);
				render::line(fvector2d(center.x, center.y - 10), fvector2d(center.x, center.y + 10), crosshair_color, 1.f);
			} while (false);

			return stub.callstack<void>(draw_transition_o, viewport, canvas);
		}
	}
}