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
	settings.universal.music = settingsini.GetInt("UI", "MUSIC", 0); // Music!
	settings.universal.text = settingsini.GetInt("UI", "TEXT", BLACK); // Text Color
	settings.universal.bars = settingsini.GetInt("UI", "BARS", 1); // Themes = 2 
	settings.universal.bg = settingsini.GetInt("UI", "BACKGROUND", 2); // Music!

}

/**
 * Save settings.
 */
void SaveUniversalSettings(void) {
	settingsini.SetInt("UI", "MUSIC", settings.universal.music);
	settingsini.SetInt("UI", "TEXT", settings.universal.text);
	settingsini.SetInt("UI", "BARS", settings.universal.bars);
	settingsini.SetInt("UI", "BACKGROUND", settings.universal.bg);
	settingsini.SaveIniFile("sdmc:/Universal-Updater/Settings.ini");
}

