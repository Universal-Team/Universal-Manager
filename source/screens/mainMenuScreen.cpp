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

#include "screens/screenCommon.hpp"

void drawMainMenu(void) {
		drawBgTop();
		drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Universal-Manager");

		drawBgBot();
		drawBarsBot();

	volt_draw_texture(MainMenuButton, 0, 40);
	volt_draw_texture(FileManagerIcon, 5, 50);
	volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "FileManager");

	volt_draw_texture(MainMenuButton, 170, 40);
	volt_draw_texture(FTPIcon, 175, 50);
	volt_draw_text(210, 57, 0.7f, 0.7f, BLACK, "FTP Client");

	volt_draw_texture(MainMenuButton, 0, 150);
	volt_draw_texture(UpdaterIcon, 5, 160);
	volt_draw_text(40, 167, 0.7f, 0.7f, BLACK, "Updater");

	volt_draw_texture(MainMenuButton, 170, 150);
	volt_draw_texture(SettingsIcon, 175, 160);
	volt_draw_text(210, 167, 0.7f, 0.7f, BLACK, "Settings");
	volt_end_draw();
}