#pragma once
#include <Windows.h>
#include <winternl.h>


namespace shared::ke {
	using KeSetSystemAffinityThread_t = void( * )( KAFFINITY );
	using KeQueryActiveProcessors_t = KAFFINITY( * )( );
	using MmGetSystemRoutineAddress_t = void*( * )( PUNICODE_STRING );
	using DbgPrint_t = void( * )( const char*, ... );
}
