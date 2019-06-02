/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "universal-Settings.hpp"
#include "inifile.h"

#include <unistd.h>
#include <string>
using std::string;
using std::wstring;

#include <3ds.h>

static CIniFile settingsini( "sdmc:/Universal-Manager/Settings.ini" );

// Settings
Settings_t settings;

void LoadUniversalSettings(void) {
	settings.universal.bars = settingsini.GetInt("UI", "BARS", BLACK); // Bars color
	settings.universal.bg = settingsini.GetInt("UI", "BG", GRAY); // Background Color
	settings.universal.music = settingsini.GetInt("UI", "MUSIC", 0); // Music Background
	settings.universal.battery = settingsini.GetInt("UI", "BATTERY", 0); // Shows the Percentage for the Battery.
	

}

/**
 * Save settings.
 */
void SaveUniversalSettings(void) {
	settingsini.SetInt("UI", "BARS", settings.universal.bars);
	settingsini.SetInt("UI", "BG", settings.universal.bg);
	settingsini.SetInt("UI", "MUSIC", settings.universal.music);
	settingsini.SetInt("UI", "BATTERY", settings.universal.battery);
	settingsini.SaveIniFile("sdmc:/Universal-Manager/Settings.ini");
}

