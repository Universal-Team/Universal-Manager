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
#include "otherScreen.hpp"
#include "univScreen.hpp"
#include "button.hpp"
#include <vector>

struct {
	int x;
	int y;
} UNIVButtons[] = {
	{129, 48,}, // UNIV Manager Release
	{220, 48}, // UNIV Manager Nightly
	{129, 88}, // Pokemon Chest Release
	{220, 88}, // Pokemon Chest Nightly
	{129, 128}, // Relaunch Release
	{220, 128}, // Relaunch Nightly
};

size_t UNIVButtonsTex[] = {
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
	sprites_updaterButton_idx,
};

Structs::ButtonPos UNIVFunction[] = {
	{129, 48, 87, 33, -1}, // Univ Manager Release 
	{220, 48, 87, 33, -1}, // Univ Manager Nightly
	{129, 88, 87, 33, -1}, // Pokemon Chest Release
	{220, 88, 87, 33, -1}, // Pokemon Chest Nightly
	{129, 128, 87, 33, -1}, // Relaunch Release
	{220, 128, 87, 33, -1}, // Relaunch Nightly
	{293, 213, 27, 27, -1}, // Back Button.
};

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Univ::drawUNIVSelection(void) const
{
	if (univSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 30, Config::barColor);
	} else if (univSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 30, Config::barColor);
	} else if (univSelection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 70, Config::barColor);
	} else if (univSelection == 3) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 70, Config::barColor);
	} else if (univSelection == 4) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 110, Config::barColor);
	} else if (univSelection == 5) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 110, Config::barColor);
	}
}

void Univ::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Universal Updater Screen"))/2, 0, 0.72f, WHITE, "Universal Updater Screen");
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	// Universal-Manager Buttons.

	for (int i = (int)(sizeof(UNIVButtons)/sizeof(UNIVButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(UNIVButtonsTex[i], UNIVButtons[i].x, UNIVButtons[i].y);
	}

	Gui::sprite(sprites_titleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "UNIV-Manager");
	Draw_Text(140, 58, 0.7f, WHITE, "Release");
	Draw_Text(229, 58, 0.7f, WHITE, "Nightly");

	// PKMN-Chest.
	Gui::sprite(sprites_titleButton_idx, 0, 88);
	Draw_Text(0, 98, 0.65f, WHITE, "PKMN-Chest");
	Draw_Text(140, 98, 0.7f, WHITE, "Release");
	Draw_Text(229, 98, 0.7f, WHITE, "Nightly");

	Gui::sprite(sprites_titleButton_idx, 0, 128);
	Draw_Text(0, 138, 0.65f, WHITE, "RELAUNCH");
	Draw_Text(140, 138, 0.7f, WHITE, "Release");
	Draw_Text(229, 138, 0.7f, WHITE, "Nightly");

	// Draw The Pages and Back Icon.
	Draw_Text(140, 4, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, WHITE, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 296, 3, RED);
	drawUNIVSelection();
}

void Univ::univSelectionLogic(u32 hDown) {
	if(hDown & KEY_UP) {
		if(univSelection > 0)	univSelection--;
	} else if(hDown & KEY_DOWN) {
		if(univSelection < 5)	univSelection++;
		} else if(hDown & KEY_A) {
			switch(univSelection) {
				case 0: {
					if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Release?")) {
					updatingSelf = true;
					updateUniversalManager(false);
					updatingSelf = false;
					}
					break;
				} case 1:
					if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Nightly?")) {
					updatingSelf = true;
					updateUniversalManager(true);
					updatingSelf = false;
					}
					break;
				  case 2: {
					if(confirmPopup("Are you sure you want to update PKMN-Chest\nTo Release?")) {
					updatePKMNChestRelease(); 
					}
					break;
				} case 3: {
					if(confirmPopup("Are you sure you want to update PKMN-Chest\nTo Nightly?")) {
					updatePKMNChestNightly(); 
					}
					break;
				} case 4: {
					if(confirmPopup("Are you sure you want to update Relaunch\nTo Release?")) {
					updateRelaunchRelease(); 
					}
					break;
				} case 5:
					if(confirmPopup("Are you sure you want to update Relaunch\nTo Nightly?")) {
					updateRelaunchNightly(); 
					}
					break;
			}
		}
}

void Univ::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	univSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_L) {
		Gui::setScreen(std::make_unique<Other>());
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE052 / \uE053 to switch Pages.");
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, UNIVFunction[0])) {
			if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Release?")) {
			updatingSelf = true;
			updateUniversalManager(false);
			updatingSelf = false;
			}
		} else if (touching(touch, UNIVFunction[1])) {
			if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Nightly?")) {
			updatingSelf = true;
			updateUniversalManager(true);
			updatingSelf = false;
			}
		} else if (touching(touch, UNIVFunction[6])) {
			Gui::setScreen(std::make_unique<UpdaterSub>());
		} else if (touching(touch, UNIVFunction[2])) {
			if(confirmPopup("Are you sure you want to update PKMN-Chest\nTo Release?")) {
			updatePKMNChestRelease(); 
			}
		} else if (touching(touch, UNIVFunction[3])) {
			if(confirmPopup("Are you sure you want to update PKMN-Chest\nTo Nightly?")) {
			updatePKMNChestNightly(); 
			}
			} else if (touching(touch, UNIVFunction[4])) {
			if(confirmPopup("Are you sure you want to update Relaunch\nTo Release?")) {
			updateRelaunchRelease(); 
			}
		} else if (touching(touch, UNIVFunction[5])) {
			if(confirmPopup("Are you sure you want to update Relaunch\nTo Nightly?")) {
			updateRelaunchNightly(); 
			}
}
}
}