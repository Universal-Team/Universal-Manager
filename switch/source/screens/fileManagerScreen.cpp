/*
*   This file is part of Universal-Manager-NX
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
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>
#include "fileManagerScreen.hpp"
#include "mainMenuScreen.hpp"

void FILEMANAGER::Draw(void) const
{
	Gui::DrawRect(0, 0, 1280, 720, BLUE);
	Gui::DrawRect(0, 0, 1280, 100, BARCOLOR);
	Gui::DrawRect(0, 620, 1280, 100, BARCOLOR);
	Gui::DrawText(250, 0, 72, WHITE, "FileManager Sub Menu");
}

void FILEMANAGER::Logic(void) {
	hidScanInput();
	hDown = hidKeysDown(CONTROLLER_P1_AUTO);

	if (hDown & KEY_A) {
		Gui::setScreen(std::make_unique<MAINMENU>());
	}
}