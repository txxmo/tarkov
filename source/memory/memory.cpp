#include "memory.hpp"
#include "dma.hpp"
#include "driver.hpp"

/* you can use dma, however I don't really suggest it.
*  the cheat was never really optimized to use all of the options available
*  in the dma memory lib (vmm), so you can probably make something simple
*  with dma but i do not suggest using it the way it is to make a full cheat.
*/

std::shared_ptr<mem_base> memory = nullptr;

auto initialize_memory( ) -> bool
{
	std::vector<LPSTR> arguments = { (LPSTR)"", (LPSTR)"-device", (LPSTR)"fpga" };
	dma::dma_handle = VMMDLL_Initialize( 3, arguments.data( ) );
	if ( !dma::dma_handle ) {
		if ( !_driver::initialize( ) )
		{
			_log::error( _( "failed to initialize driver" ) );
			return false;
		}
		
		memory = std::make_shared<driver>( );
		_log::message( _( "using memory mode: driver" ) );
	}
	else {
		memory = std::make_shared<DMA>( );
		_log::message( _( "using memory mode: DMA" ) );
	}

	return true;
}