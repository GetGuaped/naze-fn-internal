#pragma once

namespace elysian_fn {
	namespace vmt {
		class shadow {
		public:
			shadow(void* address) {
				this->address = uintptr_t(address);
				this->original_vtable = *(uintptr_t**)this->address;
				this->size = int32_t(0);

				do this->size += 1;
				while (*(uintptr_t*)(uintptr_t(this->original_vtable) + (this->size * 8)));
			}

			//works but gets me banned faster
			//todo; fix integrity check thing
			//
			//_declspec(noinline) static uintptr_t bind_function_to_cave(uintptr_t cave_begin, void* function) {

			//	BYTE hook[] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xD0, 0x48 };
			//	*reinterpret_cast<PVOID*>(&hook[6]) = (PVOID*)function;

			//	DWORD protection_original;
			//	IFH(VirtualProtect)((LPVOID)cave_begin, sizeof(hook), PAGE_EXECUTE_READWRITE, &protection_original); //instaban

			//	memcpy(static_cast<unsigned char*>((void*)cave_begin), static_cast<unsigned const char*>((void*)hook), sizeof(hook)); //FortniteClient-Win64-Shipping .text patch

			//	IFH(VirtualProtect)((LPVOID)cave_begin, sizeof(hook), protection_original, &protection_original); //instaban

			//	return cave_begin;
			//}

			//works fine, not doing useless shit and is p
			template <typename hook_type>
			_declspec(noinline) hook_type apply(uintptr_t hook_function, int32_t index) {
				auto vfunction_original = (void*)this->original_vtable[index];

				uintptr_t* fake_vtable = new uintptr_t[this->size * int32_t(8)];

				for (int32_t i = 0; i < this->size; i++) {
					if (i == index) continue;
					fake_vtable[i] = *(uintptr_t*)(uintptr_t(this->original_vtable) + (i * int32_t(8)));
				} fake_vtable[index] = hook_function;

				*(uintptr_t**)this->address = fake_vtable;

				return hook_type(vfunction_original);
			}
		private:
			uintptr_t address;
			uintptr_t* original_vtable;
			int32_t size;
		};
	}
}