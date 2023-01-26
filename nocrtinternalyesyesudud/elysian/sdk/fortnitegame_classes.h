#pragma once

struct ufortvehiclefuelcomponent : uobject
{
	auto get_fuel_capacity() -> float
	{

		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortVehicleFuelComponent.GetFuelCapacity");
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

struct afortvehicle : aactor
{

};

struct afortmeatballvehicle : afortvehicle
{
	
};

struct afortpawn : acharacter
{
	auto set_pawn_visiblity(bool new_hidden, bool weapon) -> void
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortPawn.SetPawnVisibility");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			bool new_hidden, weapon;
		} params = { };

		this->process_event(function, &params);
	}

	auto is_dead() -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortPawn.IsDead");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			bool return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto is_dbno() -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortPawn.IsDBNO");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			bool return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_team() -> char
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortPawn.GetTeam");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			char return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}
};

struct afortplayerpawn : afortpawn
{
	auto get_stamina() -> float
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortPlayerPawn.GetStamina");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			float return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_max_stamina() -> float
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortPlayerPawn.GetMaxStamina");
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

struct afortplayerpawnathena : afortplayerpawn
{
	
};

struct afortprojectilebase : aactor
{

};

struct afortweapon : aactor
{
	auto is_reloading() -> bool
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortWeapon.IsReloading");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			bool return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_bullets_per_clip() -> int32_t
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortWeapon.GetBulletsPerClip");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			int32_t return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_magazine_ammo_count() -> int32_t
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortWeapon.GetMagazineAmmoCount");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			int32_t return_value;
		} params = { };

		this->process_event(function, &params);

		return params.return_value;
	}

	auto get_muzzle_location() -> fvector
	{
		static uobject* function;
		if (!function)
		{
			auto function_name = Encrypt(L"FortniteGame.FortWeapon.GetMuzzleLocation");
			function = uobject::find_object(function_name.decrypt()), function_name.clear();
		}

		struct
		{
			int32_t index;
			fvector return_value;
		} params = { 0 };

		this->process_event(function, &params);

		return params.return_value;
	}
};