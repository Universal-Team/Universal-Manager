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

#include "gui.hpp"

#include "screens/fileManagerScreen.hpp"
#include "screens/mainMenuScreen.hpp"
#include "screens/screenCommon.hpp"
#include "screens/settingsScreen.hpp"


bool touching(touchPosition touch, Structs::ButtonPos button);

void MainMenu::Draw(void) const
{
	if (screenDrawn) return;

	Gui::DrawBGTop();
	Gui::DrawBarsTop();

	Gui::DrawBGBot();
	Gui::DrawBarsBot();

	printTextCenteredScaled("Universal-Manager", 1.4, 1.4, 0, 30, true);
	printText(APP_VERSION, 180, 175, true, false);

	// Battery Icon.
	drawImage(217, 0, batteryChargeData.width, batteryChargeData.height, batteryCharge, true);
	drawImage(0, 25, menuButtonData.width, menuButtonData.height, menuButton, false);
	printTextScaled("FileManager", 1.1, 1.1, 5, 70, false);
	drawImage(130, 25, menuButtonData.width, menuButtonData.height, menuButton, false);
	printTextScaled("Settings", 1.1, 1.1, 135, 70, false);

	screenDrawn = true;
}

void MainMenu::Logic(u16 hDown, touchPosition touch) 
{
	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtonPos[0])) {
			screenDrawn = false;
			Gui::setScreen(std::make_unique<FileManager>());
		} else if (touching(touch, mainButtonPos[1])) {
			screenDrawn = false;
			Gui::setScreen(std::make_unique<Settings>());
		}
	} else if (hDown & KEY_A) {
		screenDrawn = false;
		Gui::setScreen(std::make_unique<FileManager>());
	} else if (hDown & KEY_Y) {
		screenDrawn = false;
		Gui::setScreen(std::make_unique<Settings>());
	}
}