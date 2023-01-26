#pragma once

uobject* font;

struct ucanvas : uobject
{
	auto k2_drawline(fvector2d a, fvector2d b, float thickness, flinearcolor rendercolor) -> void
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Canvas.K2_DrawLine");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector2d a, b;
			float thickness;
			flinearcolor rendercolor;
		} params = { a, b, thickness, rendercolor };

		this->process_event(function, &params);
	}

	auto k2_drawtext(fstring rendertext, fvector2d screen, flinearcolor color, double scale, bool center_x, bool center_y, bool outlined) -> void
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Canvas.K2_DrawText");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			uobject* font;
			fstring rendertext;
			fvector2d screen, scale;
			flinearcolor rendercolor;
			float kerning;
			flinearcolor shadowcolor;
			fvector2d shadowoffset;
			bool center_x, center_y, outlined;
			flinearcolor outline_color;
		} params = { font, rendertext, screen, fvector2d(scale, scale), color, scale, flinearcolor(), fvector2d(), center_x, center_y, outlined, flinearcolor(0.f, 0.f, 0.f, 1.f)};

		this->process_event(function, &params);
	}
};

struct ulocalplayer : uobject
{

};

struct ugameinstance : uobject
{

};

struct ugameviewportclient : uobject
{

};

struct aplayercameramanager : uobject
{
	auto get_camera_location() -> fvector
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerCameraManager.GetCameraLocation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_camera_rotation() -> frotator
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerCameraManager.GetCameraRotation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			frotator return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_fov_angle() -> float
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerCameraManager.GetFOVAngle");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			float return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct uscenecomponent : uobject
{

};

struct uprimitivecomponent : uscenecomponent
{

};

struct umeshcomponent : uprimitivecomponent
{

};

struct uskinnedmeshcomponent : umeshcomponent
{
	auto get_bone_matrix(int32_t index) -> fmatrix
	{
		auto function = reinterpret_cast<void(*)(uskinnedmeshcomponent*, fmatrix*, int32_t)>(elysian::game_base + 0xEFC81C);
		auto matrix = fmatrix();
		
		stub.callstack<void>(function, this, &matrix, index);

		return matrix;
	}

	auto get_bone_location(int32_t index) -> fvector
	{
		auto matrix = this->get_bone_matrix(index);

		return fvector(matrix.wplane.x, matrix.wplane.y, matrix.wplane.z);
	}
};

struct uskeletalmeshcomponent : uskinnedmeshcomponent
{

};

struct acontroller : uobject
{
	auto k2_getpawn() -> uobject*
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Controller.K2_GetPawn");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			uobject* return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct aplayercontroller : acontroller
{
	auto add_pitch_input(float val) -> void
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerController.AddPitchInput");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			float val;
		} params = { val };

		this->process_event(function, &params);
	}
	
	auto add_yaw_input(float val) -> void
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerController.AddYawInput");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			float val;
		} params = { val };

		this->process_event(function, &params);
	}

	auto get_mouse_position() -> fvector2d
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerController.GetMousePosition");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			float x, y;
			bool return_value;
		} params = { };

		this->process_event(function, &params);

		return fvector2d(double(params.x), double(params.y));
	}

	auto is_input_key_down(fkey key) -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerController.IsInputKeyDown");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fkey key;
			bool return_value;
		} params = { key };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto was_input_key_just_pressed(fkey key) -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerController.WasInputKeyJustPressed");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fkey key;
			bool return_value;
		} params = { key };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto world_to_screen(fvector world, fvector2d* screen) -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.PlayerController.ProjectWorldLocationToScreen");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector world;
			fvector2d screen;
			bool relative_viewport;
			bool return_value;
		} params = { world, fvector2d(), true};

		this->process_event(function, &params);

		*screen = params.screen;
		return params.return_value;
	}
};

