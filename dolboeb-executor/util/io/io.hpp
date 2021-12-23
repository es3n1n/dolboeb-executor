#pragma once
#include <Windows.h>
#include <cstdint>
#include <filesystem>


namespace util::io {
	using handle_t = HANDLE;

	handle_t create_file( const char* path );
	handle_t create_file( const wchar_t* path );
	handle_t open_file( const char* path );
	handle_t open_file( const wchar_t* path );
	size_t get_file_size( handle_t handle );
	bool read_file( handle_t handle, uint8_t* buffer, size_t size );
	bool write_file( handle_t handle, uint8_t* buffer, size_t size );
	void close_file( handle_t handle );
}