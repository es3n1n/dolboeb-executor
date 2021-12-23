#pragma once
#include "dolboeb-manager/dolboeb_manager.hpp"
#include "util/svc/svc.hpp"
#include "util/io/io.hpp"
#include "util/logger.hpp"
#include "exploit/i_exploit.hpp"


namespace bootstrap {
	//
	// Initializing dolboeb-core
	bool startup( exploit::i_exploit* my_exploit );
	
	//
	// Cleaning up
	void cleanup( exploit::i_exploit* my_exploit );
}
