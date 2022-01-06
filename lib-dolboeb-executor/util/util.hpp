#pragma once
#include <Windows.h>
#include <string>
#include <vector>


namespace dolboeb::util {
	//
	// Granting priviledges
	inline void grant_privileges( const std::vector<std::wstring_view> names ) {
		TOKEN_PRIVILEGES Priv, PrivOld;
		DWORD cbPriv = sizeof( PrivOld );
		HANDLE token;

		if ( !OpenThreadToken( GetCurrentThread( ), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, FALSE, &token ) ) {
			if ( GetLastError( ) != ERROR_NO_TOKEN )
				return;
			if ( !OpenProcessToken( GetCurrentProcess( ), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &token ) )
				return;
		}

		Priv.PrivilegeCount = 1;
		Priv.Privileges[ 0 ].Attributes = SE_PRIVILEGE_ENABLED;
		for ( auto& name : names ) {
			LookupPrivilegeValueW( NULL, name.data( ), &Priv.Privileges[ 0 ].Luid );

			AdjustTokenPrivileges( token, FALSE, &Priv, sizeof( Priv ), &PrivOld, &cbPriv );
		}
	}
}