struct aactor : uobject
{
	auto k2_set_actor_location_and_rotation(fvector new_location, frotator new_rotation, bool sweep, bool teleport) -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Actor.K2_SetActorLocationAndRotation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector new_location;
			frotator new_rotation;
			bool sweep;
			uint8_t hit_result;
			bool teleport;
			bool return_value;
		} params = { new_location, new_rotation, sweep, 0, teleport };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto k2_get_actor_location() -> fvector
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Actor.K2_GetActorLocation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto k2_get_actor_rotation() -> frotator
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Actor.K2_GetActorRotation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			frotator return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto k2_set_actor_location(fvector new_location, bool sweep, bool teleport) -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Actor.K2_SetActorLocation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector new_location;
			bool sweep;
			uint8_t hit_result;
			bool teleport;
			bool return_value;
		} params = { new_location, sweep, 0, teleport };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct apawn : aactor
{

};

struct acharacter : apawn
{
	auto can_jump() -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.Character.CanJump");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			bool return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct kismetsystemlibrary : uobject
{
	auto get_object_name(uobject* object) -> fstring
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetSystemLibrary.GetObjectName");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			uobject* object;
			fstring return_value;
		} params = { object };

		this->process_event(function, &params);

		return params.return_value;
	}
};	

struct kismetmathlibrary : uobject
{
	frotator interpolate_constant(frotator current, frotator target, float delta_time, float speed)
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.RInterpTo_Constant");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			frotator current, target;
			float delta_time, speed;
			frotator return_value;
		} params = { current, target, delta_time, speed };

		this->process_event(function, &params);

		return params.return_value;
	}

	frotator interpolate(frotator current, frotator target, float delta_time, float speed)
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.RInterpTo");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			frotator current, target;
			float delta_time, speed;
			frotator return_value;
		} params = { current, target, delta_time, speed };

		this->process_event(function, &params);

		return params.return_value;
	}

	double interpolate_double(double current, double target, double delta_time, double speed)
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.FInterpTo");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			double current, target, delta_time, speed, return_value;
		} params = { current, target, delta_time, speed };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto vector_distance(fvector v1, fvector v2) -> double
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.Vector_Distance");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector v1, v2;
			double return_value;
		} params = { v1, v2 };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto vector_distance_2d(fvector2d v1, fvector2d v2) -> double
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.Vector_Distance2D");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector v1, v2;
			double return_value;
		} params = { fvector(v1.x, v1.y, 0), fvector(v2.x, v2.y, 0) };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto find_look_at_rotation(fvector start, fvector target) -> frotator
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.FindLookAtRotation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fvector start, target;
			frotator return_value;
		} params = { start, target };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto sin(double a) -> double
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.sin");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			double a;
			double return_value;
		} params = { a };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto cos(double a) -> double
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetMathLibrary.cos");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			double a;
			double return_value;
		} params = { a };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct gameplaystatics : uobject
{
	auto get_world_delta_seconds(uobject* world) -> double
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.GameplayStatics.GetWorldDeltaSeconds");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			uobject* world;
			double return_value;
		} params = { world };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_all_actors_of_class(uobject* world, uobject* actor_class) -> tarray<uobject*>
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.GameplayStatics.GetAllActorsOfClass");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			uobject* world;
			uobject* actor_class;
			tarray<uobject*> return_value;
		} params = { world, actor_class };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct kismetstringlibrary : uobject
{
	auto contains(fstring search_in, fstring sub_string, bool use_case) -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetStringLibrary.Contains");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fstring search_in, sub_string;
			bool use_case, search_from_end, return_value;
		} params = { search_in, sub_string, use_case, false };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto build_string_double(fstring append_to, fstring prefix, double in_double, fstring suffix) -> fstring
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetStringLibrary.BuildString_Double");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fstring append_to, prefix;
			double in_double;
			fstring suffix;
			fstring return_value;
		} params = { append_to, prefix, in_double, suffix };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto build_string_int(fstring append_to, fstring prefix, int32_t in_int, fstring suffix) -> fstring
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetStringLibrary.BuildString_Int");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fstring append_to, prefix;
			int32_t in_int;
			fstring suffix;
			fstring return_value;
		} params = { append_to, prefix, in_int, suffix };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto string_to_name(fstring in_string) -> fname
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"Engine.KismetStringLibrary.Conv_StringToName");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			fstring in_string;
			fname return_value;
		} params = { in_string };

		this->process_event(function, &params);

		return params.return_value;
	}
};



