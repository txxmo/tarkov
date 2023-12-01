#include "overlay.hpp"

#include <d3d9.h>
#include <dxgi.h>
#include <dwmapi.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_notify.h>
#include <imgui/tahoma.h>
#include <imgui/custom.hpp>
#include <imgui/fa_solid_900.h>
#include <imgui/bytes.hpp>

#include "../../gui/gui.hpp"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "Dwmapi.lib")

/* definitions for overlay to hijack */
#define HIJACK_CLASS "WMPlayerApp"
#define HIJACK_NAME "Windows Media Player"

DWORD overlay::process_id = 0;
HWND overlay::window = 0;

LPDIRECT3D9              g_pD3D = NULL;
LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
D3DPRESENT_PARAMETERS    g_d3dpp = { };

/* forward declarations of functions */
bool create_device_d3d( HWND hwnd );
void clean_device( );

void overlay::_do( )
{
	/* start windows media player to hijack its window */
	_log::warning( _( "starting windows media player" ) );
	std::system( _( "start wmplayer.exe" ) );

	/* sleep for a second to allow media player to fully start */
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

	if ( !overlay::hijack_window( _( HIJACK_CLASS ), _( HIJACK_NAME ) ) )
		return;

    if ( !create_device_d3d( overlay::window ) )
    {
        clean_device( );
        return;
    }

    // Show and update the window
    ::ShowWindow( overlay::window, SW_SHOWDEFAULT );
    ::UpdateWindow( overlay::window );

    overlay::initialized = true;

    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );
    ImGuiIO& io = ImGui::GetIO( );
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    /* disable creation of imgui.ini file */
    io.LogFilename = nullptr;
    io.IniFilename = nullptr;

    input::imgui_initialized = true;

    // Initialize ImGui Style
    ImGui::StyleColorsDark( );

    // Initialize ImGui Backends
    ImGui_ImplWin32_Init( overlay::window );
    ImGui_ImplDX9_Init( g_pd3dDevice );

    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.FontDataOwnedByAtlas = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] = {

        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\tahoma.ttf", 14, &font_config, ranges );
    io.Fonts->AddFontFromMemoryTTF( icons_2, sizeof icons_2, 15, &font_config, ranges );
    io.Fonts->AddFontFromMemoryTTF( icons_binary, sizeof icons_binary, 15, &font_config, ranges );
    io.Fonts->AddFontFromMemoryTTF( (void*)tahoma, sizeof( tahoma ), 17.f, &font_config );

    // Initialize ImGui Notify library
    ImGui::MergeIconsWithLatestFont( 16.f, false );

    MSG msg;
    ZeroMemory( &msg, sizeof( msg ) );
    bool overlay_running = true;

    while ( overlay_running )
    {
        if ( g::pid <= 0 && !overlay_mode )
            continue;

        if ( ::PeekMessageA( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            if ( msg.message == WM_QUIT )
                overlay_running = false;

            ::TranslateMessage( &msg );
            ::DispatchMessageA( &msg );
            continue;
        }

        ImGui_ImplDX9_NewFrame( );
        ImGui_ImplWin32_NewFrame( );
        ImGui::NewFrame( );

        /* forward input to ImGui from our custom input handler */
        input::input_mutex.lock( );
        {
            io.AddMousePosEvent( input::mouse::lastMousePos.x, input::mouse::lastMousePos.y );
            io.AddMouseButtonEvent( 0, input::mouse::isLeftButtonDown );
            io.AddMouseButtonEvent( 1, input::mouse::isRightButtonDown );
            io.AddMouseWheelEvent( 0.0f, static_cast<float>(input::mouse::accumulatedScrollDelta) );
            input::mouse::accumulatedScrollDelta = 0;
        }
        input::input_mutex.unlock( );

        /* render our ImGui Notifications */
        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 5.f ); // Round borders
        ImGui::PushStyleColor( ImGuiCol_WindowBg, ImVec4( 30.f / 255.f, 30.f / 255.f, 30.f / 255.f, 100.f / 255.f ) ); // Background color
        ImGui::RenderNotifications( ); // <-- Here we render all notifications
        ImGui::PopStyleVar( 1 ); // Don't forget to Pop()
        ImGui::PopStyleColor( 1 );

        gui.render( );

        ImGui::EndFrame( );

        g_pd3dDevice->Clear( 0, nullptr, D3DCLEAR_TARGET, RGB(0,0,0 ), 1.0f, 0 );
        g_pd3dDevice->BeginScene( );

        // Perform rendering and other operations here
        ImGui::Render( );
        ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

        g_pd3dDevice->EndScene( );
        g_pd3dDevice->Present( nullptr, nullptr, nullptr, nullptr );
    }

    ImGui_ImplDX9_Shutdown( );
    ImGui_ImplWin32_Shutdown( );

    ImGui::Shutdown( );

    clean_device( );

    _log::error( _( "overlay shutdown :(" ) );
}

bool create_device_d3d( HWND hwnd ) {
    RECT rc;

    if ( NULL == (g_pD3D = Direct3DCreate9( D3D_SDK_VERSION )) )
        return FALSE;

    // Create the D3DDevice
    ZeroMemory( &g_d3dpp, sizeof( g_d3dpp ) );
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if ( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice ) ) )
        return FALSE;

    return TRUE;
}

void clean_device( )
{
    if ( g_pd3dDevice ) { g_pd3dDevice->Release( ); g_pd3dDevice = NULL; }
    if ( g_pD3D ) { g_pD3D->Release( ); g_pD3D = NULL; }
}