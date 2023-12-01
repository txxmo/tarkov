#include "custom.hpp"

#include <map>
#include "imgui_internal.h"

using namespace ImGui;

void c_custom::render_arrows_for_horizontal_bar( ImVec2 pos, float alpha, float width, float height ) {

    auto draw = GetWindowDrawList();
    draw->AddRect( pos, pos + ImVec2( width, height ), ImColor( 0.f, 0.f, 0.f, alpha * GetStyle( ).Alpha ), 4 );
}

bool c_custom::expand_button( const char* label, bool selected, float rounding, ImDrawFlags flags ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );
    ImVec2 icon_size = GetIO( ).Fonts->Fonts[2]->CalcTextSizeA( GetIO( ).Fonts->Fonts[2]->FontSize - 2, FLT_MAX, 0, "A" );
    ImVec2 pos = window->DC.CursorPos;

    auto draw = window->DrawList;

    ImRect rect( pos, pos + ImVec2( GetWindowWidth( ), 45 ) );
    ItemAdd( rect, id );
    ItemSize( rect, GetStyle( ).FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( rect, id, &hovered, &held, NULL );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( selected ? 1.f : 0.f ), 0.035f );

    draw->AddRectFilled( rect.Min, rect.Max, ImColor( 1.f, 1.f, 1.f, 0.025f * value->second ), rounding, flags );
    draw->AddText( GetIO( ).Fonts->Fonts[2], GetIO( ).Fonts->Fonts[2]->FontSize - 2, ImVec2( rect.Min.x + 50 / 2 - icon_size.x / 2, rect.GetCenter( ).y - icon_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), "A" );
    draw->AddText( ImVec2( rect.Min.x + 50, rect.GetCenter( ).y - label_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), label );

    return pressed;
}

bool c_custom::tab( const char* icon, const char* label, bool selected ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );
    ImVec2 icon_size = GetIO( ).Fonts->Fonts[1]->CalcTextSizeA( GetIO( ).Fonts->Fonts[1]->FontSize, FLT_MAX, 0, icon );
    ImVec2 pos = window->DC.CursorPos;

    auto draw = window->DrawList;

    ImRect rect( pos, pos + ImVec2( GetWindowWidth( ), 40 ) );
    ItemAdd( rect, id );
    ItemSize( rect, GetStyle( ).FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( rect, id, &hovered, &held, NULL );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( selected || hovered ? 1.f : 0.f ), 0.035f );

    draw->AddText( GetIO( ).Fonts->Fonts[1], GetIO( ).Fonts->Fonts[1]->FontSize, ImVec2( rect.Min.x + 50 / 2 - icon_size.x / 2, rect.GetCenter( ).y - icon_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), icon );
    draw->AddText( ImVec2( rect.Min.x + 50, rect.GetCenter( ).y - label_size.y / 2 ), GetColorU32( ImGuiCol_Text, ( value->second > 0.3f ? value->second : 0.3f ) ), label );

    return pressed;
}

bool c_custom::sub_tab( const char* label, bool selected ) {

    ImGuiWindow* window = GetCurrentWindow( );
    ImGuiID id = window->GetID( label );

    ImVec2 label_size = CalcTextSize( label, 0, 1 );
    ImVec2 pos = window->DC.CursorPos;

    auto draw = window->DrawList;

    ImRect rect( pos, pos + label_size );
    ItemAdd( rect, id );
    ItemSize( ImVec4( rect.Min.x, rect.Min.y, rect.Max.x + 15, rect.Max.y ), GetStyle( ).FramePadding.y );

    bool hovered, held;
    bool pressed = ButtonBehavior( rect, id, &hovered, &held, NULL );

    static std::unordered_map < ImGuiID, float > values;
    auto value = values.find( id );
    if ( value == values.end( ) ) {

        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, ( selected ? 1.f : 0.f ), 0.035f );
    ImColor col = selected ? custom.get_accent_color( value->second * GetStyle( ).Alpha ) : ImColor( GetColorU32( ImGuiCol_Text, ( 155 ) ) );
    draw->AddText( rect.Min, col, label );
    //draw->AddRectFilled( ImVec2( rect.GetCenter( ).x, rect.Max.y + 10 ), 2 * value->second, custom.get_accent_color( value->second * GetStyle( ).Alpha ) );
    return pressed;
}

struct tab_area_structure {

    bool hovered, should_expand;
    float width;
};

