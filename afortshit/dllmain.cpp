#include "fn/include.h"

/*using updatetargetview_org = void(*)(classes::camera_manager* camera_manager, structs::ftviewtarget& OutVT, float DeltaTime);
updatetargetview_org updatetargetview_o = 0;

void updatetargetview(classes::camera_manager* camera_manager, structs::ftviewtarget& OutVT, float DeltaTime)*/

//getplayerviewpoint = 0xD2

namespace global {
	uobject* player_class = nullptr;
	uobject* projectile_class = nullptr;

	fvector camera_location;

	fvector org_location;
	frotator org_rotation;

	player_pawn* target_pawn = 0;
}

namespace keys {
	fkey left_mouse;
	fkey right_mouse;
	fkey insert;
	fkey space;
	fkey capslock;
	fkey dot;
	fkey slash;
	fkey enter;
	fkey w, a, s, d;
}

namespace renderer
{
	uobject* font;

	void text(ucanvas* canvas, fstring content, fvector2d location, flinearcolor color, bool center, bool outline) {
		canvas->k2_drawtext(font, content, location, { 0.8f, 0.8f }, color, 0.8f, { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f }, center, center, outline, { 0.f, 0.f, 0.f, 1.f });
	}

	void text_scale(ucanvas* canvas, fstring content, fvector2d location, flinearcolor color, bool center, bool outline, float scale) {
		canvas->k2_drawtext(font, content, location, { scale, scale }, color, 0.8f, { 0.f, 0.f, 0.f, 0.f }, { 0.f, 0.f }, center, center, outline, { 0.f, 0.f, 0.f, 1.f });
	}

	void line(ucanvas* canvas, fvector2d from, fvector2d to, float thickness, flinearcolor color) {
		canvas->k2_drawline(from, to, thickness, color);
	}

	void circle(ucanvas* canvas, fvector2d center, float radius, float segments, float thickness, flinearcolor color) {
		float pi = 3.1415927f;

		float step = 3.1415927f * 2.0 / segments;
		int count = 0;
		fvector2d v[64];

		for (float a = 0; a < 3.1415927f * 2.0; a += step) {
			float x1 = radius * cos(a) + center.x;
			float y1 = radius * sin(a) + center.y;
			float x2 = radius * cos(a + step) + center.x;
			float y2 = radius * sin(a + step) + center.y;
			v[count].x = x1;
			v[count].y = y1;
			v[count + 1].x = x2;
			v[count + 1].y = y2;

			line(canvas, fvector2d{ v[count].x, v[count].y }, fvector2d{ x2, y2 }, thickness, color);
		}
	}

	void filled_circle(ucanvas* canvas, fvector2d center, float radius, float segments, float thickness, flinearcolor color) {
		float pi = 3.1415927f;

		float step = 3.1415927f * 2.0 / segments;
		int count = 0;
		fvector2d v[64];

		for (float a = 0; a < 3.1415927f * 2.0; a += step) {
			float x1 = radius * cos(a) + center.x;
			float y1 = radius * sin(a) + center.y;
			float x2 = radius * cos(a + step) + center.x;
			float y2 = radius * sin(a + step) + center.y;
			v[count].x = x1;
			v[count].y = y1;
			v[count + 1].x = x2;
			v[count + 1].y = y2;

			line(canvas, fvector2d{ v[count].x, v[count].y }, fvector2d{ center.x, center.y }, thickness, color);
		}
	}


	fvector2d calculate_tracer(fvector2d center, float offset, float angle) {
		return { center.x + cosf(angle) * offset, center.y + sinf(angle) * offset };
	}

	void filled_box(ucanvas* canvas, fvector2d position, fvector2d size, flinearcolor color) {
		for (float i = 0.f; i < size.y; i += 1.0f)
		{
			canvas->k2_drawline({ position.x, position.y + i }, { position.x + size.x, position.y + i }, 1.f, color);
		}
	}

