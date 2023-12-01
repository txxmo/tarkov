#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <termcolor/termcolor.hpp>
#endif

#include <iostream>

namespace _log {
    // Console text colors
    enum class text_color {
        default_color,
        black,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white
    };

    // Set console text color
    inline void set_text_color( text_color color ) {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
        int colorCode = 0;

        switch ( color ) {
        case text_color::default_color:
            colorCode = 15; // Default color
            break;
        case text_color::black:
            colorCode = 0; // Black
            break;
        case text_color::red:
            colorCode = 12; // Red
            break;
        case text_color::green:
            colorCode = 10; // Green
            break;
        case text_color::yellow:
            colorCode = 14; // Yellow
            break;
        case text_color::blue:
            colorCode = 9; // Blue
            break;
        case text_color::magenta:
            colorCode = 13; // Magenta
            break;
        case text_color::cyan:
            colorCode = 11; // Cyan
            break;
        case text_color::white:
            colorCode = 15; // White
            break;
        }

        SetConsoleTextAttribute( hConsole, colorCode );
#else
        std::string colorCode;

        switch ( color ) {
        case text_color::default_color:
            colorCode = termcolor::reset;
            break;
        case text_color::black:
            colorCode = termcolor::color::grey;
            break;
        case text_color::red:
            colorCode = termcolor::red;
            break;
        case text_color::green:
            colorCode = termcolor::green;
            break;
        case text_color::yellow:
            colorCode = termcolor::yellow;
            break;
        case text_color::blue:
            colorCode = termcolor::blue;
            break;
        case text_color::magenta:
            colorCode = termcolor::magenta;
            break;
        case text_color::cyan:
            colorCode = termcolor::cyan;
            break;
        case text_color::white:
            colorCode = termcolor::white;
            break;
        }

        std::cout << colorCode;
#endif
    }

    // Log an error message with color and printf-style formatting
    template <typename... Args>
    void error( const char* format, const Args&... args ) {
        set_text_color( text_color::red );
        printf( "[ ! ] " );
        set_text_color( text_color::default_color );
        printf( format, args... );
        std::cout << std::endl;  // Use std::endl instead of "\n"
        set_text_color( text_color::default_color );
        fflush( stdout );  // Flush the output buffer
    }

    // Log a message with color and printf-style formatting
    template <typename... Args>
    void message( const char* format, const Args&... args ) {
        set_text_color( text_color::green );
        printf( "[ > ] " );
        set_text_color( text_color::default_color );
        printf( format, args... );
        std::cout << std::endl;  // Use std::endl instead of "\n"
        set_text_color( text_color::default_color );
        fflush( stdout );  // Flush the output buffer
    }

    // Log a warning message with color and printf-style formatting
    template <typename... Args>
    void warning( const char* format, const Args&... args ) {
        set_text_color( text_color::yellow );
        printf( "[ * ] " );
        set_text_color( text_color::default_color );
        printf( format, args... );
        std::cout << std::endl;  // Use std::endl instead of "\n"
        set_text_color( text_color::default_color );
        fflush( stdout );  // Flush the output buffer
    }


    template <typename... Args>
    void offset( const char* format, const Args&... args ) {
        set_text_color( text_color::magenta );
        printf( "[ OFFSET ] " );
        set_text_color( text_color::default_color );
        printf( format, args... );
        std::cout << std::endl;  // Use std::endl instead of "\n"
        set_text_color( text_color::default_color );
        fflush( stdout );  // Flush the output buffer
    }
}