#pragma once

namespace elysian_fn {
	namespace sdk {
		namespace actor_classes {
			class actor : public uobject {
			public:
				frotator get_actor_rotation() {
					static uobject* member;
					if (!member) member = uobject::find_object(L"Actor.K2_GetActorRotation");

					return this->get<frotator>(member);
				}

				fvector get_actor_location() {
					static uobject* member;
					if (!member) member = uobject::find_object(L"Actor.K2_GetActorLocation");

					return this->get<fvector>(member);
				}
			};

			class playerpawn : public actor {
			public:
				mesh* get_mesh() {
					return memory::read<mesh*>(this + 0x310);
				}

				playerstate* get_state() {
					return memory::read<playerstate*>(this + 0x2a8);
				}

				weapon* get_weapon() {
					static uobject* member;
					if (!member) member = uobject::find_object(L"FortniteGame.FortPawn.GetCurrentWeapon");

					return this->get<weapon*>(member);
				}

				char get_team() {
					static uobject* member;
					if (!member) member = uobject::find_object(L"FortniteGame.FortPawn.GetTeam");

					return this->get<char>(member);
				}

				bool is_dead() {
					static uobject* member;
					if (!member) member = uobject::find_object(L"FortniteGame.FortPawn.IsDead");

					return this->get<bool>(member);
				}

				bool is_dbno() {
					static uobject* member;
					if (!member) member = uobject::find_object(L"FortniteGame.FortPawn.IsDBNO");

					return this->get<bool>(member);
				}
			};

			class pickuppawn : public actor {
			public:
				item_definition* get_item_definition() {
					return memory::read<item_definition*>(this + 0x318 + 0x18);
				}
			};
		}
	}
}

using namespace elysian_fn::sdk::actor_classes;