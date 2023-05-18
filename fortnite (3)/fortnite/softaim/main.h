#pragma once

/// @brief 
namespace render {
	bool show_menu = true;
	bool show_menu_old = false;
	int hovered_headers = 0;
	uworld* world;
	aplayercontroller* controller;
	ucanvas* canvas;
	uobject* font;
	fvector2d screen_center, screen_size;

	bool is_insert_clicked() {
		static bool insert_clicked = false;
		static bool insert_down_already = false;

		bool insert_down = render::controller->is_key_down(defines::insert);
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

		return insert_clicked;
	}

	bool in_circle(int cx, int cy, int r, int x, int y) {
		int dist = (x - cx) * (x - cx) + (y - cy) * (y - cy);
		if (dist <= r * r)
			return true;
		else
			return false;
	}

	void text(fstring content, fvector2d position, flinearcolor render_color, bool center_x, bool center_y, bool outlined) {
		canvas->k2_draw_text(font, content, position, fvector2d(1.0, 1.0), render_color, 1.0f, flinearcolor(), fvector2d(), center_x, center_y, outlined, flinearcolor(0, 0, 0, 1));
	}

	fvector2d text_size(fstring content) {
		return canvas->k2_text_size(font, content, fvector2d(1.0, 1.0));
	}

	void line(fvector2d a, fvector2d b, flinearcolor render_color, float thickness) {
		canvas->k2_draw_line(a, b, thickness, render_color);
	}

	void circle(fvector2d position, int radius, int segments, flinearcolor color) {
		float PI = 3.1415927f;

		float Step = PI * 2.0 / segments;
		int Count = 0;
		fvector2d V[128];
		for (float a = 0; a < PI * 2.0; a += Step) {
			float X1 = radius * defines::math->cos(a) + position.x;
			float Y1 = radius * defines::math->sin(a) + position.y;
			float X2 = radius * defines::math->cos(a + Step) + position.x;
			float Y2 = radius * defines::math->sin(a + Step) + position.y;
			V[Count].x = X1;
			V[Count].y = Y1;
			V[Count + 1].x = X2;
			V[Count + 1].y = Y2;
			line(fvector2d{ V[Count].x, V[Count].y }, fvector2d{ X2, Y2 }, color, 1.f);
		}
	}

	void filled_box(fvector2d position, fvector2d size, flinearcolor render_color) {
		for (int i = 0; i < size.y; i++) {
			canvas->k2_draw_line(fvector2d(position.x, position.y + i), fvector2d(position.x + size.x, position.y + i), 1.f, render_color);
		}
	}

	void mouse_cursor() {
		auto cursor = render::controller->get_mouse_position();
		filled_box(fvector2d(cursor.x - 2.5, cursor.y - 2.5), fvector2d(5, 5), flinearcolor(0.f, 0.f, 0.f, 1.f));
	}

	class menu_column {
	public:
		bool is_collapsed() {
			return this->collapsed;
		}

		bool in_area(fvector2d pos, fvector2d size) {
			if (this->cursor_pos.x > pos.x && this->cursor_pos.y > pos.y)
				if (this->cursor_pos.x < pos.x + size.x && this->cursor_pos.y < pos.y + size.y)
					return true;

			return false;
		}

		void checkbox(fstring text, bool* option) {
			fvector2d current = menu_pos + menu_offset;
			fvector2d size = fvector2d(this->menu_size.x - (this->wide_offset * 2), 35);

			bool hovered = in_area(current, size);
			bool down = hovered && this->mouse_down;
			bool clicked = hovered && this->mouse_clicked;

			flinearcolor render_color = flinearcolor();
			if (*option)
				render_color = flinearcolor(0.2f, 0.075f, 0.175f, 1.f);

			/*if (down)
				render_color = flinearcolor(0.15f, 0.15f, 0.075f, 1.f);
			else if (hovered)
				render_color = flinearcolor(0.175f, 0.175f, 0.075f, 1.f);
			else if (*option)
				render_color = flinearcolor(0.2f, 0.2f, 0.075f, 1.f);*/

			if (render_color)
				render::filled_box(current, size, render_color);

			render::text(text.c_str(), fvector2d(current.x + size.x / 2, current.y + 9), flinearcolor(1.f, 1.f, 1.f, 1.f), true, false, false);

			if (clicked) *option = !*option;

			this->menu_offset.y += 35;
		}

