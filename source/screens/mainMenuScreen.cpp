	/*
*   This file is part of Universal-Manager
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

#include "screens/screenCommon.hpp"
#include "i18n.hpp"

// Version numbers.
char universal_manager_vertext[13];

void drawMainMenu(void) {
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	//Gui::sprite(sprites_battery0_idx, 361, 0); // Showing Purpose.
	Gui::staticText((i18n::localize("UNIVERSAL_MANAGER")), 200, 0, 0.72f, 0.72f, WHITE, TextPosX::CENTER, TextPosY::TOP);
	draw_text(340, 218, FONT_SIZE_18, FONT_SIZE_18, WHITE, universal_manager_vertext);

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();

	Gui::sprite(sprites_mainMenuButton_idx, 0, 40);
	Gui::sprite(sprites_fileManagerIcon_idx, 5, 50);
	Gui::staticText((i18n::localize("FILEMANAGER")), 85, 57, 0.65f, 0.65f, WHITE, TextPosX::CENTER, TextPosY::TOP);

	Gui::sprite(sprites_mainMenuButton_idx, 170, 40);
	Gui::sprite(sprites_ftpIcon_idx, 175, 50);
	Gui::staticText((i18n::localize("FTP")), 230, 57, 0.7f, 0.7f, WHITE, TextPosX::CENTER, TextPosY::TOP);

	Gui::sprite(sprites_mainMenuButton_idx, 0, 150);
	Gui::sprite(sprites_updaterIcon_idx, 5, 160);
	Gui::staticText((i18n::localize("UPDATER")), 80, 167, 0.7f, 0.7f, WHITE, TextPosX::CENTER, TextPosY::TOP);

	Gui::sprite(sprites_mainMenuButton_idx, 170, 150);
	Gui::sprite(sprites_settingsIcon_idx, 175, 160);
	Gui::staticText((i18n::localize("SETTINGS")), 245, 167, 0.7f, 0.7f, WHITE, TextPosX::CENTER, TextPosY::TOP);
}
