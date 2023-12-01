#define IMGUI_DEFINE_MATH_OPERATORS
#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )

#include "gui.hpp"
#include "../config/config.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_notify.h>
#include <imgui/tahoma.h>
#include <imgui/custom.hpp>
#include <imgui/fa_solid_900.h>

menu gui;
static char config_name_buffer[64] = "main";

#define settings g_config[ config_name_buffer ]

static bool menu_open = true;

void menu::render( )
{
    using namespace ImGui;

    static bool prev_key_state = false;
    if ( input::key_pressed( VK_INSERT ) && !prev_key_state )
    {
        menu_open = !menu_open;
        prev_key_state = true;
    }
    else if ( !input::key_pressed( VK_INSERT ) )
        prev_key_state = false;

    if ( !menu_open )
        return;

    vector < const char* > target_selection = { "closest to crosshair", "lowest health", "highest damage", "closest disttance" };
    vector < const char* > recoil_modes = { "none", "legit", "semi-rage", "rage" };

    const char* hitboxes_items[] = { "head", "neck", "chest", "waist", "legs" };

    const char* removals_items[] = { "spread", "sway" };

    PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0, 0 ) );
    PushStyleVar( ImGuiStyleVar_WindowMinSize, ImVec2( 510, 380 ) );

    ImGui::Begin( _( "##MenuPanel" ), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse );
    {

        auto window = GetCurrentWindow( );
        auto draw = window->DrawList;
        auto pos = window->Pos;
        auto size = window->Size;
        auto style = GetStyle( );

        custom.m_anim = ImLerp( custom.m_anim, 1.f, 0.03f );

        draw->AddLine( pos + ImVec2( 65, 40 ), pos + ImVec2( size.x - 15, 40 ), ImColor( 1.f, 1.f, 1.f, 0.05f ) );

        switch ( custom.m_tab ) {

        case 0: // aimbot

            SetCursorPos( ImVec2( 70, 13 ) );
            BeginGroup( );

            for ( int i = 0; i < custom.rage_subtabs.size( ); ++i ) {

                if ( custom.sub_tab( custom.rage_subtabs.at( i ), custom.m_rage_subtab == i ) && custom.m_rage_subtab != i )
                    custom.m_rage_subtab = i, custom.m_anim = 0.f;

                if ( i != custom.rage_subtabs.size( ) - 1 )
                    SameLine( );
            }

            EndGroup( );

            SetCursorPos( ImVec2( 65, 50 ) );
            BeginChild( "##rage_childs", ImVec2( GetWindowWidth( ) - 80, GetWindowHeight( ) - 80 ) );
            {

                switch ( custom.m_rage_subtab ) {

                case 0:

                    custom.begin_child( "General", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                        Checkbox( "enabled", &settings.aimbot.enabled );
                        Checkbox( "prediction", &settings.aimbot.prediction );
                        SliderInt( "field of view", &settings.aimbot.fov, 0, 180 );
                        SliderInt( "smoothing", &settings.aimbot.smoothing, 0, 100 );
                        Combo( "target selection", &settings.aimbot.target_select, target_selection.data( ), target_selection.size( ) );
                        custom.multi_combo( "aimbot hitboxes", settings.aimbot.hitboxes, hitboxes_items, IM_ARRAYSIZE( hitboxes_items ) );

                    } custom.end_child( );

                    SameLine( );

                    custom.begin_child( "Other", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                        Combo( "recoil mode", &settings.aimbot.recoil_mode, recoil_modes.data( ), recoil_modes.size( ) );
                        custom.multi_combo( "removals", settings.aimbot.removals_multi, removals_items, IM_ARRAYSIZE( removals_items ) );
                        SliderInt( "max distance", &settings.aimbot.max_distance, 0, 1000 );
                        Checkbox( "ignore friends", &settings.aimbot.ignore_friends );
                        Checkbox( "draw aim line", &settings.aimbot.draw_target_line );

                    } custom.end_child( );

                    break;
                }

            } EndChild( );

            break;

        case 1: // visuals

            SetCursorPos( ImVec2( 70, 13 ) );
            BeginGroup( );

            for ( int i = 0; i < custom.visuals_subtabs.size( ); ++i ) {

                if ( custom.sub_tab( custom.visuals_subtabs.at( i ), custom.m_visuals_subtab == i ) && custom.m_visuals_subtab != i )
                    custom.m_visuals_subtab = i, custom.m_anim = 0.f;

                if ( i != custom.visuals_subtabs.size( ) - 1 )
                    SameLine( );
            }

            EndGroup( );

            SetCursorPos( ImVec2( 65, 50 ) );
            BeginChild( "##visuals_childs", ImVec2( GetWindowWidth( ) - 80, GetWindowHeight( ) - 80 ) );
            {

                switch ( custom.m_visuals_subtab ) {

                case 0:

                    custom.begin_child( "ESP", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                        Checkbox( "enabled", &settings.esp.enabled );

                        Checkbox( "box", &settings.esp.boxes ); SameLine( GetWindowWidth( ) - 33 ); ColorEdit4( "boxes##color", settings.esp.box_color, ALPHA );
                        Checkbox( "names", &settings.esp.names ); SameLine( GetWindowWidth( ) - 33 ); ColorEdit4( "names##color", settings.esp.name_color, ALPHA );
                        Checkbox( "health", &settings.esp.names );
                        Checkbox( "snaplines", &settings.esp.snapline ); SameLine( GetWindowWidth( ) - 33 ); ColorEdit4( "lines##color", settings.esp.line_color, ALPHA );
                        Checkbox( "distance", &settings.esp.distance ); SameLine( GetWindowWidth( ) - 33 ); ColorEdit4( "distance##color", settings.esp.distance_color, ALPHA );
                        Checkbox( "weapon", &settings.esp.weapon ); SameLine( GetWindowWidth( ) - 33 ); ColorEdit4( "weapon##color", settings.esp.weapon_color, ALPHA );
                        Checkbox( "skeleton", &settings.esp.skeleton ); SameLine( GetWindowWidth( ) - 33 ); ColorEdit4( "skele##color", settings.esp.skeleton_color, ALPHA );


                    } custom.end_child( );

                    SameLine( );

                    custom.begin_child( "Settings", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                        Checkbox( "show team", &settings.esp.show_team );
                        Checkbox( "distance culling", &settings.esp.distance_culling );
                        SliderInt( "max pmc distance", &settings.esp.max_distance, 0, 5000, "%d" );
                        SliderInt( "max ai distance", &settings.esp.max_ai_distance, 0, 5000 );


                    } custom.end_child( );

                    break;

                }

            } EndChild( );

            break;

        case 2: // misc

            SetCursorPos( ImVec2( 70, 13 ) );
            BeginGroup( );

            for ( int i = 0; i < custom.misc_subtabs.size( ); ++i ) {

                if ( custom.sub_tab( custom.misc_subtabs.at( i ), custom.m_misc_subtab == i ) && custom.m_misc_subtab != i )
                    custom.m_misc_subtab = i, custom.m_anim = 0.f;

                if ( i != custom.misc_subtabs.size( ) - 1 )
                    SameLine( );
            }

            EndGroup( );

            SetCursorPos( ImVec2( 65, 50 ) );
            BeginChild( "##misc_childs", ImVec2( GetWindowWidth( ) - 80, GetWindowHeight( ) - 80 ) );
            {

                switch ( custom.m_misc_subtab ) {

                case 0:

                    custom.begin_child( "Misc", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                    } custom.end_child( );

                    SameLine( );

                    custom.begin_child( "Extra", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                    } custom.end_child( );

                    break;

                case 1:

                    custom.begin_child( "Configuration", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {


                    } custom.end_child( );

                    SameLine( );

                    custom.begin_child( "Settings", ImVec2( GetWindowWidth( ) / 2 - style.ItemSpacing.x / 2, GetWindowHeight( ) ) );
                    {

                        if ( custom.button_out( "save config##RARAR", ImVec2( GetWindowWidth( ) - 20, 20 ), ImGuiButtonFlags_None ) )
                        {
                            config::save( config_name_buffer );

                            ImGuiToast toast( ImGuiToastType_Success, 3000 ); // <-- content can also be passed here as above
                            toast.set_title( "configuration saved" );
                            toast.set_content( "config saved to user documents" );
                            ImGui::InsertNotification( toast );
                        }

                        if ( custom.button_out( "load config##RARAR", ImVec2( GetWindowWidth( ) - 20, 20 ), ImGuiButtonFlags_None ) )
                        {
                            config::load( config_name_buffer );

                            ImGuiToast toast( ImGuiToastType_Success, 3000 ); // <-- content can also be passed here as above
                            toast.set_title( "configuration loaded" );
                            toast.set_content( "config loaded from user documents" );
                            ImGui::InsertNotification( toast );
                        }

                    } custom.end_child( );

                    break;

                }

            } EndChild( );

            break;

        }

        SetCursorPosY( 0 );
        custom.tab_area( "##tab_area", ImVec2( 50, size.y - 20 ), [] ( ) {

            for ( int i = 0; i < custom.tabs.size( ); ++i )
                if ( custom.tab( custom.tabs_icons.at( i ), custom.tabs.at( i ), custom.m_tab == i ) && custom.m_tab != i )
                    custom.m_tab = i, custom.m_anim = 0.f;

        } );

        // footer
        draw->AddRectFilled( pos + ImVec2( 0, size.y - 20 ), pos + size, ImColor( 25, 25, 25 ), style.WindowRounding, ImDrawFlags_RoundCornersBottom );
        draw->AddText( pos + ImVec2( 5, size.y - 18 ), GetColorU32( ImGuiCol_Text ), "pandora v3 for tarkov?? wadahel" );
        float framerate = ImGui::GetIO( ).Framerate;
        std::string framerateString = std::to_string( framerate );

        size_t decimalPos = framerateString.find( '.' );
        if ( decimalPos != std::string::npos ) {
            framerateString.erase( decimalPos );
        }

        draw->AddText( pos + ImVec2( size.x - CalcTextSize( framerateString.c_str( ) ).x - 5, size.y - 18 ), GetColorU32( ImGuiCol_Text ), framerateString.c_str( ) );

    } ImGui::End( );

    PopStyleVar( 2 );
}