		void slider(fstring text, double min, double max, double* value) {
			fvector2d current = menu_pos + menu_offset;
			fvector2d size = fvector2d(this->menu_size.x - (this->wide_offset * 2), 35);

			bool hovered = in_area(current, size);
			bool down = hovered && this->mouse_down;

			if (hovered && down) {
				*value = ((this->cursor_pos.x - current.x) * ((max - min) / size.x)) + min;
				*value = defines::math->round(*value);
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			flinearcolor render_color = flinearcolor(0.2f, 0.075f, 0.175f, 1.f);
			double oneP = size.x / (max - min);
			render::filled_box(current, fvector2d(oneP * (*value - min), size.y), render_color);

			fstring end_text = defines::string->build_string_double(text.c_str(), L"", *value, L"");
			render::text(end_text.c_str(), fvector2d(current.x + size.x / 2, current.y + 9), flinearcolor(1.f, 1.f, 1.f, 1.f), true, false, false);

			this->menu_offset.y += 35;
		}

		bool invisible_button(fvector2d size, bool* hovered) {
			fvector2d current = menu_pos + menu_offset;

			*hovered = in_area(current, size);
			bool clicked = *hovered && this->mouse_clicked;

			return clicked;
		}

		bool invisible_r_button(fvector2d size, bool* hovered) {
			fvector2d current = menu_pos + menu_offset;

			*hovered = in_area(current, size);
			bool clicked = *hovered && this->mouse_rmb_clicked;

			return clicked;
		}

		void handle_input() {
			this->cursor_pos = render::controller->get_mouse_position();

			this->mouse_rmb_down = render::controller->is_key_down(defines::right_mouse_button);
			if (this->mouse_rmb_down) {
				if (this->mouse_rmb_down_already)
					this->mouse_rmb_clicked = false;
				else {
					this->mouse_rmb_clicked = true;
					this->mouse_rmb_down_already = true;
				}
			}
			else {
				this->mouse_rmb_clicked = false;
				this->mouse_rmb_down_already = false;
			}

			this->mouse_down = render::controller->is_key_down(defines::left_mouse_button);
			if (this->mouse_down) {
				if (this->mouse_down_already)
					this->mouse_clicked = false;
				else {
					this->mouse_clicked = true;
					this->mouse_down_already = true;
				}
			}
			else {
				this->mouse_clicked = false;
				this->mouse_down_already = false;
			}
		}

		bool begin(fstring title, fvector2d pos, fvector2d size, flinearcolor header_color) {
			if (!this->once) {
				this->menu_pos = pos;
				this->menu_size = size;

				this->wide_offset = 8;

				this->header_color = header_color;

				this->collapsed = true;
				this->once = true;
			}

			//todo: set world
			this->world_delta_time = defines::game_statics->get_world_delta_seconds(render::world);

			this->menu_offset = fvector2d();

			this->handle_input();

			bool header_hovered = false;
			bool header_clicked = invisible_r_button(fvector2d(this->menu_size.x, 40), &header_hovered);
			if (header_clicked) this->collapsed = !this->collapsed;
			/*if (this->anim_closing || (header_clicked && this->collapsed)) {
				this->anim_closing = true;
				this->real_menu_size_y = defines::math->f_interp_to(this->real_menu_size_y, 0, this->world_delta_time, 20);
				
				if (this->real_menu_size_y == 0) {
					this->anim_closing = false;
				}
			}
			else if (this->anim_opening || (header_clicked && !this->collapsed)) {
				this->anim_opening = true;
				this->real_menu_size_y = defines::math->f_interp_to(this->real_menu_size_y, this->menu_size, this->world_delta_time, 20);
				
				if (this->real_menu_size_y == this->menu_size.y) {
					this->anim_opening = false;
				}
			}*/

			//fix menu moving
			if (header_hovered) {
				render::hovered_headers += 1;
			}

			//menu moving
			{
				static fvector2d static_cursor = fvector2d();
				static fvector2d static_calculated = fvector2d();
				
				if (render::hovered_headers == 1 && header_hovered && this->mouse_down) {
					if (!static_cursor)
						static_cursor = this->cursor_pos;

					if (!static_calculated)
						static_calculated = static_cursor - this->menu_pos;

					this->menu_pos = this->cursor_pos - static_calculated;
				}

				if (!this->mouse_down) {
					static_cursor = fvector2d();
					static_calculated = fvector2d();
				}
			}

			//header rendering
			{
				flinearcolor header_color = flinearcolor(0.025f, 0.025f, 0.025f, 1.f);
				if (!this->collapsed)
					header_color = this->header_color;

				render::filled_box(this->menu_pos, fvector2d(this->menu_size.x, 40), header_color);
				render::text(title.c_str(), fvector2d(this->menu_pos.x + (this->menu_size.x / 2), this->menu_pos.y + 12), flinearcolor(0.9f, 0.9f, 0.9f, 0.9f), true, false, false);
			}

			//background rendering
			 if (!this->collapsed) {
				flinearcolor background_color = flinearcolor( 0.025f, 0.025f, 0.025f, 1.f );
				render::filled_box(fvector2d(this->menu_pos.x + this->wide_offset, this->menu_pos.y + 40), fvector2d(this->menu_size.x - (this->wide_offset * 2), this->menu_size.y), background_color);

				this->menu_offset = fvector2d(this->wide_offset, 40);
			 }

			return true;
		}
		
		bool end() {
			return true;
		}
	private:
		float world_delta_time;

		double wide_offset;

		bool anim_opening;
		bool anim_closing;

		bool once;
		bool collapsed;

		bool mouse_rmb_clicked;
		bool mouse_rmb_down;
		bool mouse_rmb_down_already;

		bool mouse_clicked;
		bool mouse_down;
		bool mouse_down_already;

		fvector2d cursor_pos;
		fvector2d menu_pos;
		fvector2d menu_size;
		fvector2d menu_offset;
		double real_menu_size_y;

		flinearcolor header_color;
	};

