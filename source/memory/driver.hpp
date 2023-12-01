#pragma once
#include "../includes.hpp"
#include "memory.hpp"

namespace _driver
{
	auto is_running( ) -> bool
	{
		return true;
	}

	auto initialize( ) -> bool
	{

		return is_running();
	}
}

class driver : public mem_base
{
public:
	DWORD get_process_id( const char* process_name )
	{
		return 0;
	}

	uint64_t get_module_address( const char* module_name )
	{
		return 0;
	}

	bool read_memory( void* address, void* buffer, uint64_t size )
	{
		return true;
	}

	bool write_memory( void* address, void* buffer, uint64_t size )
	{
		return true;
	}
};