	class gui {
	public:
		bool initialize() {
			this->color_background = { 0.f, 0.f, 0.f, 1.f };
			this->color_titlebar = { 0.f, 0.f, 0.f, 1.f };
			this->color_text = { 1.f, 1.f, 1.f, 1.f };
			this->color_text_ex = { 0.f, 1.f, 1.f, 1.f };
			this->color_frame = { 0.8f, 0.8f, 0.8f, 1.f };
			this->color_frame_hovered = { 0.7f, 0.7f, 0.7f, 1.f };
			this->color_checkmark = { 0.f, 1.f, 1.f, 1.f };
			this->color_slider = { 0.f, 1.f, 1.f, 1.f };
			this->color_button = { 0.8f, 0.8f, 0.8f, 1.f };
			this->color_button_hovered = { 0.7f, 0.7f, 0.7f, 1.f };
			this->color_button_clicked = { 0.6f, 0.6f, 0.6f, 1.f };

			this->item_spacing = 5.f;

			return true;
		}

		void set_controller(acontroller* controller) {
			this->menu_controller = controller;
		}

		void set_canvas(ucanvas* canvas) {
			this->menu_canvas = canvas;
		}

		fvector2d get_menu_location() {
			return this->menu_start_location;
		}

		fvector2d get_jump_location() {
			return this->jump_location;
		}

		float get_item_spacing() {
			return this->item_spacing;
		}

		void add_jumper(fvector2d jump_value) {
			this->jump_location.x += jump_value.x;
			this->jump_location.y += jump_value.y;
		}

		void set_jumper(fvector2d jump_value) {
			this->jump_location.x = jump_value.x;
			this->jump_location.y = jump_value.y;
		}

		int get_tab() {
			return this->current_tab;
		}

		void set_tab(int value) {
			this->current_tab = value;
		}

		fvector2d get_cursor_location() {
			fvector2d cursor_location = { };
			this->menu_controller->get_mouse_position(&cursor_location.x, &cursor_location.y);
			return cursor_location;
		}

		bool cursor_in_area(fvector2d Pos, fvector2d Size) {
			if (this->cursor_location.x > Pos.x && this->cursor_location.y > Pos.y)
				if (this->cursor_location.x < Pos.x + Size.x && this->cursor_location.y < Pos.y + Size.y)
					return true;
			return false;
		}

		bool update_input() {
			//if (this->menu_controller->is_input_key_down(keys::left_mouse))
			if (GetAsyncKeyState(1))
				this->is_mouse_down = true;
			else
				this->is_mouse_down = false;

			if (this->is_mouse_down) {
				if (!this->is_mouse_down_old) {
					this->is_mouse_clicked = true;
					this->is_mouse_down_old = true;
				}
				else {
					this->is_mouse_clicked = false;
				}
			}
			else {
				this->is_mouse_down_old = false;
				this->is_mouse_clicked = false;
			}

			this->cursor_location = this->get_cursor_location();

			return true;
		}

		void check_box(fstring text, bool* option) {
			float size = 15.f;

			fvector2d current_position = this->jump_location;
			flinearcolor frame_clr = this->color_frame;

			bool hovered = cursor_in_area(current_position, { size, size });
			bool clicked = this->is_mouse_clicked && hovered;
			if (hovered)
				frame_clr = this->color_frame_hovered;

			if (clicked)
				*option = !*option;

			renderer::filled_box(this->menu_canvas, current_position, { size, size }, frame_clr);

			if (*option)
				renderer::filled_box(this->menu_canvas, { current_position.x + 4.f, current_position.y + 4.f }, { size - 8.f, size - 8.f }, this->color_checkmark);

			renderer::text(this->menu_canvas, text, { current_position.x + 20.f, current_position.y }, this->color_text, false, false);

			if (this->jumper)
				this->jump_location.y += size + this->item_spacing;
		}

