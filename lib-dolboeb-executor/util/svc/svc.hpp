#pragma once
#include <Windows.h>
#include <string>


namespace dolboeb::util::svc {
	bool start( const std::string& name, const std::string& path );
	bool stop( const std::string& name );
	bool drop_from_resources_and_run( unsigned char* res, size_t size, const std::string& name );
}
