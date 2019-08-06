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
#include "cfwScreen.hpp"
#include "button.hpp"
#include <vector>

struct {
	int x;
	int y;
} CFWButtons[] = {
	{129, 48,}, // Luma3DS Release
	{220, 48}, // Luma3DS Nightly
	{129, 88}, // GodMode9 Release
};

size_t CFWButtonsTex[] = {
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
};

Structs::ButtonPos CFWFunction[] = {
	{129, 48, 87, 33, -1}, // Luma3DS Release
	{220, 48, 87, 33, -1}, // Luma3DS Nightly
	{129, 88, 87, 33, -1}, // GodMode9 Release
	{293, 213, 27, 27, -1}, // Back Button.
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void CFW::drawCFWSelection(void) const
{
	if (cfwSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 30, Config::barColor);
	} else if (cfwSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 30, Config::barColor);
	} else if (cfwSelection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 70, Config::barColor);
	}
}

void CFW::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "CFW Updater Screen"))/2, 0, 0.72f, WHITE, "CFW Updater Screen");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	// Luma 3DS Buttons.
	for (int i = (int)(sizeof(CFWButtons)/sizeof(CFWButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(CFWButtonsTex[i], CFWButtons[i].x, CFWButtons[i].y);
	}

	Gui::sprite(sprites_titleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "Luma3DS");
	Draw_Text(140, 58, 0.7f, WHITE, "Release");
	Draw_Text(229, 58, 0.7f, WHITE, "Nightly");

	 // GodMode9 Buttons.
	Gui::sprite(sprites_titleButton_idx, 0, 88);
	Draw_Text(0, 98, 0.65f, WHITE, "GodMode9");
	Draw_Text(140, 98, 0.7f, WHITE, "Release");

	// Draw The Pages and Back Icon.
	Draw_Text(140, 4, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, WHITE, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, BLACK, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 236, 3, RED);
	drawCFWSelection();
}

void CFW::cfwSelectionLogic(u32 hDown) {
	if(hDown & KEY_UP) {
		if(cfwSelection > 0)	cfwSelection--;
	} else if(hDown & KEY_DOWN) {
		if(cfwSelection < 2)	cfwSelection++;
		} else if(hDown & KEY_A) {
			switch(cfwSelection) {
				case 0: {
					if(confirmPopup("Are you sure you want to update Luma3DS\nTo Release?")) {
					updateLuma(false);
					}
					break;
				} case 1:
					if(confirmPopup("Are you sure you want to update Luma3DS\nTo Nightly?")) {
					updateLuma(true);
					}
					break;
				  case 2: {
					if(confirmPopup("Are you sure you want to update GodMode9\nTo Release?")) {
					downloadGodMode9();
					}
					break;
			}
		}
		}
}

void CFW::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	cfwSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_R) {
		Gui::setScreen(std::make_unique<TWL>());
	} else if (touching(touch, CFWFunction[0])) {
			if(confirmPopup("Are you sure you want to update Luma3DS\nTo Release?")) {
			updateLuma(false);
			}
		} else if (touching(touch, CFWFunction[1])) {
			if(confirmPopup("Are you sure you want to update Luma3DS\nTo Nightly?")) {
			updateLuma(true);
			}
		} else if (touching(touch, CFWFunction[2])) {
			if(confirmPopup("Are you sure you want to update GodMode9\nTo Release?")) {
			downloadGodMode9();
			}
		} else if (touching(touch, CFWFunction[3])) {
			Gui::setScreen(std::make_unique<UpdaterSub>());
		}
		}