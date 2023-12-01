#pragma once
#include "../../includes.hpp"

namespace overlay
{
	extern HWND window;
	extern DWORD process_id;

    inline bool initialized = false;
    inline bool overlay_mode = false;

	void _do( );

	inline auto hijack_window( LPCSTR wnd_class, LPCSTR wnd_name ) -> bool
	{
        _log::warning( _( "starting window hijack" ) );
        overlay::window = FindWindowA( wnd_class, wnd_name );
        if ( !overlay::window )
        {
            _log::error( _( "failed to find window to hijack" ) );
            return false;
        }

        const DWORD ExtendedStyleFlags = WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW;
        const DWORD StyleFlags = WS_VISIBLE;

        if ( !SetWindowLongPtrA( overlay::window, GWL_EXSTYLE, ExtendedStyleFlags ) ) {
            _log::error( _( "failed to set hijacked window exstyles" ) );
            return false;
        }

        if ( !SetWindowLongPtrA( overlay::window, GWL_STYLE, WS_VISIBLE ) ) {
            _log::error( _( "failed to set hijacked window style" ) );
            return false;
        }

        if ( !SetLayeredWindowAttributes( overlay::window, RGB( 0, 0, 0 ), 0, LWA_COLORKEY ) ) {
            _log::error( _( "failed to set hijacked window colorkey" ) );
            return false;
        }

        ShowWindow( overlay::window, SW_SHOW );

        SetWindowPos( overlay::window, HWND_TOPMOST, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), SWP_SHOWWINDOW );

        GetWindowThreadProcessId( overlay::window, &overlay::process_id );

        _log::message( "overlay initialized!" );
        return true;
	}
}