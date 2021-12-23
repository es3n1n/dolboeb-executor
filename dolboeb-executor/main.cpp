#include <Windows.h>
#include "bootstrap/bootstrap.hpp"
#include "dolboeb-executor/dolboeb_executor.hpp"
#include "util/util.hpp"
#include "phymem/c_phymem.hpp"


int main( ) {
	util::logger::info( "Starting..." );

	//
	// Granting priviledges
	util::grant_priviledges( { L"SeDebugPrivilege",  L"SeLoadDriverPrivilege" } );

	//
	// Initializing custom exploit
	exploit::i_exploit* my_exploit = new exploit::c_phymem( );

	//
	// Initializing dolboeb-core
	if ( !bootstrap::startup( my_exploit ) ) {
		util::logger::error( "Unable to initialize" );
		return EXIT_FAILURE;
	}

	//
	// Initializing dolboeb-manager
	dolboeb::c_manager manager = dolboeb::c_manager( my_exploit );
	manager.init_exploit( );

	//
	// Patching function
	if ( !dolboeb::executor::startup( manager ) ) {
		util::logger::error( "Unable to start executor" );
		bootstrap::cleanup( my_exploit );
		return EXIT_FAILURE;
	}

	//
	// Getting some imports from ntoskrnl that we'll use later
	const auto dbgprint = manager.m_utils->get_ntoskrnl_export( "DbgPrint" );
	util::logger::info( "DebugPrint: 0x%p", dbgprint );

	//
	// Execute our callback in kernel
	dolboeb::executor::exec( manager, [ & ] ( shared::dolboeb::handler_ctx_t ctx ) -> void {
		reinterpret_cast< shared::ke::DbgPrint_t >( dbgprint )( "[+] hello from phymem :)\n" );
	} );

	//
	// Cleaning up
	bootstrap::cleanup( my_exploit );

	return EXIT_SUCCESS;
}
