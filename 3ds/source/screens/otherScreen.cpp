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
#include "download.hpp"
#include <algorithm>
#include <unistd.h>
#include "settings.hpp"

#include "updaterSubScreen.hpp"
#include "twlScreen.hpp"
#include "otherScreen.hpp"
#include "univScreen.hpp"

struct {
	int x;
	int y;
} OTHERButtons[] = {
	{129, 48,}, // Themes
	{220, 48}, // Scripts
};

size_t OTHERButtonsTex[] = {
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
};

Structs::ButtonPos OTHERFunction[] = {
	{129, 48, 87, 33, -1}, // Themes
	{220, 48, 87, 33, -1}, // Scripts
	{293, 213, 27, 27, -1}, // Back Button.
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Other::drawOTHERSelection(void) const
{
	if (otherSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 30, Config::barColor);
	} else if (otherSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 30, Config::barColor);
	}
}

void Other::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Other Updater Screen"))/2, 0, 0.72f, WHITE, "Other Updater Screen");
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	for (int i = (int)(sizeof(OTHERButtons)/sizeof(OTHERButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(OTHERButtonsTex[i], OTHERButtons[i].x, OTHERButtons[i].y);
	}

	Gui::sprite(sprites_titleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "Extras");
	Draw_Text(140, 58, 0.7f, WHITE, "Themes");
	Draw_Text(229, 58, 0.7f, WHITE, "Scripts");

	

	Draw_Text(140, 4, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, WHITE, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, BLACK, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 3, RED);

	drawOTHERSelection();
}

void Other::otherSelectionLogic(u32 hDown) {
	if(hDown & KEY_UP) {
		if(otherSelection > 0)	otherSelection--;
	} else if(hDown & KEY_DOWN) {
		if(otherSelection < 1)	otherSelection++;
		} else if(hDown & KEY_A) {
			switch(otherSelection) {
				case 0: {
					downloadThemes();
					break;
				} case 1:
					downloadScripts();
					break;
			}
		}
}

void Other::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	otherSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<UpdaterSub>());
	} else if (hDown & KEY_L) {
		Gui::setScreen(std::make_unique<TWL>());
	} else if (hDown & KEY_R) {
		Gui::setScreen(std::make_unique<Univ>());
	} else if (hDown & KEY_TOUCH) {
			if (touching(touch, OTHERFunction[0])) {
			downloadThemes();
		} else if (touching(touch, OTHERFunction[1])) {
			downloadScripts();
		} else if(touching(touch, OTHERFunction[2])) {
			Gui::setScreen(std::make_unique<UpdaterSub>());
		}
}
}