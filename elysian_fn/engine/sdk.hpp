#pragma once

namespace elysian_fn {
	namespace sdk {
		namespace classes {
			namespace defines {
				static uobject* font = nullptr;

				static uobject* system = nullptr;
				static uobject* game_statics = nullptr;
				static uobject* string = nullptr;
				static uobject* math = nullptr;
				static uobject* fort = nullptr;

				static uobject* player_class = nullptr;
				static uobject* pickup_class = nullptr;
			}

			static void init() {
				defines::font = uobject::find_object(L"DefaultRegularFont", reinterpret_cast<uobject*>(-1));
				defines::system = uobject::find_object(L"Engine.Default__KismetSystemLibrary");
				defines::game_statics = uobject::find_object(L"Engine.Default__GameplayStatics");
				defines::string = uobject::find_object(L"Engine.Default__KismetStringLibrary");
				defines::math = uobject::find_object(L"Engine.Default__KismetMathLibrary");
				defines::fort = uobject::find_object(L"FortniteGame.Default__FortKismetLibrary");

				defines::player_class = uobject::find_object(L"FortniteGame.FortPlayerPawn");
				defines::pickup_class = uobject::find_object(L"FortniteGame.FortPickup");
			}
		}

		class ucanvas : public uobject{
		public:
			float get_clip_x() {
				return memory::read<float>(this + 0x30);
			}

			float get_clip_y() {
				return memory::read<float>(this + 0x30 + 0x4);
			}

			void k2_drawline(fvector2d screen_position_a, fvector2d screen_position_b, float thickness, flinearcolor render_color) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.Canvas.K2_DrawLine");

				struct {
					fvector2d screen_position_a;
					fvector2d screen_position_b;
					float thickness;
					flinearcolor render_color;
				} params = { screen_position_a, screen_position_b, thickness, render_color };

				this->process_event(member, &params);
			}

			void k2_drawtext(uobject* render_font, fstring render_text, fvector2d screen_position, fvector2d scale, flinearcolor render_color, float kerning, flinearcolor shadow_color, fvector2d shadow_offset, bool center_x, bool center_y, bool outlined, flinearcolor outline_color) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.Canvas.K2_DrawText");

				struct {
					uobject* render_font;
					fstring render_text;
					fvector2d screen_position;
					fvector2d scale;
					flinearcolor render_color;
					float kerning;
					flinearcolor shadow_color;
					fvector2d shadow_offset;
					bool center_x;
					bool center_y;
					bool outlined;
					flinearcolor outline_color;
				} params = { render_font, render_text, screen_position, scale, render_color, kerning, shadow_color, shadow_offset, center_x, center_y, outlined, outline_color };

