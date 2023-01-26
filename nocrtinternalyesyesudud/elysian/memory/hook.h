#pragma once

namespace elysian
{
	class vmt
	{
	public:
		vmt(void* address)
		{
			this->address = std::uintptr_t(address);
			this->original_vtable = *(std::uintptr_t**)this->address;
			this->size = std::uint32_t(0);

			do this->size += 1;
			while (*(std::uintptr_t*)(std::uintptr_t(this->original_vtable) + (this->size * std::uint32_t(8))));
		}

		//removed my code cave hook because it was detected, this one should be fine as well

		template <typename hook_type>
		_declspec(noinline) auto apply(void* hook_function, uint32_t index) -> hook_type
		{
			auto vfunction_original = (void*)this->original_vtable[index];

			std::uintptr_t* fake_vtable = new std::uintptr_t[this->size * std::uint32_t(8)];

			for (std::uint32_t i = 0; i < this->size; i++) {
				if (i == index) continue;
				fake_vtable[i] = *(std::uintptr_t*)(std::uintptr_t(this->original_vtable) + (i * std::uint32_t(8)));
			} fake_vtable[index] = std::uintptr_t(hook_function);

			*(std::uintptr_t**)this->address = fake_vtable;

			return hook_type(vfunction_original);
		}
	private:
		std::uintptr_t address;
		std::uintptr_t* original_vtable;
		std::uint32_t size;
	};
}