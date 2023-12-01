#pragma once
#include "../../includes.hpp"

namespace input
{
	extern std::mutex input_mutex;

	void handle_keyboard( );
	void handle_mouse( );

	inline std::set<int> pressed_key;
	inline std::set<int> previous_keys;

	inline bool imgui_initialized = false;

	bool key_pressed( int virtualKeyCode );

	bool key_released( int virtualKeyCode );

	bool key_held( int virtualKeyCode );

	namespace mouse
	{
		inline POINT lastMousePos = { 0, 0 };
		inline bool isLeftButtonDown = false;
		inline bool isRightButtonDown = false;
		inline int accumulatedScrollDelta = 0;

		inline bool left_click = isLeftButtonDown;
		inline bool right_click = isRightButtonDown;

		inline void LogMouseMove( int x, int y ) {
			lastMousePos.x = x;
			lastMousePos.y = y;
		}

		inline void LogLeftButtonState( bool down ) {
			isLeftButtonDown = down;
		}

		inline void LogRightButtonState( bool down ) {
			isRightButtonDown = down;
		}

		inline void LogMouseScroll( int delta ) {
			accumulatedScrollDelta += delta;
		}
	}
}

inline std::string GetVirtualKeyCodeString( int virtualKeyCode ) {
	// Get the keyboard layout of the active window
	HKL keyboardLayout = GetKeyboardLayout( 0 );

	// Create a buffer to store the key name
	char keyName[256] = { 0 };

	// Convert the virtual key code to a scan code
	DWORD scanCode = MapVirtualKeyEx( virtualKeyCode, MAPVK_VK_TO_VSC, keyboardLayout );

	// Get the key name based on the scan code
	int result = ::GetKeyNameTextA( scanCode << 16, keyName, sizeof( keyName ) );

	if ( result > 0 ) {
		return std::string( keyName );
	}
	else {
		return "Unknown";
	}
}

const char* const KeyNames[] = {
	("..."), ("M1"), ("M2"), ("BRK"), ("M3"), ("M4"), ("M5"),
	("..."), ("BCK"), ("TAB"), ("..."), ("..."), ("..."), ("ENT"), ("..."), ("..."), ("SFT"),
	("CTL"), ("ALT"), ("PUS"), ("CAP"), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("ESC"), ("..."), ("..."), ("..."), ("..."), ("SPC"), ("PGU"), ("PGD"), ("END"), ("HOM"), ("LFT"),
	("UP"), ("RGT"), ("DWN"), ("..."), ("PRN"), ("..."), ("PRNS"), ("INS"), ("DEL"), ("..."), ("0"), ("1"),
	("2"), ("3"), ("4"), ("5"), ("6"), ("7"), ("8"), ("9"), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("A"), ("B"), ("C"), ("D"), ("E"), ("F"), ("G"), ("H"), ("I"), ("J"), ("K"), ("L"), ("M"), ("N"), ("O"), ("P"), ("Q"), ("R"), ("S"), ("T"), ("U"),
	("V"), ("W"), ("X"), ("Y"), ("Z"), ("WIN"), ("WIN"), ("..."), ("..."), ("..."), ("NU0"), ("NU1"),
	("NU2"), ("NU3"), ("NU4"), ("NU5"), ("NU6"), ("NU7"), ("NU8"), ("NU9"), ("*"), ("+"), (""), ("..."), ("."), ("/"), ("F1"), ("F2"), ("F3"),
	("F4"), ("F5"), ("F6"), ("F7"), ("F8"), ("F9"), ("F10"), ("F11"), ("F12"), ("F13"), ("F14"), ("F15"), ("F16"), ("F17"), ("F18"), ("F19"), ("F20"),
	("F21"), ("F22"), ("F23"), ("F24"), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("NLK"), ("SCL"), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("SFT"), ("SFT"), ("CTL"),
	("CTL"), ("MEN"), ("MEN"), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("..."), ("..."), ("NRK"), ("PRK"), ("STP"), ("PLY"), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), (";"), ("+"), ("),"), ("..."), ("."), ("/?"), ("~"), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("{"), ("\\|"), ("}"), ("'\""), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."), ("..."),
	("..."), ("...")
};