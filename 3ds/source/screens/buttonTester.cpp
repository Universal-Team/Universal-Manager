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

#include "screens/buttonTester.hpp"
#include "screens/screenCommon.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void ButtonTester::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Button Tester"))/2, 0, 0.72f, WHITE, "Button Tester");
	std::string pressedBtn = "Currently Pressed Button: ";
	pressedBtn += currentButton;
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, pressedBtn.c_str()))/2, 218, 0.72f, WHITE, pressedBtn.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void ButtonTester::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		currentButton = "\uE000";

	} else if (hDown & KEY_B) {
		currentButton = "\uE001";

	} else if (hDown & KEY_X) {
		currentButton = "\uE002";

	} else if (hDown & KEY_Y) {
		currentButton = "\uE003";

	} else if (hDown & KEY_UP) {
			currentButton = "\uE079";

		} else if (hDown & KEY_DOWN) {
			currentButton = "\uE07A";

		} else if (hDown & KEY_LEFT) {
			currentButton = "\uE07B";

		} else if (hDown & KEY_RIGHT) {
			currentButton = "\uE07C";

		} else if (hDown & KEY_L) {
			currentButton = "\uE052";

		} else if (hDown & KEY_R) {
			currentButton = "\uE053";

		} else if (hDown & KEY_ZL) {
			currentButton = "\uE054";

		} else if (hDown & KEY_ZR) {
			currentButton = "\uE055";

		} else if (hDown & KEY_START) {
			currentButton = "\uE045";

		} else if (hDown & KEY_SELECT) {
			currentButton = "\uE046";

	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, buttonTesterButtonPos[0])) {
			Gui::screenBack();
			return;
	}
}
}