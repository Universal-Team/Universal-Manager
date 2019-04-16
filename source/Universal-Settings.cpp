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
	settings.universal.theme = settingsini.GetInt("UI", "THEME", 0); // Themes = 2 
	settings.universal.music = settingsini.GetInt("UI", "MUSIC", 0); // Music!

}

/**
 * Save settings.
 */
void SaveUniversalSettings(void) {
	settingsini.SetInt("UI", "THEME", settings.universal.theme);
	settingsini.SetInt("UI", "MUSIC", settings.universal.music);
	settingsini.SaveIniFile("sdmc:/Universal-Updater/Settings.ini");
}