void c_custom::tab_area( const char* str_id, ImVec2 size, std::function < void ( ) > content ) {

    static std::unordered_map < std::string, tab_area_structure > values;
    auto value = values.find( std::string( str_id ) );
    if ( value == values.end( ) ) {

        values.insert( { std::string( str_id ), { false, false, size.x } } );
        value = values.find( std::string( str_id ) );
    }

    value->second.width = ImLerp( value->second.width, ( value->second.should_expand && value->second.hovered ) ? size.x + 60 : size.x, 0.08f );

    if ( ( value->second.should_expand && value->second.hovered ) )
        GetForegroundDrawList( )->AddRectFilled( GetWindowPos( ) + ImVec2( value->second.width, 0 ), GetWindowPos( ) + GetWindowSize( ), ImColor( 0.f, 0.f, 0.f, 0.2f ), GetStyle( ).WindowRounding );

    PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );

    BeginChild( std::string( str_id ).append( ".child" ).c_str( ), ImVec2( value->second.width, size.y ) );

    value->second.hovered = IsMouseHoveringRect( GetCurrentWindow( )->Pos, GetCurrentWindow( )->Pos + ImVec2( value->second.width, size.y ) ); // could use IsWindowHovered( ); , but it bugs

    GetWindowDrawList( )->AddRectFilled( GetCurrentWindow( )->Pos, GetCurrentWindow( )->Pos + GetCurrentWindow( )->Size, ImColor( 25, 25, 25 ), GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersTopLeft );

    if ( expand_button( "Expand", value->second.should_expand, GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersTopLeft ) )
        value->second.should_expand = !value->second.should_expand;

    content( );

    EndChild( );

    PopStyleVar( );

}

void c_custom::begin_child( const char* name, ImVec2 size ) {

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow( );

    ImVec2 pos = window->DC.CursorPos;

    BeginChild( std::string( name ).append( ".main" ).c_str( ), size, false, ImGuiWindowFlags_NoScrollbar );

    GetWindowDrawList( )->AddRectFilled( pos, pos + size, to_vec4( 35, 35, 35, custom.m_anim ), 4 ); // ImColor( 28, 28, 29 ) : ImColor( 22, 22, 23 )
    GetWindowDrawList( )->AddText( pos + ImVec2( 10, 8 ), custom.get_accent_color( custom.m_anim ), name, FindRenderedTextEnd( name ) );

    SetCursorPosY( 30 );
    BeginChild( name, { size.x, size.y - 30 } );
    SetCursorPosX( 10 );

    BeginGroup( );

    PushStyleVar( ImGuiStyleVar_Alpha, custom.m_anim );
    PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8, 8 } );
}

void c_custom::end_child( ) {

    PopStyleVar( 2 );
    EndGroup( );
    EndChild( );
    EndChild( );
}

void c_custom::multi_combo( const char* label, bool combos[ ], const char* items[ ], int items_count )
{
    std::vector<std::string> vec;
    static std::string preview;
    for( int i = 0, j = 0; i < items_count; i++ )
    {
        if( combos[ i ] )
        {
            vec.push_back( items[ i ] );
            if( j > 3 )
                preview = vec.at( 0 ) + ", " + vec.at( 1 ) + ", " + vec.at( 2 ) + ", ...";
            else if( j )
                preview += ", " + ( std::string )items[ i ];
            else
                preview = items[ i ];

            j++;
        }
    }
    if( BeginCombo( label, preview.c_str( ) ) )
    {
        for( int i = 0; i < items_count; i++ ) {
            PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2( 0, 0 ) );
            Selectable( items[ i ], &combos[ i ], ImGuiSelectableFlags_DontClosePopups | ImGuiSelectableFlags_NoPadWithHalfSpacing, ImVec2( 0, 20 ) );
            PopStyleVar( );
        }
        EndCombo( );
    }

    preview = "none";
}

struct c_button_anims {
    float closing_anim;
    float closing_alpha;
    float label_alpha;
};

