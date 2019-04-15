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
	settings.universal.bordertop = settingsini.GetInt("UI", "BORDERTOP", 5); // Borders = 11
	settings.universal.borderbottom = settingsini.GetInt("UI", "BORDERBOTTOM", 8); // Borders = 11
	settings.universal.dot = settingsini.GetInt("UI", "DOT", 3); // Dots = 11
	settings.universal.button = settingsini.GetInt("UI", "BUTTON", 4); //Buttons : 7
}

/**
 * Save settings.
 */
void SaveUniversalSettings(void) {
	settingsini.SetInt("UI", "BORDERTOP", settings.universal.bordertop);
	settingsini.SetInt("UI", "BORDERBOTTOM", settings.universal.borderbottom);
	settingsini.SetInt("UI", "DOT", settings.universal.dot);
	settingsini.SetInt("UI", "BUTTON", settings.universal.button);
	settingsini.SaveIniFile("sdmc:/Universal-Updater/Settings.ini");
}

