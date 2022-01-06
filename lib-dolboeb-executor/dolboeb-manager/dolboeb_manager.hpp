#pragma once
#include "../exploit/i_exploit.hpp"
#include "../util/kernel_utils/kernel_utils.hpp"


namespace dolboeb {
	class c_manager {
	public:
		c_manager( ) = default;
		c_manager( i_exploit* exploit );
		~c_manager( );

		//
		// Set directory base
		void init_exploit( );

	public:
		i_exploit* m_exploit;
		util::c_kernel_utils* m_utils;
	};
}