		void slider(fstring text, float* value, float min, float max) {
			float size_x = 170.f;
			float size_y = 10.f;
			flinearcolor frame_clr = this->color_frame;

			fvector2d current_pos = this->jump_location;
			bool hovered = this->cursor_in_area(current_pos, { size_x, size_y });
			bool pressed = this->is_mouse_down && hovered;
			if (hovered)
				frame_clr = this->color_frame_hovered;

			if (pressed) {
				*value = ((this->cursor_location.x - current_pos.x) * ((max - min) / size_x)) + min;
				if (*value < min) *value = min;
				if (*value > max) *value = max;
			}

			renderer::filled_box(this->menu_canvas, current_pos, { size_x, size_y }, frame_clr);

			float one = size_x / (max - min);
			fvector2d slide_location = { one * (*value - min), size_y };
			renderer::filled_box(this->menu_canvas, { current_pos.x + slide_location.x, current_pos.y }, { 5.f, slide_location.y }, this->color_slider);
			renderer::text(this->menu_canvas, text, { current_pos.x + size_x + 5.f, current_pos.y - (size_y / 2.f) + 2.f }, this->color_text, false, false);

			if (this->jumper)
				this->jump_location.y += size_y + this->item_spacing;
		}

		bool button(fstring text, fvector2d size) {
			fvector2d current_position = this->jump_location;
			flinearcolor clr = this->color_button;

			bool hovered = this->cursor_in_area(current_position, size);
			bool clicked = this->is_mouse_clicked && hovered;
			if (this->is_mouse_down && hovered)
				clr = this->color_button_clicked;
			else if (hovered)
				clr = this->color_button_hovered;

			renderer::filled_box(this->menu_canvas, current_position, size, clr);
			renderer::text(this->menu_canvas, text, { current_position.x + (size.x / 2.f), current_position.y + 12.f }, this->color_text, true, false);

			if (this->jumper)
				this->jump_location.y += size.y + this->item_spacing;

			return clicked;
		}

		bool button_ex(fstring text, fvector2d location, fvector2d size) {
			fvector2d current_position = location;
			flinearcolor clr = this->color_button;

			bool hovered = this->cursor_in_area(current_position, size);
			bool clicked = this->is_mouse_clicked && hovered;
			if (this->is_mouse_down && hovered)
				clr = this->color_button_clicked;
			else if (hovered)
				clr = this->color_button_hovered;

			renderer::filled_box(this->menu_canvas, current_position, size, clr);
			renderer::text(this->menu_canvas, text, { current_position.x + (size.x / 2.f), current_position.y + 12.f }, this->color_text, true, false);

			return clicked;
		}

		void text(fstring text) {
			fvector2d current_position = this->jump_location;

			renderer::text(this->menu_canvas, text, { current_position.x, current_position.y }, this->color_text, false, false);

			if (this->jumper)
				this->jump_location.y += 15.f + this->item_spacing;
		}

		void text_colored(fstring text, flinearcolor color) {
			fvector2d current_position = this->jump_location;

			renderer::text(this->menu_canvas, text, { current_position.x, current_position.y }, color, false, false);

			if (this->jumper)
				this->jump_location.y += 15.f + this->item_spacing;
		}

		void enable_jumper() {
			this->jumper = true;
		}

		void disable_jumper() {
			this->jumper = false;
		}

		void default_page_x() {
			this->set_jumper({ this->menu_start_location.x + 15.f, this->get_jump_location().y });
		}

		void next_page() {
			this->set_jumper({ this->menu_start_location.x + (this->menu_size.x / 2.f), this->menu_start_location.y + 30.f + 25.f + this->get_item_spacing() });
		}

		void next(float offset_right) {
			this->add_jumper({ offset_right, 0.f });
		}

		bool begin(fstring title, fvector2d location, fvector2d size) {
			this->title = title;
			this->menu_start_location = location;
			this->menu_size = size;
			this->jumper = true;
			this->jump_location = { location.x + 15.f, location.y + 30.f };
			if (!this->initialized)
				this->initialized = this->initialize();
			this->update_input();

			renderer::filled_box(this->menu_canvas, location, size, this->color_background);
			renderer::filled_box(this->menu_canvas, location, { size.x, 18.f }, this->color_titlebar);
			renderer::text(this->menu_canvas, title, { location.x, location.y + 1.f }, this->color_text_ex, false, false);

			return true;
		}

