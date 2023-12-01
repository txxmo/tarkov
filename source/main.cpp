#include "includes.hpp"

#include "utils/overlay/overlay.hpp"
#include "core/core.hpp"

//#define OVERLAY_ONLY

auto main( ) -> int
{
	SetConsoleTitleA( _( "sdk" ) );

	_log::warning( _( "initializing..." ) );

#ifndef OVERLAY_ONLY
	if ( !initialize_memory( ) )
	{
		Sleep( 3000 );
		return EXIT_FAILURE;
	}
#endif

#ifdef OVERLAY_ONLY
	overlay::overlay_mode = true;
#endif

	auto create_threads = [&] ( )
	{
		_log::warning( _( "creating threads..." ) );

		/* threads to capture input */
		CreateThread( 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(input::handle_keyboard), 0, 0, 0 );
		CreateThread( 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(input::handle_mouse), 0, 0, 0 );

		/* thread to create and run overlay */
		CreateThread( 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(overlay::_do), 0, 0, 0 );

		/* cheat threads */
#ifndef OVERLAY_ONLY
		CreateThread( 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(core::run_thread), 0, 0, 0 );
#endif

		_log::message( _( "threads created" ) );
	};

	create_threads( );

	while ( !overlay::initialized )
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

#ifndef OVERLAY_ONLY
	_log::message( _( "initialized, launch escape from tarkov now!" ) );

	do
	{
		g::pid = memory->get_process_id( _("EscapeFromTarkov.exe") );
	} while ( g::pid <= 0 );

	_log::message( _( "escape from tarkov found! pid: %d" ), g::pid );
	_log::warning( _( "searching for game modules..." ) );
	
	do
	{
		g::unity_player = memory->get_module_address( _("UnityPlayer.dll") );
		g::mono_module = memory->get_module_address( _("mono-2.0-bdwgc.dll" ) );
	} while ( g::unity_player <= 0 || g::mono_module <= 0 );
	
	_log::offset( _( "UnityPlayer.dll: 0x%llx" ), g::unity_player );
	_log::offset( _( "mono-2.0-bdwgc.dll: 0x%llx" ), g::mono_module );
	
	_log::message( _( "tarkov ready!" ) );
#endif

	while ( !input::key_pressed( VK_DELETE ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	_log::error( _( "shutting down" ) );

	TerminateProcess( OpenProcess( PROCESS_TERMINATE, FALSE, overlay::process_id ), EXIT_SUCCESS );
	
	return EXIT_SUCCESS;
}