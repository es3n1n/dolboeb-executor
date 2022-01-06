#pragma once
#include "../dolboeb-manager/dolboeb_manager.hpp"
#include "../util/shell.hpp"
#include "shared/dolboeb.hpp"
#include "shared/ke.hpp"
#include "../resources/dolboeb_shellcode.hpp"


namespace dolboeb::executor {
	//
	// The function that will be executed in drv
	inline shared::dolboeb::callback_t callback;

	//
	// Patch target function
	bool startup( dolboeb::c_manager& dev );

	//
	// Executes cb inside dev.m_exploit
	bool exec( dolboeb::c_manager& dev, shared::dolboeb::callback_t cb );
}
