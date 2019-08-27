#include "lang/lang.h"
#include "lang/langStrings.h"

#include "utils/inifile.h"

#include <fstream>

std::string langs[] = { "de", "en", "es", "fr", "it", "jp"};

void Lang::loadLangStrings(int lang) {
	// Declare variables
	char path[32];
	std::string line;

	// Load app strings
		snprintf(path, sizeof(path), "romfs:/lang/%s/app.ini", langs[lang].c_str());

	CIniFile ini(path);

	// [MainMenu]
	Lang::MainMenu[0] = ini.GetString("MainMenu", "FileManager", Lang::MainMenu[0]);
	Lang::MainMenu[1] = ini.GetString("MainMenu", "FTP", Lang::MainMenu[1]);
	Lang::MainMenu[2] = ini.GetString("MainMenu", "Scripts", Lang::MainMenu[2]);
	Lang::MainMenu[3] = ini.GetString("MainMenu", "Music", Lang::MainMenu[3]);
	Lang::MainMenu[4] = ini.GetString("MainMenu", "Updater", Lang::MainMenu[4]);
	Lang::MainMenu[5] = ini.GetString("MainMenu", "Settings", Lang::MainMenu[5]);
	
	Lang::MainMenu[6] = ini.GetString("MainMenu", "ImageViewer", Lang::MainMenu[6]);
	Lang::MainMenu[7] = ini.GetString("MainMenu", "Credits", Lang::MainMenu[7]);
	Lang::MainMenu[8] = ini.GetString("MainMenu", "TextEditor", Lang::MainMenu[8]);
	Lang::MainMenu[9] = ini.GetString("MainMenu", "Utils", Lang::MainMenu[9]);
	Lang::MainMenu[10] = ini.GetString("MainMenu", "Games", Lang::MainMenu[10]);
}