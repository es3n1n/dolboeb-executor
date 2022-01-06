#include "kernel_utils.hpp"

#include <Windows.h>
#include <winternl.h>

#include "shared/nt.hpp"


namespace dolboeb::util {
	c_kernel_utils::c_kernel_utils( i_exploit* dev ) {
		m_ntoskrnl = LoadLibraryExA( "ntoskrnl.exe", NULL, DONT_RESOLVE_DLL_REFERENCES );
		m_dev = dev;
	}

	c_kernel_utils::~c_kernel_utils( ) {
		FreeLibrary( m_ntoskrnl );
	}

	uint64_t c_kernel_utils::find_ntos_dirbase( ) {
		uint8_t* buffer = new uint8_t[ 0x10000 ];

		for ( int i = 0; i < 10; i++ ) {
			if ( !m_dev->read_phys( static_cast< uint64_t >( i ) * 0x10000, buffer, 0x10000 ) )
				continue;

			for ( int offset = 0; offset < 0x10000; offset += 0x1000 ) {
				if ( ( 0x00000001000600E9 ^ ( 0xffffffffffff00ff & *( uint64_t* )( buffer + offset ) ) ) ||
					 ( 0xfffff80000000000 ^ ( 0xfffff80000000000 & *( uint64_t* )( buffer + offset + 0x70 ) ) ) ||
					 ( 0xffffff0000000fff & *( uint64_t* )( buffer + offset + 0xA0 ) ) )
					continue;

				uint64_t directory_base = *( uint64_t* )( buffer + offset + 0xA0 );
				delete[ ] buffer;

				return directory_base;
			}
		}

		delete[ ] buffer;
		return 0;
	}

	uint64_t c_kernel_utils::virt2phys( const uint64_t virt_addr ) {
		uint64_t dir_base = m_dev->get_directory_base( );

		if ( !dir_base )
			return 0;

		//read PML4E
		uint64_t PML4E = 0;
		uint16_t PML4 = ( uint16_t )( ( virt_addr >> 39 ) & 0x1FF );
		m_dev->read_phys( dir_base + ( PML4 * 8 ), reinterpret_cast< uint8_t* >( &PML4E ), 8 );
		if ( !PML4E )
			return 0;

		//read PDPTE 
		uint64_t PDPTE = 0;
		uint16_t DirPtr = ( uint16_t )( ( virt_addr >> 30 ) & 0x1FF );
		m_dev->read_phys( ( PML4E & 0xFFFFFFFFFF000 ) + ( DirPtr * 8 ), reinterpret_cast< uint8_t* >( &PDPTE ), 8 );
		if ( !PDPTE )
			return 0;

		//PS=1 (1GB page)
		if ( ( PDPTE & ( 1 << 7 ) ) != 0 ) {
			//if (PageSize) *PageSize = 0x40000000/*1Gb*/;
			return ( PDPTE & 0xFFFFFC0000000 ) + ( virt_addr & 0x3FFFFFFF );
		}

		//read PDE 
		uint64_t PDE = 0;
		uint16_t Dir = ( uint16_t )( ( virt_addr >> 21 ) & 0x1FF );
		m_dev->read_phys( ( PDPTE & 0xFFFFFFFFFF000 ) + ( Dir * 8 ), reinterpret_cast< uint8_t* >( &PDE ), 8 );
		if ( !PDE )
			return 0;

		//PS=1 (2MB page)
		if ( ( PDE & ( 1 << 7 ) ) != 0 ) {
			//if (PageSize) *PageSize = 0x200000/*2MB*/;
			return ( PDE & 0xFFFFFFFE00000 ) + ( virt_addr & 0x1FFFFF );
		}

		//read PTE
		uint64_t PTE = 0;
		uint16_t Table = ( uint16_t )( ( virt_addr >> 12 ) & 0x1FF );
		m_dev->read_phys( ( PDE & 0xFFFFFFFFFF000 ) + ( Table * 8 ), reinterpret_cast< uint8_t* >( &PTE ), 8 );
		if ( !PTE )
			return 0;

		//BasePage (4KB Page)
		//if (PageSize) *PageSize = 0x1000/*4KB*/;
		return ( PTE & 0xFFFFFFFFFF000 ) + ( virt_addr & 0xFFF );
	}

	uint64_t c_kernel_utils::get_module( const std::string& module_name ) {
		void* buffer = nullptr;
		DWORD buffer_size = 0;

		NTSTATUS status = NtQuerySystemInformation( static_cast< SYSTEM_INFORMATION_CLASS >( shared::nt::SystemModuleInformation ), buffer, buffer_size, &buffer_size );

		while ( status == shared::nt::STATUS_INFO_LENGTH_MISMATCH ) {
			if ( buffer != nullptr )
				VirtualFree( buffer, 0, MEM_RELEASE );

			buffer = VirtualAlloc( nullptr, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
			status = NtQuerySystemInformation( static_cast< SYSTEM_INFORMATION_CLASS >( shared::nt::SystemModuleInformation ), buffer, buffer_size, &buffer_size );
		}

		if ( !NT_SUCCESS( status ) ) {
			if ( buffer != nullptr )
				VirtualFree( buffer, 0, MEM_RELEASE );
			return 0;
		}

		const auto modules = static_cast< shared::nt::PRTL_PROCESS_MODULES >( buffer );
		if ( !modules )
			return 0;

		for ( auto i = 0u; i < modules->NumberOfModules; ++i ) {
			const std::string current_module_name = std::string( reinterpret_cast< char* >( modules->Modules[ i ].FullPathName ) + modules->Modules[ i ].OffsetToFileName );

			if ( !_stricmp( current_module_name.c_str( ), module_name.c_str( ) ) ) {
				const uint64_t result = reinterpret_cast< uint64_t >( modules->Modules[ i ].ImageBase );

				VirtualFree( buffer, 0, MEM_RELEASE );
				return result;
			}
		}

		VirtualFree( buffer, 0, MEM_RELEASE );
		return 0;
	}

	uint64_t c_kernel_utils::get_ntoskrnl_export( const std::string& fn_name ) {
		const uint64_t ntos_local_address = reinterpret_cast< uint64_t >( GetProcAddress( m_ntoskrnl, fn_name.c_str( ) ) );
		const uint64_t ntos_fn_offset = ntos_local_address - reinterpret_cast< uint64_t >( m_ntoskrnl );

		const uint64_t ntos_address = get_module( "ntoskrnl.exe" );

		return ntos_local_address ? ntos_address + ntos_fn_offset : 0;
	}
}