				this->process_event(member, &params);
			}
		};

		class playerstate : public uobject {
		public:
			fstring get_platform() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"FortniteGame.FortPlayerState.GetPlatform");

				return this->get<fstring>(member);
			}
		};

		class mesh : public uobject {
		public:
			fvector get_bone_location(std::int32_t index) {
				fmatrix out_matrix = { };
				reinterpret_cast<fmatrix* (*)(mesh*, fmatrix*, std::int32_t)>(game_base + offsets::bone_matrix)(this, &out_matrix, index);

				return fvector(out_matrix.m[3][0], out_matrix.m[3][1], out_matrix.m[3][2]);
			}
		};

		class item_definition : public uobject {
		public:
			fort_item_tier get_tier() {
				return memory::read<fort_item_tier>(this + 0x73);
			}

			ftext display_name() {
				return memory::read<ftext>(this + 0x90);
			}
		};

		class weapon : public uobject {
		public:
			bool is_reloading() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"FortniteGame.FortWeapon.IsReloading");

				return this->get<bool>(member);
			}
			
			int32_t get_bullets_per_clip() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"FortniteGame.FortWeapon.GetBulletsPerClip");

				return this->get<int32_t>(member);
			}
			
			int32_t get_magazine_ammo_count() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"FortniteGame.FortWeapon.GetMagazineAmmoCount");

				return this->get<int32_t>(member);
			}

			item_definition* get_weapon_data() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"FortniteGame.FortWeapon.GetWeaponData");

				return this->get<item_definition*>(member);
			}
		};

		class controller : public uobject {
		public:
			float input_pitch_scale() {
				return memory::read<float>(this + 0x534);
			}

			float input_yaw_scale() {
				return memory::read<float>(this + 0x530);
			}

			uobject* get_pawn() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.Controller.K2_GetPawn");

				struct {
					uobject* return_value;
				} params = { };

				this->process_event(member, &params);

				return params.return_value;
			}

			bool is_input_key_down(fkey key) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerController.IsInputKeyDown");

				struct {
					fkey key;
					bool return_value;
				} params = { key };

				this->process_event(member, &params);

				return params.return_value;
			}

			bool was_input_key_down(fkey key) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerController.WasInputKeyJustPressed");

				struct {
					fkey key;
					bool return_value;
				} params = { key };

				this->process_event(member, &params);

				return params.return_value;
			}

			fvector2d get_mouse_position() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerController.GetMousePosition");

				struct {
					float x;
					float y;
				} params = { };

				this->process_event(member, &params);

				return fvector2d((double)params.x, (double)params.y);
			}

			bool w2s(fvector world_location, fvector2d* screen_location) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerController.ProjectWorldLocationToScreen");

				struct {
					fvector world_location;
					fvector2d screen_location;
					bool return_value;
				} params = { world_location, fvector2d(), true };

				this->process_event(member, &params);

				*screen_location = params.screen_location;

				return params.return_value;
			}

			void add_yaw_input(float val) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerController.AddYawInput");

				struct {
					float val;
				} params = { val };

				this->process_event(member, &params);
			}

			void add_pitch_input(float val) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerController.AddPitchInput");

				struct {
					float val;
				} params = { val };

				this->process_event(member, &params);
			}
		};

		class camera_manager : public uobject {
		public:
			fvector get_camera_location() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerCameraManager.GetCameraLocation");

				return this->get<fvector>(member);
			}

			frotator get_camera_rotation() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerCameraManager.GetCameraRotation");

				return this->get<frotator>(member);
			}

			float get_fov_angle() {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.PlayerCameraManager.GetFOVAngle");

				return this->get<float>(member);
			}
		};

		class game_viewport : public uobject {
		public:
			uobject* get_world() {
				return memory::read<uobject*>(this + 0x78);
			}
		};

		class local_player : public uobject {
		public:
			game_viewport* get_viewport() {
				return memory::read<game_viewport*>(this + 0x78);
			}
		};

		class game_instance : public uobject {
		public:
			tarray<local_player*> get_local_players() {
				return memory::read<tarray<local_player*>>(this + 0x38);
			}
		};

		class system {
		public:
			static fstring get_object_name(uobject* object) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetSystemLibrary.GetObjectName");

				struct {
					uobject* object;
					fstring return_value;
				} params = { object };

				classes::defines::system->process_event(member, &params);

				return params.return_value;
			}

			static uobject* get_outer_object(uobject* object) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetSystemLibrary.GetOuterObject");

				struct {
					uobject* object;
					uobject* return_value;
				} params = { object };

				classes::defines::system->process_event(member, &params);

				return params.return_value;
			}
		};

		class game_statics {
		public:
			static game_instance* get_game_instance(uobject* world_context_object) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.GameplayStatics.GetGameInstance");

				struct {
					uobject* world_context_object;
					game_instance* return_value;
				} params = { world_context_object };

				classes::defines::game_statics->process_event(member, &params);

				return params.return_value;
			}

			static controller* get_player_controller(uobject* world_context_object) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.GameplayStatics.GetPlayerControllerFromID");

				struct {
					uobject* world_context_object;
					std::int32_t player_index;
					controller* return_value;
				} params = { world_context_object, 0 };

				classes::defines::game_statics->process_event(member, &params);

				return params.return_value;
			}

			static camera_manager* get_camera_manager(uobject* world_context_object) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.GameplayStatics.GetPlayerCameraManager");

				struct {
					uobject* world_context_object;
					std::int32_t player_index;
					camera_manager* return_value;
				} params = { world_context_object, 0 };

				classes::defines::game_statics->process_event(member, &params);

				return params.return_value;
			}

			static tarray<uobject*> get_all_actors_of_class(uobject* world_context_object, uobject* actor_class) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.GameplayStatics.GetAllActorsOfClass");

				struct {
					uobject* world_context_object;
					uobject* actor_class;
					tarray<uobject*> return_value;
				} params = { world_context_object, actor_class };

				classes::defines::game_statics->process_event(member, &params);

				return params.return_value;
			}

			static double get_world_delta_seconds(uobject* world_context_object) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.GameplayStatics.GetWorldDeltaSeconds");

				struct {
					uobject* world_context_object;
					double return_value;
				} params = { world_context_object };

				classes::defines::string->process_event(member, &params);

				return params.return_value;
			}
		};

		class string {
		public:
			static fname string_to_name(fstring in_string) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetStringLibrary.Conv_StringToName");

				struct {
					fstring in_string;
					fname return_value;
				} params = { in_string };

				classes::defines::string->process_event(member, &params);

				return params.return_value;
			}

			static bool contains(fstring search_in, fstring sub_string) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetStringLibrary.Contains");

				struct {
					fstring search_in;
					fstring sub_string;
					bool use_case;
					bool search_from_end;
					bool return_value;
				} params = { search_in, sub_string, false, false };

				classes::defines::string->process_event(member, &params);

				return params.return_value;
			}

			static fstring concat(fstring a, fstring b) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetStringLibrary.Concat_StrStr");

				struct {
					fstring a;
					fstring b;
					fstring return_value;
				} params = { a, b };

				classes::defines::string->process_event(member, &params);

				return params.return_value;
			}

			static fstring build_string_double(fstring append_to, fstring prefix, double in_double, fstring suffix) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetStringLibrary.BuildString_Double");

				struct {
					fstring append_to;
					fstring prefix;
					double in_double;
					fstring suffix;
					fstring return_value;
				} params = { append_to, prefix, in_double, suffix };

				classes::defines::string->process_event(member, &params);

				return params.return_value;
			}

			static fstring build_string_int(fstring append_to, fstring prefix, int32_t in_int, fstring suffix) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetStringLibrary.BuildString_Int");

				struct {
					fstring append_to;
					fstring prefix;
					int32_t in_int;
					fstring suffix;
					fstring return_value;
				} params = { append_to, prefix, in_int, suffix };

				classes::defines::string->process_event(member, &params);

				return params.return_value;
			}
		};

		class math {
		public:
			static double sin(double a) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.sin");

				struct {
					double a;
					double return_value;
				} params = { a };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static double cos(double a) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.cos");

				struct {
					double a;
					double return_value;
				} params = { a };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static double tan(double a) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.tan");

				struct {
					double a;
					double return_value;
				} params = { a };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static double atan2(double x, double y) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.Atan2");

				struct {
					double x;
					double y;
					double return_value;
				} params = { x, y };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static frotator interpolate(frotator current, frotator target, float delta_time, float speed) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.RInterpTo");

				struct {
					frotator current;
					frotator target;
					float delta_time;
					float speed;
					frotator return_value;
				} params = { current, target, delta_time, speed };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}
			
			static frotator interpolate_constant(frotator current, frotator target, float delta_time, float speed) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.RInterpTo_Constant");

				struct {
					frotator current;
					frotator target;
					float delta_time;
					float speed;
					frotator return_value;
				} params = { current, target, delta_time, speed };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static double vector_distance(fvector v1, fvector v2) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.Vector_Distance");

				struct {
					fvector current;
					fvector target;
					double return_value;
				} params = { v1, v2 };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static double distance_2d(fvector2d v1, fvector2d v2) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.Distance2D");

				struct {
					fvector2d current;
					fvector2d target;
					double return_value;
				} params = { v1, v2 };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}

			static frotator find_look_at_rotation(fvector start, fvector target) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"Engine.KismetMathLibrary.FindLookAtRotation");

				struct {
					fvector start;
					fvector target;
					frotator return_value;
				} params = { start, target };

				classes::defines::math->process_event(member, &params);

				return params.return_value;
			}
		};

		class fort {
		public:
			static fstring get_player_name_safe(uobject* actor) {
				static uobject* member;
				if (!member) member = uobject::find_object(L"FortniteGame.FortKismetLibrary.GetPlayerNameSafe");

				struct {
					uobject* actor;
					fstring return_value;
				} params = { actor };

				classes::defines::fort->process_event(member, &params);

				return params.return_value;
			}
		};
	}
}

using namespace elysian_fn::sdk;