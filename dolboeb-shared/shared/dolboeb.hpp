#pragma once
#include <cstdint>
#include <functional>


namespace shared::dolboeb {
	struct handler_ctx_t {
	public:
		uintptr_t MmGetSystemRoutineAddress;
	};

	using callback_t = std::function<void( handler_ctx_t )>;
	using handler_t = void( __fastcall* )( handler_ctx_t ctx );
}
