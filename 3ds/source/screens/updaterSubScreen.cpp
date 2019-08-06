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
#include <algorithm>
#include <unistd.h>
#include "settings.hpp"
#include "updaterSubScreen.hpp"
#include "twlScreen.hpp"
#include "mainMenuScreen.hpp"
#include "cfwScreen.hpp"
#include "otherScreen.hpp"
#include "univScreen.hpp"
#include "button.hpp"
#include <vector>

struct {
	int x;
	int y;
} mainButtons[] = {
	{38, 48}, // CFW Screen
	{129, 48}, // TWL Screen
	{220, 48}, // Other Screen
	{38, 88}, // Universal Screen
};

size_t mainButtonsTex[] = {
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
};

Structs::ButtonPos mainFunction[] = {
	{38, 48, 87, 33, -1}, // CFW Screen
	{129, 48, 87, 33, -1}, // TWL Screen
	{220, 48, 87, 33, -1}, // Other Screen
	{38, 88, 87, 33, -1}, // Universal Screen
	{293, 213, 27, 27, -1}, // Back Button.
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void UpdaterSub::drawSubSelection(void) const
{
	if (subSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 100, 38, Config::barColor);
	} else if (subSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 38, Config::barColor);
	} else if (subSelection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 38, Config::barColor);
	} else if (subSelection == 3) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 100, 78, Config::barColor);
	}
}

void UpdaterSub::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Updater Sub Menu"))/2, 0, 0.72f, WHITE, "Updater Sub Menu");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	for (int i = (int)(sizeof(mainButtons)/sizeof(mainButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(mainButtonsTex[i], mainButtons[i].x, mainButtons[i].y);
	}
	Draw_Text(49, 58, 0.7f, WHITE, "CFW");
	Draw_Text(140, 58, 0.7f, WHITE, "TWL");
	Draw_Text(229, 58, 0.7f, WHITE, "Other");
	Draw_Text(49, 98, 0.7f, WHITE, "UNIV");
	drawSubSelection();
}

void UpdaterSub::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(hDown & KEY_UP) {
		if(subSelection > 0)	subSelection--;
	} else if(hDown & KEY_DOWN) {
		if(subSelection < 3)	subSelection++;
		} else if(hDown & KEY_A) {
			switch(subSelection) {
				case 0: {
					Gui::setScreen(std::make_unique<CFW>());
					break;
				} case 1:
					Gui::setScreen(std::make_unique<TWL>());
					break;
				  case 2: {
					Gui::setScreen(std::make_unique<Other>());
					break;
				} case 3: {
					Gui::setScreen(std::make_unique<Univ>());
					break;
				}
			}
	} else if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<MainMenu>());

	} else if(hDown & KEY_TOUCH) {
				if (touching(touch, mainFunction[0])) {
			Gui::setScreen(std::make_unique<CFW>());
		} else if (touching(touch, mainFunction[1])) {
			Gui::setScreen(std::make_unique<TWL>());
		} else if (touching(touch, mainFunction[2])) {
			Gui::setScreen(std::make_unique<Other>());
		} else if (touching(touch, mainFunction[3])) {
			Gui::setScreen(std::make_unique<Univ>());
		} else if(touching(touch, mainFunction[4])) {
			Gui::screenBack();
			return;
		}
	}
}
