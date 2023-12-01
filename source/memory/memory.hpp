#pragma once
#include "../includes.hpp"

class mem_base
{
public:
	virtual DWORD get_process_id( const char* process_name ) = 0;
	virtual uint64_t get_module_address( const char* module_name ) = 0;
	virtual bool read_memory( void* address, void* buffer, uint64_t size ) = 0;
	virtual bool write_memory( void* address, void* buffer, uint64_t size ) = 0;

	template <typename T>
	T read( uint64_t address )
	{
		T buffer;
		bool result = this->read_memory( reinterpret_cast<void*>(address), &buffer, sizeof( T ) );
		return (result ? buffer : T( ));
	}

	template <typename T>
	bool write( uint64_t address, T buffer )
	{
		return this->write_memory( reinterpret_cast<void*>(address), &buffer, sizeof( T ) );
	}

	uint64_t read_chain(const uint64_t base, const std::vector<uint64_t>& offsets )
	{
		uint64_t result = this->read<uint64_t>( base + offsets[0] );
		for ( int i = 1; i < offsets.size( ); i++ )
			result = this->read<uint64_t>( result + offsets[i] );

		return result;
	}

	std::string read_string( const uint64_t address )
	{
		int length = this->read<int>( address + 0x10 );
		std::wstring wstr;
		wstr.resize( length );
		if ( !this->read_memory( reinterpret_cast<void*>(address + 0x14), wstr.data( ), length * sizeof( wchar_t ) ) )
			return std::string( );

		return std::string( wstr.begin( ), wstr.end( ) );
	}
};

extern std::shared_ptr<mem_base> memory;

auto initialize_memory( ) -> bool;

