#pragma once
#include "../dolboeb-manager/dolboeb_manager.hpp"
#include "../util/svc/svc.hpp"
#include "../util/io/io.hpp"
#include "../util/logger.hpp"
#include "../exploit/i_exploit.hpp"


namespace dolboeb::bootstrap {
	//
	// Initializing dolboeb-core
	bool startup( i_exploit* my_exploit );
	
	//
	// Cleaning up
	void cleanup( i_exploit* my_exploit );
}
