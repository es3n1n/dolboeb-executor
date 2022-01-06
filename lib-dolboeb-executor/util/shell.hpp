#pragma once
#include <Windows.h>
#include <cstdint>


namespace dolboeb::util {
	class c_shellcode_crafter {
	private:
		uint8_t* buffer {};
		size_t buffer_size = 0;
	public:
		c_shellcode_crafter( ) {
			buffer = reinterpret_cast< uint8_t* >( VirtualAlloc( nullptr, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE ) );
		}

		~c_shellcode_crafter( ) {
			VirtualFree( buffer, 0, MEM_RELEASE );
		}

		template<typename T>
		void make( const T& v ) {
			if constexpr ( std::is_same<T, int>::value ) {
				*reinterpret_cast< uint8_t* >( buffer + buffer_size ) = *reinterpret_cast< const uint8_t* >( &v );
				buffer_size += sizeof( uint8_t );
			} else {
				*reinterpret_cast< T* >( buffer + buffer_size ) = v;
				buffer_size += sizeof( T );
			}
		}

		template<typename F, typename... R>
		void make( const F& first, const R&... rest ) {
			make( first );
			make( rest... );
		}

		void get( uint8_t* getter ) {
			for ( size_t i = 0; i < buffer_size; i++ ) {
				getter[ i ] = buffer[ i ];
			}
		}

		uint8_t* get( ) {
			return reinterpret_cast< uint8_t* >( buffer );
		}

		size_t size( ) {
			return buffer_size;
		}
	};
}