#pragma once
#include <Windows.h>
#include <cstdint>
#include <filesystem>


namespace dolboeb::util::io {
	constexpr auto open_attrs = GENERIC_READ | GENERIC_WRITE;

	class c_file {
	public:
		c_file( ) = default;
		c_file( HANDLE file_handle ) : m_file_handle( file_handle ) { }
		__forceinline ~c_file( ) { close( ); }

	public:
		//
		// This also will open a file if it exists
		static c_file create( const char* path );
		static c_file create( const wchar_t* path );

	public:
		size_t size( );
		bool read( uint8_t* buffer, size_t size );
		bool write( uint8_t* buffer, size_t size );

	public:
		//
		// Kinda useless, cz it will be called inside destructor, but why not :shrug:
		bool close( );

	public:
		__forceinline static bool safe( HANDLE h ) { return h != INVALID_HANDLE_VALUE; }
		__forceinline bool safe( ) { return c_file::safe( m_file_handle ); }

	private:
		HANDLE m_file_handle = 0;
		bool m_is_closed = false;
	};

}