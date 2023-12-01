#pragma once

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include <string>
#include <vector>
#include <functional>

using namespace std;

#define to_vec4( r, g, b, a ) ImColor( r / 255.f, g / 255.f, b / 255.f, a )

class c_custom {

public:
    float m_anim = 0.f;

    int m_tab = 0;
    vector < const char* > tabs = { "Aim", "Visuals", "Misc", "Local", "Scripts" }, tabs_icons = { "B", "D", "G", "I", "J" };

    int m_rage_subtab = 0;
    vector < const char* > rage_subtabs = { "Aimbot"  };

    int m_visuals_subtab = 0;
    vector < const char* > visuals_subtabs = { "Players", "World" };

    int m_misc_subtab = 0;
    vector < const char* > misc_subtabs = { "Main", "Config" };

    float col_buf[4] = { 1.f, 1.f, 1.f, 1.f };

    void render_arrows_for_horizontal_bar( ImVec2 pos, float alpha, float width, float height );

    bool expand_button( const char* label, bool selected, float rounding = 0.f, ImDrawFlags flags = NULL );
    bool tab( const char* icon, const char* label, bool selected );
    bool sub_tab( const char* label, bool selected );

    void tab_area( const char* str_id, ImVec2 size, std::function < void ( ) > content );

    void begin_child( const char* name, ImVec2 size );
    void end_child( );

    void multi_combo( const char* label, bool combos[ ], const char* items[ ], int items_count );
    bool button( const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags );
    bool button_out( const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags );

    int accent_color[4] = { 58, 97, 161, 255 };
    ImColor get_accent_color( float a = 1.f ) {

        return to_vec4( accent_color[0], accent_color[1], accent_color[2], a );
    }

};

inline c_custom custom;
