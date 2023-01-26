#pragma once

extern "C" void* _spoofer_stub();

#define RVA(Instr, InstrSize) ((DWORD64)Instr + InstrSize + *(LONG*)((DWORD64)Instr + (InstrSize - sizeof(LONG))))
#define ConstStrLen(Str) ((sizeof(Str) - sizeof(Str[0])) / sizeof(Str[0]))
#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)

template <typename StrType> __declspec(noinline) constexpr unsigned short HashStr(StrType Data, int Len)
{
	unsigned short CRC = 0xFFFF; while (Len--)
	{
		auto CurChar = *Data++; if (!CurChar) break;
		CRC ^= ToLower(CurChar) << 8; for (int i = 0; i < 8; i++)
			CRC = CRC & 0x8000 ? (CRC << 1) ^ 0x8408 : CRC << 1;
	} return CRC;
}

#define ConstHashStr(Str) [](){ constexpr unsigned short CRC = HashStr(Str, ConstStrLen(Str)); return CRC; }()

template <typename PtrType>
__forceinline PtrType EPtr(PtrType Ptr)
{
	typedef union
	{
		struct
		{
			USHORT Key1; USHORT Key2;
			USHORT Key3; USHORT Key4;
		}; ULONG64 Key;
	} CryptData;
	CryptData Key{ ConstHashStr(__TIME__), ConstHashStr(__DATE__),
		ConstHashStr(__TIMESTAMP__), ConstHashStr(__TIMESTAMP__) };
	volatile LONG64 PtrData; volatile LONG64 VKey;
	InterlockedExchange64(&VKey, (ULONG64)Key.Key);
	InterlockedExchange64(&PtrData, (ULONG64)Ptr);
	PtrData ^= VKey; return (PtrType)PtrData;
}

class stub_call
{
public:
	PVOID ReturnAddressStubEnc = 0;
	template<typename Ret = void, typename First = void*, typename Second = void*, typename Third = void*, typename Fourth = void*, typename... Stack>
	__forceinline Ret callstack(void* Func, First a1 = First{}, Second a2 = Second{}, Third a3 = Third{}, Fourth a4 = Fourth{}, Stack... args)
	{
		struct shell_params { const void* a1; void* a2; void* a3; };
		shell_params CallCtx = { EPtr(ReturnAddressStubEnc), Func, nullptr };
		typedef Ret(*ShellFn)(First, Second, Third, Fourth, PVOID, PVOID, Stack...);
		return ((ShellFn)&_spoofer_stub)(a1, a2, a3, a4, &CallCtx, nullptr, args...);
	}

	__forceinline void set_stub(PVOID R15_Stub) { ReturnAddressStubEnc = EPtr(R15_Stub); }
};

stub_call stub;