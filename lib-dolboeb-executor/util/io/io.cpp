#include "io.hpp"


namespace dolboeb::util::io {
	c_file c_file::create( const char* path ) {
		HANDLE file_handle = CreateFileA( path, open_attrs, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( !c_file::safe( file_handle ) )
			file_handle = CreateFileA( path, open_attrs, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
		return c_file( file_handle );
	}

	c_file c_file::create( const wchar_t* path ) {
		HANDLE file_handle = CreateFileW( path, open_attrs, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if ( !c_file::safe( file_handle ) )
			file_handle = CreateFileW( path, open_attrs, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );
		return c_file( file_handle );
	}

	size_t c_file::size( ) {
		LARGE_INTEGER i = {};
		if ( GetFileSizeEx( m_file_handle, &i ) )
			return i.QuadPart;
		return 0;
	}

	bool c_file::read( uint8_t* buffer, size_t size ) {
		return ReadFile( m_file_handle, buffer, static_cast< DWORD >( size ), NULL, NULL );
	}

	bool c_file::write( uint8_t* buffer, size_t size ) {
		return WriteFile( m_file_handle, buffer, static_cast< DWORD >( size ), NULL, NULL );
	}

	bool c_file::close( ) {
		if ( m_is_closed ) return true;
		m_is_closed = true;

		return CloseHandle( m_file_handle );
	}
}