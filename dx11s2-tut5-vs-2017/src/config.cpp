#include "config.h"


namespace config
{
	int some_config_int = 123;
	const WCHAR* engine_version = L"0.0000001 alpha";
	std::string some_config_string = "foo";
	std::string project_directory = "Engine/data";
	std::string textures_folder = project_directory + "/textures";
	std::string shaders_folder = project_directory + "/shaders";
}

bool config::load_config_file()
{
	// Code to load and set the configuration variables

	return true;
}