		bool end() {
			renderer::filled_box(this->menu_canvas, { this->cursor_location.x - 5.f, this->cursor_location.y - 5.f }, { 5.f, 5.f }, { 0.f, 0.f, 0.f, 1.f });

			return true;
		}
	private:
		acontroller* menu_controller;
		ucanvas* menu_canvas;

		flinearcolor color_background;
		flinearcolor color_titlebar;
		flinearcolor color_text;
		flinearcolor color_text_ex;
		flinearcolor color_frame;
		flinearcolor color_frame_hovered;
		flinearcolor color_checkmark;
		flinearcolor color_slider;
		flinearcolor color_button;
		flinearcolor color_button_hovered;
		flinearcolor color_button_clicked;

		fstring title;

		fvector2d menu_start_location;
		fvector2d menu_size;
		fvector2d cursor_location;
		fvector2d jump_location;

		bool initialized = false;
		bool is_mouse_down = false;
		bool is_mouse_down_old = false;
		bool is_mouse_clicked = false;
		bool jumper = false;

		int current_tab;

		float item_spacing;
	};
}

bool test_show_menu = true;
bool cool_boatfly = true;

float deg_2_rad(float degrees) {
	float radians;
	radians = degrees * (3.14159265358979323846 / 180);
	return radians;
}

