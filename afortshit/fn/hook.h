#pragma once

namespace hook {
	void vmt(void* addr, void* pDes, int index, void** ret)
	{
		auto vtable = *(std::uintptr_t**)addr;
		int methods = 0;
		do {
			++methods;
		} while (*(std::uintptr_t*)((std::uintptr_t)vtable + (methods * 0x8)));
		auto vtable_buf = new uint64_t[methods * 0x8];
		for (auto count = 0; count < methods; ++count) {
			vtable_buf[count] = *(std::uintptr_t*)((std::uintptr_t)vtable + (count * 0x8));

			*ret = (void*)vtable[index];

			vtable_buf[index] = (std::uintptr_t)(pDes);
			*(std::uintptr_t**)addr = vtable_buf;
		}
	}

	/*void* AllocatePageNearAddressRemote(void* targetAddr)
	{
		SYSTEM_INFO sysInfo;
		LI_FN(GetSystemInfo)(&sysInfo);
		const uint64_t PAGE_SIZE = sysInfo.dwPageSize;

		uint64_t startAddr = (uint64_t(targetAddr) & ~(PAGE_SIZE - 1)); //round down to nearest page boundary
		uint64_t minAddr = min(startAddr - 0x7FFFFF00, (uint64_t)sysInfo.lpMinimumApplicationAddress);
		uint64_t maxAddr = max(startAddr + 0x7FFFFF00, (uint64_t)sysInfo.lpMaximumApplicationAddress);

		uint64_t startPage = (startAddr - (startAddr % PAGE_SIZE));

		uint64_t pageOffset = 1;
		while (1)
		{
			uint64_t byteOffset = pageOffset * PAGE_SIZE;
			uint64_t highAddr = startPage + byteOffset;
			uint64_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

			bool needsExit = highAddr > maxAddr && lowAddr < minAddr;

			if (highAddr < maxAddr)
			{
				void* outAddr = LI_FN(VirtualAllocEx).get()((HANDLE)-1, (void*)highAddr, (size_t)PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (outAddr)
					return outAddr;
			}

			if (lowAddr > minAddr)
			{
				void* outAddr = LI_FN(VirtualAllocEx).get()((HANDLE)-1, (void*)lowAddr, (size_t)PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
				if (outAddr != nullptr)
					return outAddr;
			}

			pageOffset++;

			if (needsExit)
			{
				break;
			}
		}

		return nullptr;
	}

	uint32_t WriteAbsoluteJump64(void* absJumpMemory, void* addrToJumpTo)
	{
		uint8_t absJumpInstructions[] = { 0x49, 0xBA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
											0x41, 0xFF, 0xE2 };

		uint64_t addrToJumpTo64 = (uint64_t)addrToJumpTo;
		LI_FN(memcpy)(&absJumpInstructions[2], &addrToJumpTo64, sizeof(addrToJumpTo64));
		LI_FN(memcpy)(absJumpMemory, absJumpInstructions, sizeof(absJumpInstructions));
		return sizeof(absJumpInstructions);
	}

	void HookFunction(uintptr_t address, int index, void* func) {
		uint64_t* vtablePtr = (uint64_t*)&address;
		uint64_t* vtable = (uint64_t*)*vtablePtr;
		uint64_t* funcPtr = (uint64_t*)(vtable[index]);

		DWORD oldProtect;
		BOOL success = LI_FN(VirtualProtectEx).get()((HANDLE)-1, funcPtr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

		void* relayFunc = AllocatePageNearAddressRemote(funcPtr);
		WriteAbsoluteJump64(relayFunc, func);

		uint8_t jmpInstruction[5] = { 0xE9, 0x0, 0x0, 0x0, 0x0 };

		const uint64_t relAddr = (uint64_t)relayFunc - ((uint64_t)funcPtr + sizeof(jmpInstruction));
		LI_FN(memcpy)(jmpInstruction + 1, &relAddr, 4);
		LI_FN(memcpy)(funcPtr, jmpInstruction, sizeof(jmpInstruction));
	}*/
}