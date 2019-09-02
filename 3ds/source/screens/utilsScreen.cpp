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
#include "screens/calculatorScreen.hpp"
#include "screens/calendarScreen.hpp"
#include "screens/screenCommon.hpp"
#include "screens/utilsScreen.hpp"
#include "utils/settings.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Utils::drawSelection(void) const
{
	if (Selection == 0) {
		Gui::drawGUISelector(button_tl_selector_idx, 0, 39, .020f);
	} else if (Selection == 1) {
		Gui::drawGUISelector(button_selector_idx, 166, 44, .020f);
	} else if (Selection == 2) {
		Gui::drawGUISelector(button_selector_idx, 3, 154, .020f);
	}
}


void Utils::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Universal-Manager"))/2, 0, 0.72f, WHITE, "Universal-Manager");
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Utils Tab"))/2, 220, 0.72f, WHITE, "Utils Tab");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	// Buttons.
	Gui::sprite(3, button_button_tl_idx, utilsButtonPos[0].x, utilsButtonPos[0].y);
	Gui::sprite(0, sprites_calendarIcon_idx, utilsButtonPos[0].x+5, utilsButtonPos[0].y+10);
	Gui::DrawString(40, 57, 0.7f, WHITE, "Calendar");

	Gui::sprite(3, button_button_2_idx, utilsButtonPos[1].x, utilsButtonPos[1].y);
	Gui::sprite(0, sprites_buttonIcon_idx, utilsButtonPos[1].x+5, utilsButtonPos[1].y+10);
	Gui::DrawString(210, 57, 0.65f, WHITE, "Btn Tester");

	Gui::sprite(3, button_button_3_idx, utilsButtonPos[2].x, utilsButtonPos[2].y);
	Gui::DrawString(37, 167, 0.65f, WHITE, "Calculator");

	drawSelection();
}

void Utils::SelectionLogic(u32 hDown) {
		if (hDown & KEY_UP) {
			if(Selection > 0)	Selection--;
		} else if (hDown & KEY_DOWN) {
			if(Selection < 2)	Selection++;
		} else if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
					Gui::setScreen(std::make_unique<Calendar>());
					break;
				} case 1:
					Gui::setScreen(std::make_unique<ButtonTester>());
					break;
				case 2: {
					Gui::setScreen(std::make_unique<Calculator>());
					break;
				}
		}
		}
}

void Utils::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, utilsButtonPos[0])) {
			Gui::setScreen(std::make_unique<Calendar>());
		} else if (touching(touch, utilsButtonPos[1])) {
			Gui::setScreen(std::make_unique<ButtonTester>());
		} else if (touching(touch, utilsButtonPos[2])) {
			Gui::setScreen(std::make_unique<Calculator>());
		} else if (touching(touch, utilsButtonPos[3])) {
			Gui::screenBack();
			return;
		}
	}
}