struct fortkismetlibrary : uobject
{
	auto get_player_name_safe(aactor* actor) -> fstring
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortKismetLibrary.GetPlayerNameSafe");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			aactor* actor;
			fstring return_value;
		} params = { actor };

		this->process_event(function, &params);

		return params.return_value;
	}
};

namespace libs
{
	kismetsystemlibrary* system = nullptr;
	kismetmathlibrary* math = nullptr;
	gameplaystatics* statics = nullptr;
	kismetstringlibrary* string = nullptr;
	fortkismetlibrary* fort = nullptr;

	static auto init() -> bool
	{
		auto system_name = Encrypt(L"Engine.KismetSystemLibrary");
		auto math_name = Encrypt(L"Engine.KismetMathLibrary");
		auto statics_name = Encrypt(L"Engine.GameplayStatics");
		auto string_name = Encrypt(L"Engine.KismetStringLibrary");
		auto fort_name = Encrypt(L"FortniteGame.FortKismetLibrary");

		system = (decltype(system))uobject::find_object(system_name.decrypt()), system_name.clear();
		if (!system) return false;

		math = (decltype(math))uobject::find_object(math_name.decrypt()), math_name.clear();
		if (!math) return false;

		statics = (decltype(statics))uobject::find_object(statics_name.decrypt()), statics_name.clear();
		if (!statics) return false;

		string = (decltype(string))uobject::find_object(string_name.decrypt()), string_name.clear();
		if (!string) return false;

		fort = (decltype(fort))uobject::find_object(fort_name.decrypt()), fort_name.clear();
		if (!fort) return false;

		return true;
	}
}

/*

// Enum Engine.ECollisionChannel
enum class ECollisionChannel : uint8 {
	ECC_WorldStatic = 0,
	ECC_WorldDynamic = 1,
	ECC_Pawn = 2,
	ECC_Visibility = 3,
	ECC_Camera = 4,
	ECC_PhysicsBody = 5,
	ECC_Vehicle = 6,
	ECC_Destructible = 7,
	ECC_EngineTraceChannel1 = 8,
	ECC_EngineTraceChannel2 = 9,
	ECC_EngineTraceChannel3 = 10,
	ECC_EngineTraceChannel4 = 11,
	ECC_EngineTraceChannel5 = 12,
	ECC_EngineTraceChannel6 = 13,
	ECC_GameTraceChannel1 = 14,
	ECC_GameTraceChannel2 = 15,
	ECC_GameTraceChannel3 = 16,
	ECC_GameTraceChannel4 = 17,
	ECC_GameTraceChannel5 = 18,
	ECC_GameTraceChannel6 = 19,
	ECC_GameTraceChannel7 = 20,
	ECC_GameTraceChannel8 = 21,
	ECC_GameTraceChannel9 = 22,
	ECC_GameTraceChannel10 = 23,
	ECC_GameTraceChannel11 = 24,
	ECC_GameTraceChannel12 = 25,
	ECC_GameTraceChannel13 = 26,
	ECC_GameTraceChannel14 = 27,
	ECC_GameTraceChannel15 = 28,
	ECC_GameTraceChannel16 = 29,
	ECC_GameTraceChannel17 = 30,
	ECC_GameTraceChannel18 = 31,
	ECC_OverlapAll_Deprecated = 32,
	ECC_MAX = 33
};

FCollisionQueryParams
(
	FName InTraceTag,
	bool bInTraceComplex,
	const AActor* InIgnoreActor
)

static bool RaycastTest
(
	const UWorld * World,
	const FVector Start,
	const FVector End,
	ECollisionChannel TraceChannel,
	const FCollisionQueryParams & Params,
	const FCollisionResponseParams & ResponseParams,
	const FCollisionObjectQueryParams & ObjectParams
)

//fname: LineOfSightCollision oder LineOfSightCheck
FCollisionQueryParams params = FCollisionQueryParams(^ fname, true, Other);

RaycastTest(World, Fvector(), Fvector(), ECollisionChannel::ECC_Visibility, );

*/

