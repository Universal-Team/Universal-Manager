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
#include "otherScreen.hpp"
#include "button.hpp"
#include <vector>

struct {
	int x;
	int y;
} TWLButtons[] = {
	{129, 48,}, // TWLMENU Release
	{220, 48}, // TWLMENU Nightly
	{129, 88}, // NDSB Release
	{220, 88}, // NDSB Nightly
	{129, 128}, // usrcheat.dat
};

size_t TWLButtonsTex[] = {
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
};

Structs::ButtonPos TWLFunction[] = {
	{129, 48, 87, 33, -1}, // TWLMENU Release
	{220, 48, 87, 33, -1}, // TWLMENU Nightly
	{129, 88, 87, 33, -1}, // NDSB Release
	{220, 88, 87, 33, -1}, // NDSB Nightly
	{129, 128, 87, 33, -1}, // usrcheat.dat
	{293, 213, 27, 27, -1}, // Back Button.
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void TWL::drawTWLSelection(void) const
{
	if (twlSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 30, Config::barColor);
	} else if (twlSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 30, Config::barColor);
	} else if (twlSelection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 70, Config::barColor);
	} else if (twlSelection == 3) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 70, Config::barColor);
	} else if (twlSelection == 4) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 110, Config::barColor);
	}
}

void TWL::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "TWL Updater Screen"))/2, 0, 0.72f, WHITE, "TWL Updater Screen");
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	// TWL Menu Buttons.
	for (int i = (int)(sizeof(TWLButtons)/sizeof(TWLButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(TWLButtonsTex[i], TWLButtons[i].x, TWLButtons[i].y);
	}
	Gui::sprite(sprites_titleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "TWLMENU++");
	Draw_Text(140, 58, 0.7f, WHITE, "Release");
	Draw_Text(229, 58, 0.7f, WHITE, "Nightly");

	// NDS-Bootstrap Buttons.
	Gui::sprite(sprites_titleButton_idx, 0, 88);
	Draw_Text(0, 98, 0.60f, WHITE, "NDS-Bootstrap");
	Draw_Text(140, 98, 0.7f, WHITE, "Release");
	Draw_Text(229, 98, 0.7f, WHITE, "Nightly");

	Gui::sprite(sprites_titleButton_idx, 0, 128);
	Draw_Text(0, 138, 0.7f, WHITE, "Extras");
	Draw_Text(140, 138, 0.7f, WHITE, "Cheats");

	Draw_Text(140, 4, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, WHITE, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, BLACK, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 3, RED);
	drawTWLSelection();
}

void TWL::twlSelectionLogic(u32 hDown) {
	if(hDown & KEY_UP) {
		if(twlSelection > 0)	twlSelection--;
	} else if(hDown & KEY_DOWN) {
		if(twlSelection < 4)	twlSelection++;
		} else if(hDown & KEY_A) {
			switch(twlSelection) {
				case 0: {
					if(confirmPopup("Are you sure you want to update TWiLightMenu\nTo Release?")) {
					updateTWiLight(false);
					}
					break;
				} case 1:
					if(confirmPopup("Are you sure you want to update TWiLightMenu\nTo Nightly?")) {
					updateTWiLight(true);
					}
					break;
				  case 2: {
					if(confirmPopup("Are you sure you want to update NDS-Bootstrap\nTo Release?")) {
					updateBootstrap(false);
					}
					break;
				} case 3: {
					if(confirmPopup("Are you sure you want to update NDS-Bootstrap\nTo Nightly?")) {
					updateBootstrap(true);
					}
					break;
				} case 4: {
					if(confirmPopup("Are you sure you want to download the Usrcheat.dat?")) {
					updateCheats();
					}
					break;
			}
		}
		}
}


void TWL::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	twlSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<UpdaterSub>());
	} else if (hDown & KEY_L) {
		Gui::setScreen(std::make_unique<CFW>());
	} else if (hDown & KEY_R) {
		Gui::setScreen(std::make_unique<Other>());
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE052 / \uE053 to switch Pages.");
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, TWLFunction[0])) {
			if(confirmPopup("Are you sure you want to update TWiLightMenu\nTo Release?")) {
			updateTWiLight(false);
			}
		} else if (touching(touch, TWLFunction[1])) {
			if(confirmPopup("Are you sure you want to update TWiLightMenu\nTo Nightly?")) {
			updateTWiLight(true);
			}
		} else if (touching(touch, TWLFunction[2])) {
			if(confirmPopup("Are you sure you want to update NDS-Bootstrap\nTo Release?")) {
			updateBootstrap(false);
			}
		} else if (touching(touch, TWLFunction[3])) {
			if(confirmPopup("Are you sure you want to update NDS-Bootstrap\nTo Nightly?")) {
			updateBootstrap(true);
			}
			} else if (touching(touch, TWLFunction[4])) {
			if(confirmPopup("Are you sure you want to download the Usrcheat.dat?")) {
			updateCheats();
			}
		} else if (touching(touch, TWLFunction[5])) {
			Gui::screenBack();
			return;
		}
	}
}