	void draw_menu(player_pawn* local_pawn) {
		if (render::is_insert_clicked()) render::show_menu = !render::show_menu;

		if (local_pawn && render::show_menu_old != render::show_menu) {
			if (render::show_menu) local_pawn->disable_input(render::controller);
			else local_pawn->enable_input(render::controller);

			render::show_menu_old = render::show_menu;
		}

		if (render::show_menu) {
			render::hovered_headers = 0;

			//8 items per column = 280 pixels

			static render::menu_column exploit_column;
			exploit_column.begin(L"Exploits", fvector2d(200, 200), fvector2d(300, 140), flinearcolor(0.175f, 0.12f, 0.075f, 1.f));
			if (!exploit_column.is_collapsed()) {
				exploit_column.checkbox(L"Enable hooks", &settings::exploits::enable_hooks);
				exploit_column.checkbox(L"Spinbot", &settings::exploits::spinbot);
				//exploit_column.checkbox(L"Enable patches", &settings::exploits::enable_patches);
				//exploit_column.checkbox(L"Instant reload", &settings::exploits::instant_reload);
				exploit_column.checkbox(L"Camera FOV", &settings::exploits::fov_changer);
				exploit_column.slider(L"FOV: ", 70, 170, &settings::exploits::fov_value);
			}
			exploit_column.end();

			static render::menu_column aimbot_column;
			aimbot_column.begin(L"Aimbot", fvector2d(500, 300), fvector2d(300, 385), flinearcolor(0.175f, 0.075f, 0.175f, 1.f));
			if (!aimbot_column.is_collapsed()) {
				aimbot_column.checkbox(L"Enable", &settings::aimbot::enable);
				aimbot_column.checkbox(L"Bullet TP (Quick ban)", &settings::aimbot::bulletp );
				if (settings::aimbot::memory) {
					settings::aimbot::silent = false;
				}
				else if (!settings::aimbot::silent) {
					settings::aimbot::memory = true;
				}
				aimbot_column.checkbox(L"Silent", &settings::aimbot::silent);
				if (settings::aimbot::silent) {
					settings::aimbot::memory = false;
				}
				else if (!settings::aimbot::silent && !settings::aimbot::memory) {
					settings::aimbot::sticky_silent = true;
				}
				aimbot_column.checkbox(L"Interpolate", &settings::aimbot::interpolate);
				aimbot_column.checkbox(L"Constant", &settings::aimbot::constant);
				aimbot_column.checkbox(L"Visible only", &settings::aimbot::visible_only);
				aimbot_column.checkbox(L"Show FOV", &settings::aimbot::show_fov);
				aimbot_column.checkbox(L"Target line", &settings::aimbot::target_line);
				aimbot_column.slider(L"FOV: ", 0, 180, &settings::aimbot::field_of_view);
				aimbot_column.slider(L"Interpolate speed: ", 1, 25, &settings::aimbot::interpolate_speed);
			}
			aimbot_column.end();

			static render::menu_column container_column;
			container_column.begin(L"Containers", fvector2d(900, 300), fvector2d(300, 385), flinearcolor(0.075f, 0.175f, 0.175f, 1.f));
			if (!container_column.is_collapsed()) {
				container_column.checkbox(L"Enable", &settings::containers::enable);
				container_column.checkbox(L"Ignore opened", &settings::containers::ignore_opened);
				container_column.checkbox(L"Display distance", &settings::containers::display_distance);
				container_column.checkbox(L"Lines", &settings::containers::lines);
				container_column.checkbox(L"Chest", &settings::containers::chest);
				container_column.checkbox(L"Ammo box", &settings::containers::ammo_box);
				container_column.checkbox(L"Safe", &settings::containers::safe);
				container_column.checkbox(L"Cooler & Food", &settings::containers::cooler_and_food);
				container_column.checkbox(L"Other", &settings::containers::others);
				container_column.slider(L"Max distance: ", 0, 200, &settings::containers::max_distance);
				container_column.slider(L"Line offset: ", 10, 300, &settings::containers::line_offset);
			}
			container_column.end();

			static render::menu_column pickup_column;
			pickup_column.begin(L"Pickups", fvector2d(200, 300), fvector2d(300, 385), flinearcolor(0.075f, 0.175f, 0.075f, 1.f));
			if (!pickup_column.is_collapsed()) {
				pickup_column.checkbox(L"Enable", &settings::pickups::enable);
				pickup_column.checkbox(L"Display distance", &settings::pickups::display_distance);
				pickup_column.checkbox(L"Lines", &settings::pickups::lines);
				pickup_column.checkbox(L"Common", &settings::pickups::common);
				pickup_column.checkbox(L"Uncommon", &settings::pickups::uncommon);
				pickup_column.checkbox(L"Rare", &settings::pickups::rare);
				pickup_column.checkbox(L"Epic", &settings::pickups::epic);
				pickup_column.checkbox(L"Legendary", &settings::pickups::legendary);
				pickup_column.checkbox(L"Mythic", &settings::pickups::mythic);
				pickup_column.slider(L"Max distance: ", 0, 200, &settings::pickups::max_distance);
				pickup_column.slider(L"Line offset: ", 10, 300, &settings::pickups::line_offset);
			}
			pickup_column.end();

			static render::menu_column stye_column;
			stye_column.begin(L"Style", fvector2d(1300, 300), fvector2d(300, 35), flinearcolor(0.175f, 0.075f, 0.075f, 1.f));
			if (!stye_column.is_collapsed()) {
				stye_column.checkbox(L"Outlined text", &settings::style::text_outlined);
			}
			stye_column.end();

			static render::menu_column player_column;
			player_column.begin(L"Player", fvector2d(1300, 800), fvector2d(300, 350), flinearcolor(0.175f, 0.075f, 0.075f, 1.f));
			if (!player_column.is_collapsed()) {
				player_column.checkbox(L"Enable", &settings::player::enable);
				player_column.checkbox(L"Skeletons", &settings::player::skeleton);
				player_column.checkbox(L"Box", &settings::player::box);
				player_column.checkbox(L"Eye Direction", &settings::player::eyes );
				player_column.checkbox(L"Normal box", &settings::player::normal_box);
				if (settings::player::normal_box) {
					settings::player::cornered_box = false;
				}
				else if (!settings::player::cornered_box) {
					settings::player::normal_box = true;
				}
				player_column.checkbox(L"Cornered box", &settings::player::cornered_box);
				if (settings::player::cornered_box) {
					settings::player::normal_box = false;
				}
				else if (!settings::player::normal_box) {
					settings::player::cornered_box = true;
				}
				player_column.checkbox(L"Snapline", &settings::player::snapline);
				player_column.checkbox(L"Player name", &settings::player::player_name);
				player_column.checkbox(L"Display distance", &settings::player::display_distance);
				player_column.checkbox(L"Active item", &settings::player::active_weapon);
				player_column.slider(L"Max distance: ", 0, 225, &settings::player::max_distance);
			}
			player_column.end();

			static render::menu_column weakspot_column;
			weakspot_column.begin(L"Weakspot", fvector2d(1700, 300), fvector2d(300, 245), flinearcolor(0.075f, 0.075f, 0.175f, 1.f));
			if (!weakspot_column.is_collapsed()) {
				weakspot_column.checkbox(L"Enable (Pickaxe)", &settings::weakspot::enable);
				weakspot_column.checkbox(L"Aimbot", &settings::weakspot::weakspot_aimbot);
				weakspot_column.checkbox(L"Silent", &settings::weakspot::silent);
				weakspot_column.checkbox(L"Interpolate", &settings::weakspot::interpolate);
				weakspot_column.checkbox(L"Constant", &settings::weakspot::constant);
				weakspot_column.checkbox(L"Target line", &settings::weakspot::target_line);
				weakspot_column.slider(L"Interpolate speed: ", 1, 25, &settings::weakspot::interpolate_speed);
			}
			weakspot_column.end();

			static render::menu_column bones_column;
			bones_column.begin( L"Target Bones", fvector2d( 600, 300 ), fvector2d( 300, 140 ), flinearcolor( 0.075f, 0.075f, 0.175f, 1.f ) );
			if ( !bones_column.is_collapsed( ) ) {
				bones_column.checkbox( L"Head", &settings::bones::head );
				if ( settings::bones::head ) {
					settings::bones::neck = false;
					settings::bones::torso = false;
					settings::bones::pelvis = false;
				}
				else if ( !settings::bones::neck && !settings::bones::torso && !settings::bones::pelvis ) {
					settings::bones::head = true;
				}

				bones_column.checkbox( L"Neck", &settings::bones::neck );
				if ( settings::bones::neck ) {
					settings::bones::head = false;
					settings::bones::torso = false;
					settings::bones::pelvis = false;
				}
				else if ( !settings::bones::head && !settings::bones::torso && !settings::bones::pelvis ) {
					settings::bones::neck = true;
				}

				bones_column.checkbox( L"Torso", &settings::bones::torso );
				if ( settings::bones::torso ) {
					settings::bones::neck = false;
					settings::bones::head = false;
					settings::bones::pelvis = false;
				}
				else if ( !settings::bones::neck && !settings::bones::head && !settings::bones::pelvis ) {
					settings::bones::torso = true;
				}

				bones_column.checkbox( L"Penis", &settings::bones::pelvis );
				if ( settings::bones::pelvis ) {
					settings::bones::neck = false;
					settings::bones::torso = false;
					settings::bones::head = false;
				}
				else if ( !settings::bones::neck && !settings::bones::pelvis && !settings::bones::head ) {
					settings::bones::pelvis = true;
				}
			}
			bones_column.end( );

			static render::menu_column radar_column;
			radar_column.begin(L"Radar", fvector2d(1700, 700), fvector2d(300, 385), flinearcolor(0.275f, 0.175f, 0.275f, 1.f));
			if (!radar_column.is_collapsed()) {
				radar_column.checkbox(L"Enable", &settings::radar::enable);
				radar_column.checkbox(L"Display view", &settings::radar::show_line_of_sight);
				radar_column.checkbox(L"Useless circle", &settings::radar::useless_circle);
				radar_column.checkbox(L"Players", &settings::radar::players);
				radar_column.checkbox(L"Containers", &settings::radar::containers);
				radar_column.checkbox(L"Pickups", &settings::radar::pickups);
				radar_column.checkbox(L"Vehicles", &settings::radar::vehicles);
				radar_column.slider(L"Range: ", 5, 30, &settings::radar::range);
				radar_column.slider(L"Size: ", 50, 600, &settings::radar::size);
				radar_column.slider(L"Position X: ", 0, render::screen_size.x, &settings::radar::positionx);
				radar_column.slider(L"Position Y: ", 0, render::screen_size.y, &settings::radar::positiony);
			}
			radar_column.end();

			render::mouse_cursor();
		}
	}
}

#define M_PI   3.14159265358979323846264338327950288

float degree_to_radian( float degree ) {
	return degree * (M_PI / 180);
}

void angle_rotation( const frotator& angles, fvector* forward )
{
	float	sp, sy, cp, cy;

	sy = sin( degree_to_radian( angles.yaw ) );
	cy = cos( degree_to_radian( angles.yaw ) );

	sp = sin( degree_to_radian( angles.pitch ) );
	cp = cos( degree_to_radian( angles.pitch ) );

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

namespace fortnite {
	namespace temp {
		bool silent_active = false;
		frotator silent_rotation = frotator();
		bool sticky_silent_active = false;
		frotator sticky_silent_rotation = frotator();
	}

	namespace radar {
		fvector2d position, size;
		fvector camera_location;
		frotator camera_rotation;

		void range(double* x, double* y, double range) {
			if (defines::math->abs((*x)) > range || defines::math->abs((*y)) > range) {
				if ((*y) > (*x)) {
					if ((*y) > -(*x)) {
						(*x) = range * (*x) / (*y);
						(*y) = range;
					}
					else {
						(*y) = -range * (*y) / (*x);
						(*x) = -range;
					}
				}
				else {
					if ((*y) > -(*x)) {
						(*y) = range * (*y) / (*x);
						(*x) = range;
					}
					else {
						(*x) = -range * (*x) / (*y);
						(*y) = -range;
					}
				}
			}
		}

		void rotate_point(fvector2d* screen, fvector origin, fvector camera_location, frotator camera_rotation) {
			auto yaw = camera_rotation.yaw * 3.1415927 / 180;

			double dx = origin.x - camera_location.x;
			double dy = origin.y - camera_location.y;

			double fsin_yaw = defines::math->sin(yaw);
			double fminus_cos_yaw = -defines::math->cos(yaw);

			double x = -(dy * fminus_cos_yaw + dx * fsin_yaw);
			double y = dx * fminus_cos_yaw - dy * fsin_yaw;

			double range_value = settings::radar::range * 1000;
			range(&x, &y, range_value);

			auto DrawPos = radar::position;
			auto DrawSize = radar::size;

			int rad_x = (int)DrawPos.x;
			int rad_y = (int)DrawPos.y;

			double r_siz_x = DrawSize.x;
			double r_siz_y = DrawSize.y;

			int x_max = (int)r_siz_x + rad_x - 5;
			int y_max = (int)r_siz_y + rad_y - 5;

			double out_screen_x = rad_x + ((int)r_siz_x / 2 + int(x / range_value * r_siz_x));
			double out_screen_y = rad_y + ((int)r_siz_y / 2 + int(y / range_value * r_siz_y));

			if (out_screen_x > x_max)
				out_screen_x = x_max;

			if (out_screen_x < rad_x)
				out_screen_x = rad_x;

			if (out_screen_y > y_max)
				out_screen_y = y_max;

			if (out_screen_y < rad_y)
				out_screen_y = rad_y;

			*screen = fvector2d(out_screen_x, out_screen_y);
		}

		void add_to_radar(fvector world_location, flinearcolor color) {
			fvector2d screen;
			rotate_point(&screen, world_location, radar::camera_location, radar::camera_rotation);

			render::filled_box(screen, fvector2d(4, 4), color);
		}

		void radar_initialize(fvector2d position, fvector2d size, fvector camera_location, frotator camera_rotation) {
			radar::position = position;
			radar::size = size;
			radar::camera_location = camera_location;
			radar::camera_rotation = camera_rotation;

			render::filled_box(position, size, flinearcolor(0.025f, 0.025f, 0.025f, 1.f));

			if (settings::radar::useless_circle) {
				render::circle(fvector2d(position.x + size.x / 2, position.y + size.y / 2), size.x / 2, 100, flinearcolor(1.f, 1.f, 1.f, 1.f));
			}

			if (settings::radar::show_line_of_sight) {
				render::line(fvector2d(position.x + size.x / 2, position.y + size.y), fvector2d(position.x + size.x / 2, position.y + size.y / 2), flinearcolor(1.f, 1.f, 1.f, 1.f), 1.f);
				render::line(fvector2d(position.x + size.x / 2, position.y + size.y / 2), fvector2d(position.x, position.y), flinearcolor(1.f, 1.f, 1.f, 1.f), 1.f);
				render::line(fvector2d(position.x + size.x / 2, position.y + size.y / 2), fvector2d(position.x + size.x, position.y), flinearcolor(1.f, 1.f, 1.f, 1.f), 1.f);
			}
		}
	}

	void set_aim(uworld* world, aplayercontroller* controller, fvector camera_location, frotator camera_rotation, fvector aim_location, bool interpolate, bool constant, double speed) {
		auto aim_rotation = defines::math->find_look_at_rotation(camera_location, aim_location);
		auto world_delta_seconds = defines::game_statics->get_world_delta_seconds(world);

		auto result = aim_rotation;
		if (interpolate) {
			if (constant)
				result = defines::math->interp_to_constant(camera_rotation, aim_rotation, world_delta_seconds, speed);
			else
				result = defines::math->interp_to(camera_rotation, aim_rotation, world_delta_seconds, speed);
		}

		result.pitch = (result.pitch - camera_rotation.pitch) / controller->input_pitch_scale();
		result.yaw = (result.yaw - camera_rotation.yaw) / controller->input_yaw_scale();

		controller->add_pitch_input((float)result.pitch);
		controller->add_yaw_input((float)result.yaw);
	}

	fvector bottom, bottom_2, head, head_2, neck, chest, pelvis, rshoulder, relbow, rhand, rthigh, rknee, rfoot, rfootout, lshoulder, lelbow, lhand, lthigh, lknee, lfoot, lfootout;
	fvector2d s_bottom, s_bottom_2, s_head, s_head_2, s_neck, s_chest, s_pelvis, s_rshoulder, s_relbow, s_rhand, s_rthigh, s_rknee, s_rfoot, s_rfootout, s_lshoulder, s_lelbow, s_lhand, s_lthigh, s_lknee, s_lfoot, s_lfootout;

	bool get_all_bones(aplayercontroller* controller, mesh* mesh) {
		bool success = true;

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

		if (!s_bottom || !s_head || !s_neck || !s_chest || !s_rshoulder || !s_relbow || !s_rhand || !s_lshoulder || !s_lelbow || !s_lhand || !s_rthigh || !s_rknee || !s_rfoot || !s_rfootout || !s_lthigh || !s_lknee || !s_lfoot || !s_lfootout)
			return false;

		return success;
	}
}

namespace essential_hooks {
	fvector location_o = fvector();
	frotator rotation_o = frotator();

	ulocalplayer* local_player_old = 0;
	aplayercontroller* player_controller_old = 0;
	
	using get_view_point_sim = void(*)(ulocalplayer* local_player, fminimalviewinfo* out_viewinfo, BYTE stereo_pass);
	get_view_point_sim get_view_point_o = 0;

	void get_view_point(ulocalplayer* local_player, fminimalviewinfo* out_viewinfo, BYTE stereo_pass) {
		get_view_point_o(local_player, out_viewinfo, stereo_pass);

		if (fortnite::temp::silent_active) {
			out_viewinfo->location = location_o;
			out_viewinfo->rotation = rotation_o;
		}
		
		if (fortnite::temp::sticky_silent_active) {
			out_viewinfo->location = location_o;
			out_viewinfo->location.z += 40;
			out_viewinfo->rotation = fortnite::temp::sticky_silent_rotation;
		}

		if (settings::exploits::fov_changer)
			out_viewinfo->fov = settings::exploits::fov_value;
	}

	using get_player_view_point_sim = void(*)(aplayercontroller* controller, fvector* location, frotator* rotation);
	get_player_view_point_sim get_player_view_point_o = 0;

	void get_player_view_point(aplayercontroller* controller, fvector* location, frotator* rotation) {
		get_player_view_point_o(controller, location, rotation);

		location_o = *location;
		rotation_o = *rotation;

		if (fortnite::temp::silent_active) {
			*rotation = fortnite::temp::silent_rotation;
		}
		
		if (fortnite::temp::sticky_silent_active) {
			*rotation = fortnite::temp::sticky_silent_rotation;
		}
	}

	void run(aplayercontroller* controller, ulocalplayer* local_player) {
		if (local_player != essential_hooks::local_player_old) {
			essential_hooks::get_view_point_o = vmt<decltype(essential_hooks::get_view_point_o)>(std::uintptr_t(local_player), std::uintptr_t(essential_hooks::get_view_point), 0x57);

			essential_hooks::local_player_old = local_player;
		}

		if (controller != essential_hooks::player_controller_old) {
			essential_hooks::get_player_view_point_o = vmt<decltype(essential_hooks::get_player_view_point_o)>(std::uintptr_t(controller), std::uintptr_t(essential_hooks::get_player_view_point), 0xF6);

			essential_hooks::player_controller_old = controller;
		}
	}
}

void elysian_main(ugameviewportclient* viewport, ucanvas* canvas) {
	auto screen_size = fvector2d((double)canvas->clip_x(), (double)canvas->clip_y());
	auto center = fvector2d((double)screen_size.x / 2, (double)screen_size.y / 2);
	bool player_silent = false;

	auto world = viewport->get_world();
	if (!world) return;

	auto game_instance = defines::game_statics->get_game_instance(world);
	if (!game_instance) return;

	auto local_player = game_instance->get_local_players()[0];
	if (!game_instance) return;

	auto controller = defines::game_statics->get_player_controller(world, std::int32_t(0));
	if (!controller) return;

	auto camera_manager = defines::game_statics->get_player_camera_manager(world, std::int32_t(0));
	if (!camera_manager) return;

	if (settings::exploits::enable_hooks) {
		essential_hooks::run(controller, local_player);
	}

	render::world = world;
	render::canvas = canvas;
	render::controller = controller;
	render::screen_center = center;
	render::screen_size = screen_size;

	auto camera_location = camera_manager->get_camera_location();
	auto camera_rotation = camera_manager->get_camera_rotation();
	auto camera_fov = camera_manager->get_fov_angle();

	auto radius = (settings::aimbot::field_of_view * center.x / (double)camera_fov) / 2;

	auto local_pawn_holding_pickaxe = false;
	auto local_pawn = controller->get_pawn();
	auto local_team = (char)6969;
	if (local_pawn) {
		local_team = local_pawn->get_team();

		if (auto current_weapon = local_pawn->get_current_weapon()) {
			if (auto weapon_data = current_weapon->get_weapon_data()) {
				auto display_name = weapon_data->display_name();
				if (display_name.data && display_name.c_str()) {
					render::text(display_name.c_str(), fvector2d(200, 200), flinearcolor(1.f, 1.f, 1.f, 1.f), true, true, true);

					if (defines::string->contains(display_name.c_str(), L"Harvesting Tool", false, false)) local_pawn_holding_pickaxe = true;
				}
			}
		}
	}
	
	if (settings::exploits::spinbot && fortnite::temp::sticky_silent_active && local_pawn) {
		static double spin = 0;
		
		if (spin > 359)
			spin = 0;
		spin += 1;

		local_pawn->set_actor_rotation(frotator(0, spin, 0), false);
	}

	if (settings::radar::enable) {
		fortnite::radar::radar_initialize(fvector2d(settings::radar::positionx, settings::radar::positiony), fvector2d(settings::radar::size, settings::radar::size), camera_location, camera_rotation);
	}

	if (settings::player::enable) {
		double closest_distance = DBL_MAX;
		player_pawn* target_player = 0;

		tarray<uobject*> player_array = defines::game_statics->get_all_actors_of_class(world, defines::actor_player_class);
		for (int i = 0; i < player_array.size(); i++) {
			if (!player_array.valid(i)) continue;

			auto player = (player_pawn*)player_array[i];
			if (player == local_pawn) continue;
			if (!player) continue;

			if (player->is_dead()) continue;

			auto object_name = defines::system->get_object_name(player);
			if (!defines::string->contains(object_name.c_str(), L"PlayerPawn", false, false)) continue;

			bool teammate = player->get_team() == local_team;

			auto esp_color = flinearcolor(1.f, 1.f, 1.f, 1.f);

			if (teammate)
				esp_color = flinearcolor(0.4f, 0.7f, 0.5f, 1.f);

			auto world_location = player->get_actor_location();

			if (settings::radar::enable && settings::radar::players) {
				fortnite::radar::add_to_radar(world_location, esp_color);
			}

			auto max_distance = settings::player::max_distance;
			double distance = defines::math->vector_distance(world_location, camera_location) * 0.01;
			if (distance > max_distance) continue;

			bool visible = player->was_recently_rendered(0.f);

			auto mesh = player->get_pawn_mesh();
			if (!mesh) continue;

			if (!fortnite::get_all_bones(controller, mesh)) continue;

			if (settings::player::skeleton) {
				render::line(fortnite::s_head, fortnite::s_neck, esp_color, 1.f);
				render::line(fortnite::s_neck, fortnite::s_chest, esp_color, 1.f);
				render::line(fortnite::s_chest, fortnite::s_pelvis, esp_color, 1.f);

				render::line(fortnite::s_chest, fortnite::s_rshoulder, esp_color, 1.f);
				render::line(fortnite::s_rshoulder, fortnite::s_relbow, esp_color, 1.f);
				render::line(fortnite::s_relbow, fortnite::s_rhand, esp_color, 1.f);
				render::line(fortnite::s_pelvis, fortnite::s_rthigh, esp_color, 1.f);
				render::line(fortnite::s_rthigh, fortnite::s_rknee, esp_color, 1.f);
				render::line(fortnite::s_rknee, fortnite::s_rfoot, esp_color, 1.f);
				render::line(fortnite::s_rfoot, fortnite::s_rfootout, esp_color, 1.f);

				render::line(fortnite::s_chest, fortnite::s_lshoulder, esp_color, 1.f);
				render::line(fortnite::s_lshoulder, fortnite::s_lelbow, esp_color, 1.f);
				render::line(fortnite::s_lelbow, fortnite::s_lhand, esp_color, 1.f);
				render::line(fortnite::s_pelvis, fortnite::s_lthigh, esp_color, 1.f);
				render::line(fortnite::s_lthigh, fortnite::s_lknee, esp_color, 1.f);
				render::line(fortnite::s_lknee, fortnite::s_lfoot, esp_color, 1.f);
				render::line(fortnite::s_lfoot, fortnite::s_lfootout, esp_color, 1.f);
			}

			fvector2d bones_to_check[] = { fortnite::s_bottom, fortnite::s_head, fortnite::s_head_2, fortnite::s_neck, fortnite::s_chest, fortnite::s_pelvis, fortnite::s_rshoulder, fortnite::s_relbow, fortnite::s_rhand, fortnite::s_rthigh, fortnite::s_rknee, fortnite::s_rfoot, fortnite::s_rfootout, fortnite::s_lshoulder, fortnite::s_lelbow,fortnite::s_lhand, fortnite::s_lthigh, fortnite::s_lknee, fortnite::s_lfoot, fortnite::s_lfootout };

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
			auto offset = calculated_distance * 0.035;
			auto corner_wide = actor_width / 3;
			auto corner_heit = actor_height / 3;

			auto text_offset = double(0);
			fvector2d bottom_middle = fvector2d(most_left + actor_width / 2, fortnite::s_bottom_2.y);

			if (settings::player::box) {
				if (settings::player::cornered_box) {
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
				else if (settings::player::normal_box) {
					//left line
					render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_left - offset, most_bottom + offset), esp_color, 1.f);

					//right line
					render::line(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset, most_bottom + offset), esp_color, 1.f);

					//top line
					render::line(fvector2d(most_left - offset, most_top - offset), fvector2d(most_right + offset, most_top - offset), esp_color, 1.f);

					//bottom line
					render::line(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_right + offset, most_bottom + offset), esp_color, 1.f);
				}
			}
			
			if (settings::player::player_name) {
				render::text(defines::lib->get_player_name_safe(player).c_str(), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), flinearcolor(1.f, 1.f, 0.f, 1.f), true, false, settings::style::text_outlined);
				text_offset += 15;
			}