bool c_custom::button( const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );
    ImDrawList* draw = GetWindowDrawList( );
    static std::map<ImGuiID, c_button_anims> anim; // #include <map> on top of this file, where is "imgui.h" located.
    auto it_anim = anim.find( id );
    if( it_anim == anim.end( ) )
    {
        anim.insert( { id, { 0.f, 0.f, 0.f } } );
        it_anim = anim.find( id );
    }

    ImVec2 pos = window->DC.CursorPos;
    if( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if( !ItemAdd( bb, id ) )
        return false;

    if( g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat )
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );

    it_anim->second.closing_anim = ImLerp( it_anim->second.closing_anim, ( hovered || held ? size.x / 2.f : 0 ), g.IO.DeltaTime * 9.f );

    if( hovered || pressed ) {
        if( it_anim->second.label_alpha < 255.f )
            it_anim->second.label_alpha += 5.f / GetIO( ).Framerate * 160.f;

        if( it_anim->second.closing_alpha < 255.f )
            it_anim->second.closing_alpha += 5.f / GetIO( ).Framerate * 160.f;
    }
    else {
        if( it_anim->second.label_alpha > 0.f )
            it_anim->second.label_alpha -= 5.f / GetIO( ).Framerate * 160.f;

        if( it_anim->second.closing_alpha > 0.f )
            it_anim->second.closing_alpha -= 5.f / GetIO( ).Framerate * 160.f;
    }

    // Render
    const ImU32 inside_solid_col = GetColorU32( ImGuiCol_WindowBg );
    const ImU32 outside_solid_col = ImColor( 30, 30, 30, ( int )it_anim->second.closing_alpha );
    const ImU32 inside_hover_col = ImColor( 58, 97, 161, ( int )it_anim->second.closing_alpha );

    RenderFrame( bb.Min, bb.Max, outside_solid_col, false, 2 );
    draw->AddRectFilled( bb.Min, ImVec2( bb.Min.x + it_anim->second.closing_anim, bb.Max.y ), inside_hover_col, 2, ImDrawCornerFlags_Left );
    draw->AddRectFilled( ImVec2( bb.Max.x - it_anim->second.closing_anim, bb.Min.y ), bb.Max, inside_hover_col, 2, ImDrawCornerFlags_Right );
    draw->AddRect( bb.Min, bb.Max, ImColor( 1.f, 1.f, 1.f, 0.07f * style.Alpha ), 2 );

    PushStyleColor( ImGuiCol_Text, ColorConvertFloat4ToU32( ImColor( 225, 225, 225, 255 - ( int )it_anim->second.label_alpha ) ) );
    if( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor( );

    PushStyleColor( ImGuiCol_Text, ColorConvertFloat4ToU32( ImColor( 255, 255, 255, ( int )it_anim->second.label_alpha ) ) );
    if( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed;
}

struct c_a_button_anims {
    float closing_anim_X;
    float closing_anim_Y;
    float closing_alpha;
    float label_alpha;
};

bool c_custom::button_out( const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags )
{
    ImGuiWindow* window = GetCurrentWindow( );
    if( window->SkipItems )
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID( label );
    const ImVec2 label_size = CalcTextSize( label, NULL, true );
    ImDrawList* draw = GetWindowDrawList( );


    static std::map<ImGuiID, c_a_button_anims> anim; // #include <map> on top of this file, where is "imgui.h" located.
    auto it_anim = anim.find( id );
    if( it_anim == anim.end( ) )
    {
        anim.insert( { id, { 0.f, 0.f, 0.f, 0.f } } );
        it_anim = anim.find( id );
    }

    ImVec2 pos = window->DC.CursorPos;
    if( ( flags & ImGuiButtonFlags_AlignTextBaseLine ) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset ) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize( size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f );

    const ImRect bb( pos, pos + size );
    ItemSize( size, style.FramePadding.y );
    if( !ItemAdd( bb, id ) )
        return false;

    if( g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat )
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior( bb, id, &hovered, &held, flags );

    it_anim->second.closing_anim_X = ImLerp( it_anim->second.closing_anim_X, ( hovered || held ? size.x / 2.f : 0 ), g.IO.DeltaTime * 6.f );
    it_anim->second.closing_anim_Y = ImLerp( it_anim->second.closing_anim_Y, ( hovered || held ? size.y / 2.f : 0 ), g.IO.DeltaTime * 6.f );

    if( hovered || pressed ) {
        if( it_anim->second.label_alpha < 255.f )
            it_anim->second.label_alpha += 5.f / GetIO( ).Framerate * 160.f;

        if( it_anim->second.closing_alpha < 255.f )
            it_anim->second.closing_alpha += 15.f / GetIO( ).Framerate * 160.f;
    }
    else {
        if( it_anim->second.label_alpha > 0.f )
            it_anim->second.label_alpha -= 5.f / GetIO( ).Framerate * 160.f;

        if( it_anim->second.closing_alpha > 0.f )
            it_anim->second.closing_alpha -= 10.f / GetIO( ).Framerate * 160.f;
    }

    // Render
    const ImU32 inside_solid_col = GetColorU32( ImGuiCol_WindowBg );
    const ImU32 outside_solid_col = ImColor( 30, 30, 30, ( int )it_anim->second.closing_alpha );
    const ImU32 inside_hover_col = ImColor( 58, 97, 161, ( int )it_anim->second.closing_alpha );

    RenderFrame( bb.Min, bb.Max, outside_solid_col, false, 2 );
    draw->AddRectFilled( ImVec2( ( bb.Min.x + size.x / 2.f ) - it_anim->second.closing_anim_X, ( bb.Min.y + size.y / 2.f ) - it_anim->second.closing_anim_Y ), ImVec2( ( bb.Max.x - size.x / 2.f ) + it_anim->second.closing_anim_X, ( bb.Max.y - size.y / 2.f ) + it_anim->second.closing_anim_Y ), inside_hover_col, 3 );
    draw->AddRect( bb.Min, bb.Max, ImColor( 1.f, 1.f, 1.f, 0.07f * style.Alpha ), 2 );

    PushStyleColor( ImGuiCol_Text, ColorConvertFloat4ToU32( ImColor( 225, 225, 225, 255 - ( int )it_anim->second.label_alpha ) ) );
    if( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor( );

    PushStyleColor( ImGuiCol_Text, ColorConvertFloat4ToU32( ImColor( 255, 255, 255, ( int )it_anim->second.label_alpha ) ) );
    if( g.LogEnabled )
        LogSetNextTextDecoration( "[", "]" );
    RenderTextClipped( bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb );
    PopStyleColor( );

    IMGUI_TEST_ENGINE_ITEM_INFO( id, label, g.LastItemData.StatusFlags );
    return pressed;
}
