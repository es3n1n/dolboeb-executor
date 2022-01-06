#include "bootstrap.hpp"


namespace dolboeb::bootstrap {
	bool startup( i_exploit* my_exploit ) {
		//
		// Dropping our .sys file and loading it
		if ( !my_exploit->setup_driver( ) ) {
			util::logger::error( "Failed to create svc" );
			return false;
		}

		//
		// Init handle to device so we'll use it in our ioctls later
		if ( !my_exploit->init_device_handle( ) ) {
			util::logger::error( "Failed to initialize device handle" );
			return false;
		}

		return true;
	}

	void cleanup( i_exploit* my_exploit ) {
		//
		// Unload driver and remove it
		if ( !my_exploit->unload_driver( ) )
			util::logger::error( "Failed to stop svc" );

		util::logger::info( "Finished" );
	}
}