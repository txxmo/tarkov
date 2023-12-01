#pragma once
#include "../includes.hpp"

namespace fs = std::filesystem;

static const std::string user_docs = std::getenv( "USERPROFILE" );
static const std::string documents_dir = user_docs + "\\Documents\\";
static const std::string config_dir = documents_dir + "config\\";
static const std::string extension = ".curb";

struct config_t
{
	struct aimbot_t
	{
		// main
		bool enabled = false;
		bool hitboxes[5]; // head, neck, chest, waist, legs
		int fov = 30;
		int target_select = 1;
		int smoothing = 10;
		bool prediction = true;
		int max_distance = 400;

		// other
		int recoil_mode = 0;
		bool removals_multi[2]; // spread, sway
		bool remove_spread = false;
		bool remove_sway = false;
		bool ignore_friends = true;
		bool draw_target_line = false;
	}aimbot;

	struct visuals_t // world based shit
	{
		bool enabled = false;

	}visuals;

	struct esp_t // actual player esp
	{
		bool enabled = false;

		bool boxes = false;
		float box_color[4] = { 1.f, 1.f, 1.f, 1.f };

		bool names = true;
		float name_color[4] = { 0.929412f, 0.819608f, 0.596078f, 1.f };

		bool health = true;

		bool snapline = false;
		float line_color[4] = { 0.431373f, 0.12549f, 0.0980392f, 1.f };

		bool distance = true;
		float distance_color[4] = { 0.443137f, 0.639216f, 0.819608f, 1.f };

		bool skeleton = false;
		float skeleton_color[4] = { 0.654902f, 0.87451f, 0.94902f, 1.f };

		bool weapon = true;
		float weapon_color[4] = { 0.301961f, 0.486275f, 1.f, 1.f };

		// Other groupbox
		int max_distance = 500;
		int max_ai_distance = 300;
		bool show_team = false;
		bool distance_culling = false;

	}esp;

	struct misc_t
	{
		bool enabled = false;
	}misc;
};

std::map<std::string, config_t> g_config;

namespace config
{
	static void load( const std::string& cfg_name )
	{
		fs::create_directory( config_dir );

		std::string config_file_name = cfg_name + extension;
		std::string config_path = config_dir + config_file_name;

		std::ifstream config_file( config_path, std::ios::binary );
		if ( !config_file )
		{
			_log::error( _( "could not open config path. (read error)" ) );
			return;
		}

		config_t loaded_config;
		config_file.read( reinterpret_cast<char*>(&g_config[cfg_name]), sizeof( config_t ) );

		if ( config_file.fail( ) )
		{
			_log::error( _( "failed to read config data!" ) );
			return;
		}

		config_file.close( );
	}

	static void save( const std::string& cfg_name )
	{
		fs::create_directory( config_dir );

		std::string config_file_name = cfg_name + extension;
		std::string config_path = config_dir + config_file_name;

		std::ofstream config_file( config_path, std::ios::binary );
		if ( !config_file )
		{
			_log::error( _( "could not open config path. (write error)" ) );
			return;
		}

		config_file.write( reinterpret_cast<char*>(&g_config[cfg_name]), sizeof( config_t ) );

		if ( config_file.fail( ) )
		{
			_log::error( _( "failed to write config data!" ) );
			return;
		}

		config_file.close( );
	}

	std::vector<std::string> fetch( )
	{
		std::vector<std::string> config_files;
		fs::create_directory( config_dir );

		for ( const auto& entry : fs::directory_iterator( config_dir ) )
		{
			if ( entry.is_regular_file( ) && entry.path( ).extension( ) == extension )
				config_files.push_back( entry.path( ).filename( ).stem( ).string( ) );
		}

		return config_files;
	}

	std::vector<const char*> fetch_config_names( ) {
		std::vector<const char*> config_names;

		// Call config::fetch to get the list of configuration names
		// Assuming config::fetch returns a vector of configuration names as const char*
		std::vector<std::string> config_name_str = config::fetch( );
		config_names.reserve( config_name_str.size( ) );
		for ( const auto& name : config_name_str ) {
			config_names.push_back( name.c_str( ) );
		}

		return config_names;
	}
}