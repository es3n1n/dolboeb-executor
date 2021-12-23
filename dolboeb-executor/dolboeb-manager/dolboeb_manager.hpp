#pragma once
#include "exploit/i_exploit.hpp"
#include "exploit/kernel_utils/kernel_utils.hpp"


namespace dolboeb {
	class c_manager {
	public:
		c_manager( ) = default;
		c_manager( exploit::i_exploit* exploit );
		~c_manager( );

		//
		// Set directory base
		void init_exploit( );

	public:
		exploit::i_exploit* m_exploit;
		exploit::c_kernel_utils* m_utils;
	};
}
