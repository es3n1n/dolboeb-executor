#pragma once
#include <Windows.h>
#include <string>
#include "../../exploit/i_exploit.hpp"


namespace dolboeb::util {
	class c_kernel_utils {
	public:
		c_kernel_utils( ) = default;
		c_kernel_utils( i_exploit* dev );
		~c_kernel_utils( );

	public:
		//
		// Finding dirbase
		uint64_t find_ntos_dirbase( );
		//
		// Converting virtual address to physical
		uint64_t virt2phys( const uint64_t virt_addr );

	public:
		//
		// Gets kernel module
		uint64_t get_module( const std::string& module_name );
		//
		// Gets ntoskrnl export
		uint64_t get_ntoskrnl_export( const std::string& fn_name );

	private:
		i_exploit* m_dev;
		HMODULE m_ntoskrnl = 0x0;
	};
}
