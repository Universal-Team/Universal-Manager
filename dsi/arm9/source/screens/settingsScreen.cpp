	/*
*   This file is part of Universal-Manager-DSi
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "config.h"
#include "gui.hpp"

#include "screens/screenCommon.hpp"
#include "screens/settingsScreen.hpp"


bool touching(touchPosition touch, Structs::ButtonPos button);

void Settings::Draw(void) const
{
	if (screenDrawn) return;

	drawRectangle(0, 20, 256, 152, Config::Bg, true); //	Top Screen.
	drawRectangle(0, 0, 256, 20, Config::Barcolor, true);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, true);

	drawRectangle(0, 20, 256, 152, Config::Bg, false); //	Bottom Screen.
	drawRectangle(0, 0, 256, 20, Config::Barcolor, false);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, false);

	printTextTinted("Settings Screen", BLACK, 60, 5, true);

	// Battery Icon.
	drawImage(217, 0, batteryChargeData.width, batteryChargeData.height, batteryCharge, true);
	// Buttons.
	drawImage(0, 25, menuButtonData.width, menuButtonData.height, menuButton, false);
	printTextTinted("Bar Color", WHITE, 5, 30, false);
	drawImage(130, 25, menuButtonData.width, menuButtonData.height, menuButton, false);
	printTextTinted("BG Color", WHITE, 135, 30, false);

	screenDrawn = true;
}

void Settings::Logic(u16 hDown, touchPosition touch) {
	if (hDown & KEY_TOUCH) {
		if (touching(touch, SettingsButtonPos[0])) {
		Config::Barcolor = DARK_BLUE;
		Config::saveConfig();
		screenDrawn = false;
		} else if (touching(touch, SettingsButtonPos[1])) {
		Config::Bg = 0xFFF;
		Config::saveConfig();
		screenDrawn = false;
		}
	} else if (hDown & KEY_B) {
		screenDrawn = false;
		Gui::screenBack();
		return;
	}
}