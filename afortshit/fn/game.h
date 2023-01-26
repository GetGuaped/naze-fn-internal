#pragma once

namespace yatinu {
	template<class T>
	struct tarray
	{
		friend struct fstring;

	public:
		inline tarray()
		{
			data = nullptr;
			count = max = 0;
		};

		inline int size() const
		{
			return count;
		};

		inline T& operator[](int i)
		{
			return data[i];
		};

		inline const T& operator[](int i) const
		{
			return data[i];
		};

		inline bool valid_index(int i) const
		{
			return i < size();
		}

		T* data;
		int32_t count;
		int32_t max;
	};

	struct fstring : tarray<wchar_t>
	{
		inline fstring()
		{
		};

		fstring(const wchar_t* other)
		{
			max = count = *other ? wcslen(other) + 1 : 0;

			if (count)
			{
				data = const_cast<wchar_t*>(other);
			}
		};

		inline bool is_valid() const
		{
			return data != nullptr;
		}

		inline const wchar_t* c_str() const
		{
			return data;
		}
	};

	struct fname {
		fname() : comparison_index(std::int32_t()), number(std::int32_t()) {};
		fname(std::int32_t index) : comparison_index(index), number(std::int32_t()) {};

		int32_t comparison_index;
		int32_t number;
	};

	struct fkey {
		fkey() : name() {}
		fkey(fname name) : name(name) {}

		fname name;
		uint8_t details[24] = {};
	};

	struct fvector2d {
		double x, y;
	};

	struct fvector {
		double x, y, z;
	};

	struct frotator {
		double pitch, yaw, roll;
	};

	struct flinearcolor {
		float r, g, b, a;
	};

	struct fmatrix {
		double m[4][4];
	};

	struct fminimalviewinfo {
		fvector location;
		frotator rotation;
		float fov;
	};

	class uobject {
	public:
		static uobject* static_find_object(uobject* object, uobject* inouter, const wchar_t* name, bool exactclass) {
			return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*, bool)>(base_address + 0xDC0924)(object, inouter, name, exactclass);
		}