			if (settings::player::display_distance) {
				auto final = defines::string->build_string_double(L"", L"", defines::math->round(distance), L"m");
				render::text(final.c_str(), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), flinearcolor(1.f, 0.f, 0.f, 1.f), true, false, settings::style::text_outlined);
				text_offset += 15;
			}

			if (settings::player::active_weapon) {
				if (auto current_weapon = player->get_current_weapon()) {
					if (auto weapon_data = current_weapon->get_weapon_data()) {
						auto display_name = weapon_data->display_name();
						if (display_name.data && display_name.c_str()) {
							auto tier = weapon_data->get_tier();

							auto render_color = flinearcolor();
							if (tier == fort_item_tier::I) render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
							else if (tier == fort_item_tier::II) render_color = flinearcolor(0.2f, 0.8f, 0.4f, 1.f);
							else if (tier == fort_item_tier::III) render_color = flinearcolor(0.f, 0.4f, 0.8f, 1.f);
							else if (tier == fort_item_tier::IV) render_color = flinearcolor(1.f, 0.f, 1.f, 1.f);
							else if (tier == fort_item_tier::V) render_color = flinearcolor(0.7f, 0.7f, 0.f, 1.f);
							else if (tier == fort_item_tier::VI) render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);

							if (render_color) {
								auto a = defines::string->build_string_int(display_name.c_str(), L" [", current_weapon->get_magazine_ammo_count(), L"");
								auto b = defines::string->build_string_int(L"", L"/", current_weapon->get_bullets_per_clip(), L"]");
								auto c = defines::string->concat_str_str(a, b);
								render::text(c.c_str(), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), render_color, true, false, settings::style::text_outlined);
								text_offset += 15;
							}
						}
					}
				}
			}

			if (settings::player::snapline) {
				render::line(fvector2d(center.x, screen_size.y - 15), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), esp_color, 1.f);
			}

			if ( settings::player::eyes )
			{
				frotator _Angle = player->get_actor_rotation( );

				fvector test2, headpos;

				headpos = fortnite::head;

				angle_rotation( _Angle, &test2 );
				test2.x *= 50;
				test2.y *= 50;
				test2.z *= 50;

				fvector end = headpos + test2;
				fvector2d test1, test3;

				if ( controller->w2s( headpos, &test1 ) )
				{
					if ( controller->w2s( end, &test3 ) )
					{
						render::line( fvector2d( test1.x, test1.y ), fvector2d( test3.x, test3.y ), { 1.0f, 0.0f, 0.0f, 1.0f }, 1.f );
					}
				}
			}

			if (!teammate && !local_pawn_holding_pickaxe && ((settings::aimbot::visible_only && visible) || (!settings::aimbot::visible_only)) && local_pawn && render::in_circle(center.x, center.y, radius, fortnite::s_head.x, fortnite::s_head.y)) {
				double distance = defines::math->distance2d(fortnite::s_head, center);
				if (distance < closest_distance) {
					target_player = player;
					closest_distance = distance;
				}
			}
		}

		if (settings::aimbot::enable) {
			auto temp_silent_active = false;
			auto temp_silent_rotation = frotator();

			auto temp_sticky_silent_active = false;
			auto temp_sticky_silent_rotation = frotator();

			if (settings::aimbot::show_fov) {
				render::circle(center, radius, 100, flinearcolor(0.9f, 0.9f, 0.9f, 1.f));
			}



			if (target_player && local_pawn) {
				if (auto mesh = target_player->get_pawn_mesh()) {

					int32_t bone = 68;

					if ( settings::bones::head )
						bone = 68;
					else if ( settings::bones::neck )
						bone = 67;
					else if ( settings::bones::torso )
						bone = 7;
					else if ( settings::bones::pelvis )
						bone = 2;


					auto aim_location = mesh->get_bone_location( bone );

					if (settings::aimbot::target_line) {
						auto aim_screen = fvector2d();
						if (controller->w2s(aim_location, &aim_screen) && aim_screen) {
							render::line(center, aim_screen, flinearcolor(1.f, 0.f, 0.f, 1.f), 1.f);
						}
					}
					
					if ( settings::aimbot::bulletp )
					{
						tarray<uobject*> projectile_array = defines::game_statics->get_all_actors_of_class( world, defines::actor_projectile_class );
						for ( int i = 0; i < projectile_array.size( ); i++ ) {
							if ( !projectile_array.valid( i ) ) continue;

							auto projectile = ( actor* ) projectile_array [ i ];
							if ( !projectile ) continue;

							auto object_name = defines::system->get_object_name( projectile );

							if ( !defines::string->contains( object_name.c_str( ), L"Bullet", false, false ) ) 
								continue;

							projectile->set_actor_location( aim_location, false, 0, false );
						}
					}

					if (controller->is_key_down(defines::right_mouse_button)) {
						if (settings::aimbot::silent) {
							player_silent = true;
							temp_silent_active = true;
							temp_silent_rotation = defines::math->find_look_at_rotation(camera_location, aim_location);
						}
						else if (settings::aimbot::sticky_silent) {
							temp_sticky_silent_active = true;
							temp_sticky_silent_rotation = defines::math->find_look_at_rotation(camera_location, aim_location);
						}
						//else {
						//	fortnite::set_aim(world, controller, camera_location, camera_rotation, aim_location, settings::aimbot::interpolate, settings::aimbot::constant, settings::aimbot::interpolate_speed);
						//}
					}
				}
			}

			fortnite::temp::silent_active = temp_silent_active;
			fortnite::temp::silent_rotation = temp_silent_rotation;

			fortnite::temp::sticky_silent_active = temp_sticky_silent_active;
			fortnite::temp::sticky_silent_rotation = temp_sticky_silent_rotation;
		}
	}

	if ( settings::vehicles::enable )
	{
		tarray<uobject*> vehicle_array = defines::game_statics->get_all_actors_of_class( world, defines::actor_vehicle_class );
		for ( int i = 0; i < vehicle_array.size( ); i++ ) {
			if ( !vehicle_array.valid( i ) ) continue;

			auto vehicle = ( vehicle_actor* ) vehicle_array [ i ];
			if ( !vehicle ) continue;

			auto screen_location = fvector2d( );
			auto world_location = vehicle->get_actor_location( );

			auto distance = defines::math->vector_distance( camera_location, world_location ) * 0.01;

			if ( distance > settings::vehicles::max_distance ) continue;

			if ( controller->w2s( world_location, &screen_location ) ) {
				render::text( vehicle->get_display_name( ).c_str( ), screen_location, flinearcolor( 1.f, 0.f, 1.f, 1.f ), true, true, settings::style::text_outlined );
			}
		}
	}

	if (settings::weakspot::enable) {
		weakspot_actor* target_weakspot = 0;
		tarray<uobject*> weakspot_array = defines::game_statics->get_all_actors_of_class(world, defines::actor_weakspot_class);
		for (int i = 0; i < weakspot_array.size(); i++) {
			if (!weakspot_array.valid(i)) continue;

			auto weakspot = (weakspot_actor*)weakspot_array[i];
			if (!weakspot) continue;

			if (!weakspot->is_active()) continue;

			target_weakspot = weakspot;
		}

		auto temp_silent_active = false;
		auto temp_silent_rotation = frotator();

		if (target_weakspot) {
			auto aim_location = target_weakspot->get_actor_location();
			auto aim_screen = fvector2d();
			if (controller->w2s(aim_location, &aim_screen) && aim_screen) {

				render::text(L"X", aim_screen, flinearcolor(1.f, 0.f, 0.f, 1.f), true, true, settings::style::text_outlined);

				if (settings::weakspot::weakspot_aimbot && local_pawn && local_pawn_holding_pickaxe) {
					if (settings::weakspot::target_line) {
						render::line(center, aim_screen, flinearcolor(1.f, 0.f, 0.f, 1.f), 1.f);
					}

					if (controller->is_key_down(defines::left_mouse_button)) {
						if (settings::weakspot::silent) {
							temp_silent_active = true;
							temp_silent_rotation = defines::math->find_look_at_rotation(camera_location, aim_location);
						}
						else {
							fortnite::set_aim(world, controller, camera_location, camera_rotation, aim_location, settings::weakspot::interpolate, settings::weakspot::constant, settings::weakspot::interpolate_speed);
						}
					}
				}
			}
		}

		if (!player_silent) {
			fortnite::temp::silent_active = temp_silent_active;
			fortnite::temp::silent_rotation = temp_silent_rotation;
		}
	}

	if (settings::pickups::enable) {
		tarray<uobject*> pickup_array = defines::game_statics->get_all_actors_of_class(world, defines::actor_pickup_class);
		for (int i = 0; i < pickup_array.size(); i++) {
			if (!pickup_array.valid(i)) continue;

			auto pickup = (pickup_actor*)pickup_array[i];
			if (!pickup) continue;

			auto screen_location = fvector2d();
			auto world_location = pickup->get_actor_location();
			auto distance = defines::math->vector_distance(camera_location, world_location) * 0.01;

			if (distance > settings::pickups::max_distance) continue;

			if (auto definition = pickup->get_item_definition()) {
				auto display_name = definition->display_name();
				if (display_name.data && display_name.c_str()) {
					auto render_color = flinearcolor();
					auto tier = definition->get_tier();

					if (settings::pickups::common && tier == fort_item_tier::I) render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
					else if (settings::pickups::uncommon && tier == fort_item_tier::II) render_color = flinearcolor(0.2f, 0.8f, 0.4f, 1.f);
					else if (settings::pickups::rare && tier == fort_item_tier::III) render_color = flinearcolor(0.f, 0.4f, 0.8f, 1.f);
					else if (settings::pickups::epic && tier == fort_item_tier::IV) render_color = flinearcolor(1.f, 0.f, 1.f, 1.f);
					else if (settings::pickups::legendary && tier == fort_item_tier::V) render_color = flinearcolor(0.7f, 0.7f, 0.f, 1.f);
					else if (settings::pickups::mythic && tier == fort_item_tier::VI) render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);

					if (settings::radar::enable && settings::radar::pickups && render_color) {
						fortnite::radar::add_to_radar(world_location, render_color);
					}

					if (controller->w2s(world_location, &screen_location) && screen_location) {
						if (render_color) {
							if (settings::pickups::lines && !render::in_circle(center.x, center.y, settings::pickups::line_offset, screen_location.x, screen_location.y)) {
								double angle = defines::math->atan2(screen_location.y - center.y, screen_location.x - center.x);

								double x = settings::pickups::line_offset * defines::math->cos(angle) + center.x;
								double y = settings::pickups::line_offset * defines::math->sin(angle) + center.y;

								fvector2d end_pos = fvector2d(x, y);
								render::line(end_pos, fvector2d(screen_location.x, screen_location.y + 8), render_color, 1.f);
							}

							if (settings::pickups::display_distance) {
								auto final = defines::string->build_string_double(display_name.c_str(), L"(", defines::math->round(distance), L"m)");
								render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
							}
							else render::text(display_name.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
						}
					}
				}
			}
		}
	}

	if (settings::containers::enable) {
		tarray<uobject*> container_array = defines::game_statics->get_all_actors_of_class(world, defines::actor_container_class);
		for (int i = 0; i < container_array.size(); i++) {
			if (!container_array.valid(i)) continue;

			auto container = (container_actor*)container_array[i];
			if (!container) continue;

			if (settings::containers::ignore_opened && container->already_searched()) continue;

			auto object_name = defines::system->get_object_name(container);

			auto screen_location = fvector2d();
			auto world_location = container->get_actor_location();
			auto distance = defines::math->vector_distance(camera_location, world_location) * 0.01;

			if (distance > settings::containers::max_distance) continue;

			auto render_color = flinearcolor(1.f, 1.f, 1.f, 1.f);
			if (controller->w2s(world_location, &screen_location) && screen_location) {
				if (settings::containers::chest && defines::string->contains(object_name.c_str(), L"Tiered_Chest", false, false)) {
					render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Chest ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Chest", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::ammo_box && defines::string->contains(object_name.c_str(), L"Tiered_Ammo", false, false)) {
					render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Ammo box ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Ammo box", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::chest && defines::string->contains(object_name.c_str(), L"AlwaysSpawn_NormalChest", false, false)) {
					render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Basic chest ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Basic chest", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::cooler_and_food && defines::string->contains(object_name.c_str(), L"Cooler_Container", false, false)) {
					render_color = flinearcolor(0.f, 1.f, 1.f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Cooler ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Cooler", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::cooler_and_food && defines::string->contains(object_name.c_str(), L"FoodBox_Produce", false, false)) {
					render_color = flinearcolor(0.f, 1.f, 0.4f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Food box ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Food box", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::safe && defines::string->contains(object_name.c_str(), L"Tiered_Safe", false, false)) {
					render_color = flinearcolor(1.f, 0.f, 0.f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Safe ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Safe", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::others && defines::string->contains(object_name.c_str(), L"Barrel_FishingRod", false, false)) {
					render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Fishing rod ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Fishing rod", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::others && defines::string->contains(object_name.c_str(), L"CashRegister", false, false)) {
					render_color = flinearcolor(0.4f, 0.4f, 0.4f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Cash register ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Cash register", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				else if (settings::containers::chest && defines::string->contains(object_name.c_str(), L"BuriedChest", false, false)) {
					render_color = flinearcolor(1.f, 1.f, 0.f, 1.f);
					if (settings::containers::display_distance) {
						auto final = defines::string->build_string_double(L"Hidden chest ", L"(", defines::math->round(distance), L"m)");
						render::text(final.c_str(), screen_location, render_color, true, true, settings::style::text_outlined);
					}
					else render::text(L"Hidden chest", screen_location, render_color, true, true, settings::style::text_outlined);
				}
				if (settings::containers::lines && render_color && !render::in_circle(center.x, center.y, settings::containers::line_offset, screen_location.x, screen_location.y)) {
					double angle = defines::math->atan2(screen_location.y - center.y, screen_location.x - center.x);

					double x = settings::containers::line_offset * defines::math->cos(angle) + center.x;
					double y = settings::containers::line_offset * defines::math->sin(angle) + center.y;

					fvector2d end_pos = fvector2d(x, y);
					render::line(end_pos, fvector2d(screen_location.x, screen_location.y + 8), render_color, 1.f);
				}
			}

			if (settings::radar::enable && settings::radar::containers && render_color) {
				fortnite::radar::add_to_radar(world_location, render_color);
			}
		}
	}

	render::draw_menu(local_pawn);
}