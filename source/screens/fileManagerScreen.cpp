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

void drawFileManagerSubMenu(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Gui::staticText((i18n::localize("FILEMANAGER_SUBMENU")), 200, 0, FONT_SIZE_18, FONT_SIZE_18, WHITE, TextPosX::CENTER, TextPosY::TOP);

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Music List Button.
	Gui::sprite(sprites_mainMenuButton_idx, 100, 40);
	Gui::sprite(sprites_music_icon_idx, 105, 50);
	Gui::staticText((i18n::localize("MUSICPLAYER")), 190, 57, 0.65f, 0.65f, WHITE, TextPosX::CENTER, TextPosY::TOP);

	// Image Viewer Button.
	Gui::sprite(sprites_mainMenuButton_idx, 100, 120);
	Gui::sprite(sprites_image_icon_idx, 105, 130);
	Gui::staticText((i18n::localize("IMAGE_VIEWER")), 190, 137, 0.65f, 0.65f, WHITE, TextPosX::CENTER, TextPosY::TOP);
}