		void process_event(uobject* function, void* params) {
			return reinterpret_cast<void (*)(uobject*, uobject*, void*)>(base_address + 0x10344E8)(this, function, params);
		}
	};

	class ucanvas : public uobject {
	public:
		char pad_0[0x30];
		float clip_x;
		float clip_y;

		void k2_drawline(fvector2d screen_position_a, fvector2d screen_position_b, float thickness, flinearcolor rendercolor) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Canvas.K2_DrawLine", false);

			struct { fvector2d a1; fvector2d a2; float a3; flinearcolor a4; } params;
			params.a1 = screen_position_a;
			params.a2 = screen_position_b;
			params.a3 = thickness;
			params.a4 = rendercolor;

			this->process_event(function, &params);
			return;
		}

		void k2_drawtext(uobject* render_font, fstring render_text, fvector2d screen_position, fvector2d scale, flinearcolor render_color, float kerning, flinearcolor shadow_color, fvector2d shadow_offset, bool bcentrex, bool bcentrey, bool boutlined, flinearcolor outline_color) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Canvas.K2_DrawText", false);

			struct { uobject* a1; fstring a2; fvector2d a3; fvector2d a4; flinearcolor a5; float a6; flinearcolor a7; fvector2d a8; bool a9; bool a10; bool a11; flinearcolor a12; } params;
			params.a1 = render_font;
			params.a2 = render_text;
			params.a3 = screen_position;
			params.a4 = scale;
			params.a5 = render_color;
			params.a6 = kerning;
			params.a7 = shadow_color;
			params.a8 = shadow_offset;
			params.a9 = bcentrex;
			params.a10 = bcentrey;
			params.a11 = boutlined;
			params.a12 = outline_color;

			this->process_event(function, &params);
			return;
		}
	};

	class camera_manager : public uobject {
	public:
		frotator get_camera_rotation() {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"PlayerCameraManager.GetCameraRotation", false);

			struct { frotator ret; } params;

			this->process_event(function, &params);
			return params.ret;
		}
	};

	class mesh_component : public uobject {
	public:
		fmatrix bone_matrix(int32_t index) {
			static uobject* function;
			if (!function)
				function = (uobject*)(base_address + 0x131787C);

			fmatrix matrix;
			auto call = reinterpret_cast<fmatrix * (__fastcall*)(uobject*, fmatrix*, int)>(function);

			call(this, &matrix, index);
			return matrix;
		}

		fvector get_bone_location(int32_t index) {
			fmatrix matrix = bone_matrix(index);
			return { matrix.m[3][0], matrix.m[3][1], matrix.m[3][2] };
		}

		int32_t get_num_bones() {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"SkinnedMeshComponent.GetNumBones", false);

			struct { int32_t ret; } params;

			this->process_event(function, &params);
			return params.ret;
		}
	};

	class actor : public uobject {
	public:
		fvector k2_get_actor_location() {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Actor.K2_GetActorLocation", false);

			struct { fvector ret; } params;

			this->process_event(function, &params);
			return params.ret;
		}

		frotator k2_get_actor_rotation() {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Actor.K2_GetActorRotation", false);

			struct { frotator ret; } params;

			this->process_event(function, &params);
			return params.ret;
		}

		bool k2_set_actor_location(fvector new_location, bool sweep, uint8_t sweep_hit_result, bool teleport) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Actor.K2_SetActorLocation", false);

			struct { fvector a1; bool a2; uint8_t a3; bool a4; bool ret; } params;
			params.a1 = new_location;
			params.a2 = sweep;
			params.a3 = sweep_hit_result;
			params.a4 = teleport;

			this->process_event(function, &params);
			return params.ret;
		}

		bool k2_set_actor_rotation(frotator new_rotation, bool teleport_physics) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Actor.K2_SetActorRotation", false);

			struct { frotator a1; bool a2; bool ret; } params;
			params.a1 = new_rotation;
			params.a2 = teleport_physics;

			this->process_event(function, &params);
			return params.ret;
		}
	};

	class player_pawn : public actor {
	public:
		char pad_0[0x310];
		mesh_component* mesh;
		char pad_1[0x2030];
		actor* current_vehicle;

		bool is_in_vehicle() {
			//IsInVehicle
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"FortPlayerPawn.IsInVehicle", false);

			struct { bool ret; } params;

			this->process_event(function, &params);
			return params.ret;
		}
	};

	class acontroller : public uobject {
	public:
		char pad_0[0x338];
		camera_manager* camera;

		player_pawn* k2_get_pawn() {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Controller.K2_GetPawn", false);

			struct { player_pawn* ret; } params;

			this->process_event(function, &params);
			return params.ret;
		}

		bool line_of_sight_to(uobject* other, fvector view_point, bool alternate_checks) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"Controller.LineOfSightTo", false);

			struct { uobject* a1; fvector a2; bool a3; bool ret; } params;
			params.a1 = other;
			params.a2 = view_point;
			params.a3 = alternate_checks;

			this->process_event(function, &params);
			return params.ret;
		}

		bool world_to_screen(fvector worldlocation, fvector2d* screenlocation, bool bplayerviewportrelative) {
			static uobject* function;
			if (!function)
				function = uobject::static_find_object(0, 0, L"PlayerController.ProjectWorldLocationToScreen", false);

			struct { fvector a1; fvector2d a2; bool a3; bool ret; } params;
			params.a1 = worldlocation;
			params.a3 = bplayerviewportrelative;

			this->process_event(function, &params);
			*screenlocation = params.a2;
			return params.ret;
		}

		bool is_input_key_down(fkey key) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"PlayerController.IsInputKeyDown", false);

			struct { fkey a1; bool ret; } params;
			params.a1 = key;

			this->process_event(function, &params);
			return params.ret;
		}

		bool get_mouse_position(double* locationx, double* locationy) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"PlayerController.GetMousePosition", false);

			struct { float a1; float a2; bool ret; } params;

			this->process_event(function, &params);
			*locationx = params.a1;
			*locationy = params.a2;

			return params.ret;
		}
	};

	class ugameviewport : public uobject {
	public:
		char pad_0[0x78];
		uobject* world;
	};

	class uengine : public uobject {
	public:
		char pad_0[0x50];
		uobject* small_font;
	};

	class ulocalplayer : public uobject {
	public:
		char pad_0[0x30];
		acontroller* controller;
		char pad_1[0x40];
		ugameviewport* viewport;
	};

	class ugameinstance : public uobject {
	public:
		char pad_0[0x38];
		tarray<ulocalplayer*> localplayers;
	};

	class uworld : public uobject {
	public:

	};

	class ugameplaystatistics : public uobject {
	public:
		void get_all_actors_of_class(uobject* world_context_object, uobject* actor_class, tarray<uobject*>* out_actors) {
			static uobject* function;
			if (!function)
				function = this->static_find_object(0, 0, L"GameplayStatics.GetAllActorsOfClass", false);

			struct { uobject* a1; uobject* a2; tarray<uobject*>a3; } params;
			params.a1 = world_context_object;
			params.a2 = actor_class;

			this->process_event(function, &params);
			*out_actors = params.a3;
			return;
		}

		ugameinstance* get_game_instance(uworld* world_context) {
			static uobject* function;
			if (!function)
				function = uobject::static_find_object(nullptr, nullptr, L"GameplayStatics.GetGameInstance", false);

			struct { uworld* a1; ugameinstance* ret; } params;
			params.a1 = world_context;

			this->process_event(function, &params);

			return params.ret;
		}
	};

	class ukismetsystem : public uobject {
	public:
		fstring get_object_name(uobject* object) {
			static uobject* function;
			if (!function)
				function = uobject::static_find_object(nullptr, nullptr, L"KismetSystemLibrary.GetObjectName", false);

			struct { uobject* a1; fstring ret; } params;
			params.a1 = object;

			this->process_event(function, &params);

			return params.ret;
		}

		uobject* get_outer_object(uobject* object) {
			static uobject* function;
			if (!function)
				function = uobject::static_find_object(nullptr, nullptr, L"KismetSystemLibrary.GetOuterObject", false);

			struct { uobject* a1; uobject* ret; } params;
			params.a1 = object;

			this->process_event(function, &params);

			return params.ret;
		}
	};

	class ukismetstring : public uobject {
	public:
		fname conv_string_to_name(fstring string) {
			static uobject* function;
			if (!function)
				function = uobject::static_find_object(nullptr, nullptr, L"KismetStringLibrary.Conv_StringToName", false);

			struct { fstring a1; fname ret; } params;
			params.a1 = string;

			this->process_event(function, &params);

			return params.ret;
		}
	};

	namespace classes {
		ugameplaystatistics* gameplaystatistics = nullptr;
		ukismetsystem* kismetsystem = nullptr;
		ukismetstring* kismetstring = nullptr;
	}
} using namespace yatinu;

uworld* gworld = nullptr;

namespace fn {
	uworld* find_world() {
		return (uworld*)(*(std::uintptr_t*)(base_address + 0xE0CF488));
	}
}