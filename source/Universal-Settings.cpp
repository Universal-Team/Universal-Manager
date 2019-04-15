#include "Universal-Settings.hpp"
#include "inifile.h"

#include <unistd.h>
#include <string>
using std::string;
using std::wstring;

#include <3ds.h>

static CIniFile settingsini( "sdmc:/Universal-Updater/Settings.ini" );

// Settings
Settings_t settings;

void LoadUniversalSettings(void) {
	settings.universal.theme = settingsini.GetInt("UI", "THEME", 1); // Themes = 2 

}

/**
 * Save settings.
 */
void SaveUniversalSettings(void) {
	settingsini.SetInt("UI", "THEME", settings.universal.theme);
	settingsini.SaveIniFile("sdmc:/Universal-Updater/Settings.ini");
}