void angle_vectors(const fvector& angles, fvector* forward) {
	float sp, sy, cp, cy;
	sy = sin(deg_2_rad(angles.y));
	cy = cos(deg_2_rad(angles.y));
	sp = sin(deg_2_rad(angles.x));
	cp = cos(deg_2_rad(angles.x));
	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

using draw_transition_org = void(*)(ugameviewport* viewport, ucanvas* canvas);
draw_transition_org draw_transition_o = 0;

using get_view_point_org = void(*)(ulocalplayer* localplayer, fminimalviewinfo* info, BYTE stereo_pass);
get_view_point_org get_view_point_o = 0;

using get_player_view_point_org = void(*)(acontroller* controller, fvector* location, frotator* rotation);
get_player_view_point_org get_player_view_point_o = 0;

void get_view_point(ulocalplayer* localplayer, fminimalviewinfo* info, BYTE stereo_pass) {
	get_view_point_o(localplayer, info, stereo_pass);

	global::camera_location = info->location;

	auto pawn = global::target_pawn;
	if (pawn && GetAsyncKeyState(2)) {
		info->location = global::org_location;
		info->rotation = global::org_rotation;
	}

	info->fov = 110.f;
}

void get_player_view_point(acontroller* controller, fvector* location, frotator* rotation) {
	get_player_view_point_o(controller, location, rotation);

	global::org_location = *location;
	global::org_rotation = *rotation;

	auto pawn = global::target_pawn;
	if (pawn && GetAsyncKeyState(2)) {
		auto mesh = pawn->mesh;
		if (mesh) {
			auto rootHead = mesh->get_bone_location(68);
			fvector camloc = *location;
			fvector VectorPos;
			VectorPos.x = rootHead.x - camloc.x;
			VectorPos.y = rootHead.y - camloc.y;
			VectorPos.z = rootHead.z - camloc.z;
			float distance = (double)(sqrtf(VectorPos.x * VectorPos.x + VectorPos.y * VectorPos.y + VectorPos.z * VectorPos.z));

			frotator rot;
			rot.pitch = -((acosf(VectorPos.z / distance) * (float)(180.0f / 3.14159265358979323846264338327950288419716939937510)) - 90.f);
			rot.yaw = atan2f(VectorPos.y, VectorPos.x) * (float)(180.0f / 3.14159265358979323846264338327950288419716939937510);
			rot.roll = 0;

			*rotation = rot;
		}
	}
}

bool in_circle(int cx, int cy, int r, int x, int y) {
	int dist = (x - cx) * (x - cx) + (y - cy) * (y - cy);
	if (dist <= r * r)
		return true;
	else
		return false;
}

acontroller* controller_old = nullptr;
ulocalplayer* localplayer_old = nullptr;

void draw_transition(ugameviewport* viewport, ucanvas* canvas) {
	draw_transition_o(viewport, canvas);

	if (!viewport) return;
	if (!canvas) return;

	float center_x = canvas->clip_x / 2.f;
	float center_y = canvas->clip_y / 2.f;
	if (!center_x || !center_y) return;

	auto world = (uworld*)viewport->world;
	if (!world) return;

	auto game_instance = classes::gameplaystatistics->get_game_instance(world);
	if (!game_instance) return;

	auto localplayer = game_instance->localplayers[0];
	if (!localplayer) return;

	if (localplayer != localplayer_old) {
		//0x58 if no work
		hook::vmt((void*)(std::uintptr_t)localplayer, get_view_point, 0x57, (void**)&get_view_point_o);
		printf("\nrehooked GetViewPoint");

		localplayer_old = localplayer;
	}

	auto controller = localplayer->controller;
	if (!controller) return;

	if (controller != controller_old) {
		hook::vmt((void*)(std::uintptr_t)controller, get_player_view_point, 0xF6, (void**)&get_player_view_point_o);
		printf("\nrehooked GetPlayerViewPoint");
		
		controller_old = controller;
	}
	renderer::circle(canvas, { center_x, center_y }, 250.f, 64.f, 1.f, { 0.8f, 0.8f, 0.8f, 0.8f });
	if (GetAsyncKeyState(VK_INSERT) & 1)
		test_show_menu = !test_show_menu;

	if (test_show_menu) {
		renderer::gui menu;

		fvector2d menu_size;
		fvector2d menu_location;
		menu_size.x = 340.f;
		menu_size.y = 600.f;
		menu_location.x = center_x - (menu_size.x / 2.f);
		menu_location.y = center_y - (menu_size.y / 2.f);
		menu.set_controller(controller);
		menu.set_canvas(canvas);
		
		menu.begin(L" Interpreter#7777", menu_location, menu_size);
		menu.text(L"Options");
		menu.check_box(L"Camera FOV", &settings::fov_changer);
		menu.check_box(L"Vehicle fly", &cool_boatfly);

		menu.next_page();
		menu.text(L"Adjustment");
		menu.slider(L"Camera FOV", &settings::camera_fov, 70.f, 140.f);
	
		/*
		if (menu.button(L"noreload", { 140.f, 25.f })) {
			//F3 0F ? ? ? ? ? ? 48 8B ? C7 44 24 20
			BYTE org[] = { 0xF3, 0x0F, 0x10, 0x90, 0xE8, 0x00, 0x00, 0x00 };

			for (int i = 0; i < 8; i++) {
				*(BYTE*)(base_address + 0x86ED75A + i) = 0x90;
			}

			MessageBoxA(0, "a", "a", 0);
		}*/
		menu.end();
	}

	auto local_pawn = controller->k2_get_pawn();

	if (cool_boatfly && local_pawn && local_pawn->is_in_vehicle()) {
		if (auto boat = local_pawn->current_vehicle) {
			if (auto camera = controller->camera) {
				double deg = 1337;

				if (GetAsyncKeyState(0x41)) //a
					deg = 90;
				if (GetAsyncKeyState(0x44)) //d
					deg = 270;
				if (GetAsyncKeyState(0x53)) //s
					deg = 0;
				if (GetAsyncKeyState(0x57)) //w
					deg = 180;

				if (deg != 1337) {
					fvector current_location = boat->k2_get_actor_location();
					frotator current_rotation = camera->get_camera_rotation();

					fvector conv_angle = { current_rotation.pitch, current_rotation.yaw + deg, current_rotation.roll };

					fvector output_angle;
					angle_vectors({ conv_angle.x, conv_angle.y, conv_angle.z }, &output_angle);

					output_angle.x *= 20;
					output_angle.y *= 20;
					output_angle.z *= 20;
					double bru = 0;

					if (GetAsyncKeyState(VK_SPACE))
						bru = 10;

					boat->k2_set_actor_location({ current_location.x - output_angle.x, current_location.y - output_angle.y, current_location.z - output_angle.z + bru }, true, 0, false);
					boat->k2_set_actor_rotation(current_rotation, true);
				}
			}
		}
	}

	tarray<uobject*> player_pawns;
	classes::gameplaystatistics->get_all_actors_of_class((uobject*)world, global::player_class, &player_pawns);
	int closest_player = 1337;

	float closest_distance = FLT_MAX;
	for (int i = 0; i < player_pawns.size(); i++) {
		auto player = (player_pawn*)player_pawns[i];
		if (!player) continue;

		if (player == local_pawn) continue;

		std::wstring object_name = classes::kismetsystem->get_object_name(player).c_str();
		if (object_name.find(L"PlayerPawn") == std::wstring::npos) continue;

		auto mesh = player->mesh;
		if (!mesh) continue;

		bool visible = true;

		/*for (int i = 0; i < 115; i++) {
			fvector loc = mesh->get_bone_location(i);
			fvector2d scren;
			if (controller->world_to_screen(loc, &scren, true)) {
				std::wstring text = std::to_wstring(i);
				renderer::text(canvas, text.c_str(), scren, { 1.f, 1.f, 1.f, 1.f }, true, false);
			}
		}*/

		flinearcolor esp_color = { 1.f, 1.f, 1.f, 1.f };
		if (visible)
			esp_color = { 0.f, 1.f, 1.f, 1.f };

		bool w2s_invalid = false;
		{
			int bone_head = 68;
			int bone_neck = 66;
			int bone_chest = 7;
			int bone_pelvis = 2;
			int bone_r_shoulder = 38;
			int bone_r_elbow = 39;
			int bone_r_hand = 40;
			int bone_l_shoulder = 35;
			int bone_l_elbow = 10;
			int bone_l_hand = 33;
			int bone_r_thigh = 78;
			int bone_r_knee = 79;
			int bone_r_foot = 82;
			int bone_r_foot_out = 83;
			int bone_l_thigh = 71;
			int bone_l_knee = 72;
			int bone_l_foot = 75;
			int bone_l_foot_out = 76;

			int32_t bone_count = mesh->get_num_bones();

			if (bone_count == 115) {
				bone_l_shoulder = 9;
			}

			fvector head_world = mesh->get_bone_location(bone_head);
			fvector2d head_screen;

			fvector neck_world = mesh->get_bone_location(bone_neck);
			fvector2d neck_screen;

			fvector chest_world = mesh->get_bone_location(bone_chest);
			chest_world.z += 7;
			fvector2d chest_screen;

			fvector pelvis_world = mesh->get_bone_location(bone_pelvis);
			fvector2d pelvis_screen;

			fvector r_shoulder_world = mesh->get_bone_location(bone_r_shoulder);
			fvector2d r_shoulder_screen;

			fvector r_elbow_world = mesh->get_bone_location(bone_r_elbow);
			fvector2d r_elbow_screen;

			fvector r_hand_world = mesh->get_bone_location(bone_r_hand);
			fvector2d r_hand_screen;

			fvector l_shoulder_world = mesh->get_bone_location(bone_l_shoulder);
			fvector2d l_shoulder_screen;

			fvector l_elbow_world = mesh->get_bone_location(bone_l_elbow);
			fvector2d l_elbow_screen;

			fvector l_hand_world = mesh->get_bone_location(bone_l_hand);
			fvector2d l_hand_screen;

			fvector r_thigh_world = mesh->get_bone_location(bone_r_thigh);
			fvector2d r_thigh_screen;

			fvector r_knee_world = mesh->get_bone_location(bone_r_knee);
			fvector2d r_knee_screen;

			fvector r_foot_world = mesh->get_bone_location(bone_r_foot);
			fvector2d r_foot_screen;

			fvector r_foot_out_world = mesh->get_bone_location(bone_r_foot_out);
			fvector2d r_foot_out_screen;

			fvector l_thigh_world = mesh->get_bone_location(bone_l_thigh);
			fvector2d l_thigh_screen;

			fvector l_knee_world = mesh->get_bone_location(bone_l_knee);
			fvector2d l_knee_screen;

			fvector l_foot_world = mesh->get_bone_location(bone_l_foot);
			fvector2d l_foot_screen;

			fvector l_foot_out_world = mesh->get_bone_location(bone_l_foot_out);
			fvector2d l_foot_out_screen;

			if (!controller->world_to_screen(head_world, &head_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(neck_world, &neck_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(chest_world, &chest_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(pelvis_world, &pelvis_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_shoulder_world, &r_shoulder_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_elbow_world, &r_elbow_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_hand_world, &r_hand_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_shoulder_world, &l_shoulder_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_elbow_world, &l_elbow_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_hand_world, &l_hand_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_thigh_world, &r_thigh_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_knee_world, &r_knee_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_foot_world, &r_foot_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(r_foot_out_world, &r_foot_out_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_thigh_world, &l_thigh_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_knee_world, &l_knee_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_foot_world, &l_foot_screen, true)) w2s_invalid = true;
			if (!controller->world_to_screen(l_foot_out_world, &l_foot_out_screen, true)) w2s_invalid = true;

			if (!w2s_invalid) {
				//std::wstring taext = std::to_wstring(bone_count);
				//renderer::text(canvas, taext.c_str(), { head_screen.x - 20.f, head_screen.y + 5.f }, { 1.f, 1.f, 1.f, 1.f }, true, false);

				//main structure
				renderer::line(canvas, head_screen, neck_screen, 1.f, esp_color);
				renderer::line(canvas, neck_screen, chest_screen, 1.f, esp_color);
				renderer::line(canvas, chest_screen, pelvis_screen, 1.f, esp_color);

				//top right
				renderer::line(canvas, chest_screen, r_shoulder_screen, 1.f, esp_color);
				renderer::line(canvas, r_shoulder_screen, r_elbow_screen, 1.f, esp_color);
				renderer::line(canvas, r_elbow_screen, r_hand_screen, 1.f, esp_color);

				//top left
				renderer::line(canvas, chest_screen, l_shoulder_screen, 1.f, esp_color);
				renderer::line(canvas, l_shoulder_screen, l_elbow_screen, 1.f, esp_color);
				renderer::line(canvas, l_elbow_screen, l_hand_screen, 1.f, esp_color);

				//bottom right
				renderer::line(canvas, pelvis_screen, r_thigh_screen, 1.f, esp_color);
				renderer::line(canvas, r_thigh_screen, r_knee_screen, 1.f, esp_color);
				renderer::line(canvas, r_knee_screen, r_foot_screen, 1.f, esp_color);
				renderer::line(canvas, r_foot_screen, r_foot_out_screen, 1.f, esp_color);

				//bottom left
				renderer::line(canvas, pelvis_screen, l_thigh_screen, 1.f, esp_color);
				renderer::line(canvas, l_thigh_screen, l_knee_screen, 1.f, esp_color);
				renderer::line(canvas, l_knee_screen, l_foot_screen, 1.f, esp_color);
				renderer::line(canvas, l_foot_screen, l_foot_out_screen, 1.f, esp_color);
			}

			auto dx = head_screen.x - center_x;
			auto dy = head_screen.y - center_y;
			auto dist = sqrtf(dx * dx + dy * dy);
			if (in_circle(center_x, center_y, 250, head_screen.x, head_screen.y) && dist < closest_distance) {
				closest_distance = dist;
				closest_player = i;
			}
		}
		
		//if (controller->line_of_sight_to(player, global::org_location, true))
		

		//fstring object_name = classes::kismetsystem->get_object_name((uobject*)player);
		//std::wstring name = object_name.c_str();
	}
	if (closest_player != 1337) {
		global::target_pawn = (player_pawn*)player_pawns[closest_player];
	}
	else global::target_pawn = 0;
		

	
	//
	//tarray<uobject*> projectile_pawns;
	//classes::gameplaystatistics->get_all_actors_of_class((uobject*)world, global::projectile_class, &projectile_pawns);
	//for (int i = 0; i < projectile_pawns.size(); i++) {
	//	auto projectile = (actor*)projectile_pawns[i];
	//	if (!projectile);

	//	std::wstring object_name = classes::kismetsystem->get_object_name(projectile).c_str();
	//	if (object_name.find(L"Bullet") != std::wstring::npos) {
	//		//if (tppos.x != 1337)
	//		auto pawn = global::target_pawn;

	//		if ( pawn )
	//		{
	//			auto mesh = pawn->mesh;
	//			if ( !mesh )
	//				continue;

	//			projectile->k2_set_actor_location( mesh->get_bone_location( 68 ), false, 0, false );
	//		}

	//		//fvector world = projectile->k2_get_actor_location();
	//		//fvector2d screen;

	//		//bool w2s_invalid = false;
	//		//if (!controller->world_to_screen(world, &screen, true)) w2s_invalid = true;

	//		//if (!w2s_invalid) {
	//		//	renderer::text(canvas, object_name.c_str(), screen, { 1.f, 1.f, 1.f, 1.f }, true, true);
	//		//}
	//	}
	//}

	//canvas->k2_drawline({ 100.f, 100.f }, { 500.f, 500.f }, 1.f, { 1.f, 1.f, 1.f, 1.f });

}

bool open_console() {
	//AllocConsole();
	//FILE* fp;
	//freopen_s(&fp, ("CONOUT$"), ("w"), stdout);

	return true;
}

bool initialize() {
	open_console();

	base_address = (std::uintptr_t)GetModuleHandleA(0);

	gworld = fn::find_world();

	classes::gameplaystatistics = (ugameplaystatistics*)uobject::static_find_object(nullptr, nullptr, L"Engine.Default__GameplayStatics", false);
	classes::kismetsystem = (ukismetsystem*)uobject::static_find_object(nullptr, nullptr, L"Engine.Default__KismetSystemLibrary", false);
	classes::kismetstring = (ukismetstring*)uobject::static_find_object(nullptr, nullptr, L"Engine.Default__KismetStringLibrary", false);
	
	//FortniteGame.FortProjectileBase
	global::player_class = uobject::static_find_object(nullptr, nullptr, L"FortniteGame.FortPawn", false);
	global::projectile_class = uobject::static_find_object(nullptr, nullptr, L"FortniteGame.FortProjectileBase", false);

	keys::left_mouse = classes::kismetstring->conv_string_to_name(L"LeftMouseButton");
	keys::right_mouse = classes::kismetstring->conv_string_to_name(L"RightMouseButton");
	keys::insert = classes::kismetstring->conv_string_to_name(L"Insert");
	keys::space = classes::kismetstring->conv_string_to_name(L"SpaceBar");
	keys::capslock = classes::kismetstring->conv_string_to_name(L"CapsLock");
	keys::dot = classes::kismetstring->conv_string_to_name(L"Period");
	keys::slash = classes::kismetstring->conv_string_to_name(L"Slash");
	keys::enter = classes::kismetstring->conv_string_to_name(L"Enter");
	keys::w = classes::kismetstring->conv_string_to_name(L"W");
	keys::a = classes::kismetstring->conv_string_to_name(L"A");
	keys::s = classes::kismetstring->conv_string_to_name(L"S");
	keys::d = classes::kismetstring->conv_string_to_name(L"D");

	auto game_instance = classes::gameplaystatistics->get_game_instance(gworld);
	auto engine = (uengine*)classes::kismetsystem->get_outer_object((uobject*)game_instance);

	renderer::font = engine->small_font;

	auto localplayer = game_instance->localplayers[0];
	auto viewport = localplayer->viewport;

	hook::vmt((void*)(std::uintptr_t)viewport, draw_transition, 0x6E, (void**)&draw_transition_o);

	return true;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == 0x1)
		initialize();

	return TRUE;
}