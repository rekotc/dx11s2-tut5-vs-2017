#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <Windows.h>
#include<string.h>

namespace config
{
	extern int some_config_int;
	extern std::string some_config_string;
	extern const WCHAR* engine_version;
	extern std::string project_directory;
	extern std::string textures_folder;
	extern std::string shaders_folder;

	bool load_config_file();
}

#endif