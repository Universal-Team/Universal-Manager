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
#include "fileBrowse.h"
#include "settings.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>
using std::string;

void drawTextEditorScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(80, 0, FONT_SIZE_18, WHITE, "Text Editor Screen");

	Draw_Text(0, 140, FONT_SIZE_18, WHITE, "This is a work in progress.");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
}

void TextEditorLogic(u32 hDown, u32 hHeld) {
	if (hDown & KEY_B) {
		screenMode = mainScreen2;
	}
}