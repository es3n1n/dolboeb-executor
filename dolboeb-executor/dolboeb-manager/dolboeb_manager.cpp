#include "dolboeb-manager/dolboeb_manager.hpp"


namespace dolboeb {
	c_manager::c_manager( exploit::i_exploit* exploit ) {
		m_exploit = exploit;
		m_utils = new exploit::c_kernel_utils( m_exploit );
	}

	c_manager::~c_manager( ) {
		m_utils->~c_kernel_utils( );
	}

	void c_manager::init_exploit( ) {
		m_exploit->set_directory_base( m_utils->find_ntos_dirbase( ) );
	}
}