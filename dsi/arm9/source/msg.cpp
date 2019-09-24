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

#include "gui.hpp"
#include "msg.hpp"

#include "graphics/graphics.h"

bool Msg::confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no) {
	
	do {
		Gui::DrawBarsTop();
		Gui::DrawBGTop();
		printTextCentered(msg1.c_str(), 0, 76, true);
		printTextCentered(msg2.c_str(), 0, 96, true);
		printTextCentered("A: "+yes+"   B: "+no.c_str(), 0, 140, true);
	} while(0);

	do {
		scanKeys();
		swiWaitForVBlank();
		if(keysDown() & KEY_A) {
			return true;
		} else if(keysDown() & KEY_B) {
			return false;
		}
	} while(1);
}

// For single confirm Popups.
bool Msg::confirmPopup(std::string msg) {
	return confirmPopup(msg, "", "Yes", "No");
}