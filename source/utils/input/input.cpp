#include "input.hpp"

#include <imgui/imgui_impl_win32.h>

/* This is tomos brand spanking new input handler.
*  basically its a low level keyboard hook that will act as
*  a keylogger for as long as the cheat is running.
*  not a bad thing, just is more accurate than my last
*  input handler. thanks bye o/
*/

std::mutex input::input_mutex;

LRESULT CALLBACK KeyboardHookCallback( int nCode, WPARAM wParam, LPARAM lParam ) {
    if ( nCode >= 0 ) {
        KBDLLHOOKSTRUCT* pKeyboardHookStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if ( input::imgui_initialized )
            ImGuiIO& io = ImGui::GetIO( );
        // Update previousKeys before updating pressedKeys

        input::previous_keys = input::pressed_key;

        // Check if a key is being pressed (WM_KEYDOWN) or released (WM_KEYUP)
        if ( wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ) {
            // Add the pressed key to the set
            input::pressed_key.insert( pKeyboardHookStruct->vkCode );
            if ( input::imgui_initialized )
                ImGui::GetIO( ).AddKeyEvent( ImGui_ImplWin32_VirtualKeyToImGuiKey( pKeyboardHookStruct->vkCode ), true );
            //std::cout << GetVirtualKeyCodeString( pKeyboardHookStruct->vkCode ) << std::endl;
        }
        else if ( wParam == WM_KEYUP || wParam == WM_SYSKEYUP ) {
            // Remove the released key from the set
            input::pressed_key.erase( pKeyboardHookStruct->vkCode );
            if ( input::imgui_initialized )
                ImGui::GetIO( ).AddKeyEvent( ImGui_ImplWin32_VirtualKeyToImGuiKey( pKeyboardHookStruct->vkCode ), false );
        }
    }

    // Call the next hook in the hook chain
    return CallNextHookEx( NULL, nCode, wParam, lParam );
}

bool input::key_pressed( int virtualKeyCode ) {
    return pressed_key.count( virtualKeyCode ) != 0 && previous_keys.count( virtualKeyCode ) == 0;
}

bool input::key_released( int virtualKeyCode ) {
    return pressed_key.count( virtualKeyCode ) == 0 && previous_keys.count( virtualKeyCode ) != 0;
}

bool input::key_held( int virtualKeyCode ) {
    // Check if the key was pressed and not released yet
    return pressed_key.count( virtualKeyCode ) != 0 && previous_keys.count( virtualKeyCode ) != 0;
}

HHOOK mouseHook; float scrollSensitivity = 0.01f;
LRESULT CALLBACK MouseHookCallback( int nCode, WPARAM wParam, LPARAM lParam ) {
    if ( nCode == HC_ACTION ) {
        MSLLHOOKSTRUCT* pMouseHookStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

        // Get the mouse position
        int mouseX = pMouseHookStruct->pt.x;
        int mouseY = pMouseHookStruct->pt.y;

        input::input_mutex.lock( );

        if ( wParam == WM_LBUTTONDOWN ) {
            input::mouse::isLeftButtonDown = true;
        }
        else if ( wParam == WM_LBUTTONUP ) {
            input::mouse::isLeftButtonDown = false;
        }

        if ( wParam == WM_RBUTTONDOWN ) {
            input::mouse::isRightButtonDown = true;
        }
        else if ( wParam == WM_RBUTTONUP ) {
            input::mouse::isRightButtonDown = false;
        }

        input::mouse::LogMouseMove( mouseX, mouseY );

        // Check for mouse scroll
        if ( wParam == WM_MOUSEWHEEL ) {
            int delta = GET_WHEEL_DELTA_WPARAM( pMouseHookStruct->mouseData );
            input::mouse::LogMouseScroll( delta * scrollSensitivity );
        }

        input::input_mutex.unlock( );
    }
    return CallNextHookEx( mouseHook, nCode, wParam, lParam );
}

void input::handle_keyboard( )
{
    HHOOK keyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0 );

    if ( keyboardHook == NULL ) {
        _log::error( _( "failed to install keyboard hook!" ) );
        return;
    }

    // Message loop to keep the program running
    MSG message;
    while ( GetMessage( &message, NULL, 0, 0 ) > 0 ) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }

    UnhookWindowsHookEx( keyboardHook );
}

void input::handle_mouse( )
{
    mouseHook = SetWindowsHookEx( WH_MOUSE_LL, MouseHookCallback, NULL, 0 );

    if ( mouseHook == NULL ) {
        _log::error( _( "failed to install mouse hook!" ) );
        return;
    }

    // Message loop to keep the program running
    MSG message;
    while ( GetMessage( &message, NULL, 0, 0 ) > 0 ) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }

    UnhookWindowsHookEx( mouseHook );
}