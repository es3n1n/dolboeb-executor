#include "../svc/svc.hpp"
#include "../logger.hpp"
#include "../io/io.hpp"
#include "shared/nt.hpp"
#include <winternl.h>
#pragma comment(lib, "ntdll.lib")


namespace dolboeb::util::svc {
	bool start( const std::string& name, const std::string& path ) {
		HKEY services_key, svc_key;

		auto stat = RegOpenKeyA( HKEY_LOCAL_MACHINE, "system\\CurrentControlSet\\Services", &services_key );
		if ( stat ) {
			RegCloseKey( services_key );
			return false;
		}

		stat = RegCreateKeyA( services_key, name.c_str( ), &svc_key );

		if ( stat ) {
			RegCloseKey( services_key );
			return false;
		}

		char localpath[ MAX_PATH ] = { 0 };
		sprintf_s( localpath, MAX_PATH, "\\??\\%s", path.c_str( ) );

		stat = RegSetValueExA( svc_key, "ImagePath", 0, REG_SZ, ( const BYTE* )localpath, ( DWORD )( sizeof( char ) * ( strlen( localpath ) + 1 ) ) );

		if ( stat ) {
			RegCloseKey( services_key );
			RegCloseKey( svc_key );
			return false;
		}

		unsigned long type = 1;
		stat = RegSetValueExA( svc_key, "Type", 0, REG_DWORD, ( const BYTE* )( &type ), sizeof( type ) );

		if ( stat ) {
			RegCloseKey( services_key );
			RegCloseKey( svc_key );
			return false;
		}

		RegCloseKey( services_key );
		RegCloseKey( svc_key );

		const std::string registery_path = "\\registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\" + name;

		UNICODE_STRING driver_registery_path { 0 };
		ANSI_STRING driver_registery_path_as { 0 };

		RtlInitAnsiString( &driver_registery_path_as, registery_path.c_str( ) );
		RtlAnsiStringToUnicodeString( &driver_registery_path, &driver_registery_path_as, TRUE );

		return NT_SUCCESS( reinterpret_cast< shared::nt::NtLoadDriver >( GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "NtLoadDriver" ) )( &driver_registery_path ) );
	}

	bool stop( const std::string& name ) {
		const std::string registery_path = "\\registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\" + name;

		UNICODE_STRING driver_registery_path { 0 };
		ANSI_STRING driver_registery_path_as { 0 };

		RtlInitAnsiString( &driver_registery_path_as, registery_path.c_str( ) );
		RtlAnsiStringToUnicodeString( &driver_registery_path, &driver_registery_path_as, TRUE );

		if ( !NT_SUCCESS( reinterpret_cast< shared::nt::NtUnloadDriver >( GetProcAddress( GetModuleHandleA( "ntdll.dll" ), "NtUnloadDriver" ) )( &driver_registery_path ) ) ) {
			return false;
		}

		if ( RegDeleteTreeA( HKEY_LOCAL_MACHINE, ( "SYSTEM\\CurrentControlSet\\Services\\" + name ).c_str( ) ) ) {
			return false;
		}

		return true;
	}

	bool drop_from_resources_and_run( unsigned char* res, size_t size, const std::string& name ) { // @note: es3n1n: NAME WITHOUT .SYS
		util::logger::info( "Creating svc..." );

		char buf[ MAX_PATH ] { };

		GetTempPathA( sizeof( buf ), buf );
		const auto path = std::string( buf ) + name + ".sys";
		util::logger::debug( "Writing file to %s", path.c_str( ) );

	OPEN_SVC_FILE:
		auto f = util::io::c_file::create( path.c_str( ) );

		if ( !f.safe( ) ) {
			if ( !stop( name ) )
				return false;
			goto OPEN_SVC_FILE;
		}

		if ( !f.write( res, size ) ) {
			util::logger::error( "Failed to write file %d", GetLastError( ) );
			return false;
		}

		f.close( );

		if ( !util::svc::start( name, path.c_str( ) ) ) {
			util::logger::error( "Failed to create svc" );
			return false;
		}

		return true;
	}
}