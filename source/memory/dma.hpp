#pragma once
#include "../includes.hpp"
#include "memory.hpp"

#pragma comment(lib, "vmm.lib")

namespace dma
{
	inline VMM_HANDLE dma_handle = 0;
}

class DMA : public mem_base
{
public:
	DWORD get_process_id( const char* process_name ) 
	{
		DWORD buffer;
		bool result = VMMDLL_PidGetFromName( dma::dma_handle, (LPSTR)process_name, &buffer );
		return (result ? buffer : 0);
	}

	uint64_t get_module_address( const char* module_name )
	{
		return VMMDLL_ProcessGetModuleBaseU( dma::dma_handle, g::pid , (LPSTR)module_name );
	}

	bool read_memory( void* address, void* buffer, uint64_t size )
	{
		return VMMDLL_MemRead( dma::dma_handle, g::pid, reinterpret_cast<ULONG64>(address), PBYTE( &buffer ), size );
	}

	bool write_memory( void* address, void* buffer, uint64_t size )
	{
		return VMMDLL_MemWrite( dma::dma_handle, g::pid, reinterpret_cast<ULONG64>(address), PBYTE( buffer ), size );
	}

	// to-do: i will get back to this later (tomo)
	bool scatter_read( void* address, void* buffer, uint64_t size );
	bool scatter_write( void* address, void* buffer, uint64_t size );
};