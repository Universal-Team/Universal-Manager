#include "config.h"
#include "inifile.h"
#include "graphics/graphics.h"

int Config::Barcolor, Config::Bg;

void Config::loadConfig() {
	CIniFile ini("/_nds/Universal-Manager/settings.ini");
	Config::Barcolor = ini.GetInt("UI", "BARCOLOR", BARCOLOR);
	Config::Bg = ini.GetInt("UI", "BG", GRAY);
}

void Config::saveConfig() {
	CIniFile ini("/_nds/Universal-Manager/settings.ini");
	ini.SetInt("UI", "BARCOLOR", Config::Barcolor);
	ini.SetInt("UI", "BG", Config::Bg);
	ini.SaveIniFile("/_nds/Universal-Manager/settings.ini");
}
