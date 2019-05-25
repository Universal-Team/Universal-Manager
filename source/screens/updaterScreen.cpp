/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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
#include "download/download.hpp"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

bool updatingSelf = false;

struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

extern bool touching(touchPosition touch, ButtonPos button);


ButtonPos downloadFunctionButtonPos[] = {
	// TWLMenu
	{129, 48, 87, 33, -1},
	{220, 48, 87, 33, -1},
	// NDS-Bootstrap
	{129, 88, 87, 33, -1},
	{220, 88, 87, 33, -1},
	// Universal-Manager
	{129, 128, 87, 33, -1},
	{220, 128, 87, 33, -1},
	// Luma 3DS.
	{129, 168, 87, 33, -1},
	{220, 168, 87, 33, -1},
	// GodMode9
	{129, 48, 87, 33, -1},
	// PKSM
	{129, 88, 87, 33, -1},
	// Checkpoint
	{129, 128, 87, 33, -1},
	// Check for Update!
	{0, 208, 32, 32, -1},
};

ButtonPos downloadButtonPos[] = {
	// TWLMenu
	{129, 48, 87, 33},
	{220, 48, 87, 33},
	// NDS-Bootstrap
	{129, 88, 87, 33},
	{220, 88, 87, 33},
	// Universal-Manager
	{129, 128, 87, 33},
	{220, 128, 87, 33},
	// Luma 3DS
	{129, 168, 87, 33},
	{220, 168, 87, 33},

	// GodMode9
	{129, 48, 87, 33},
	// PKSM
	{129, 88, 87, 33},
	// Checkpoint
	{129, 128, 87, 33},
};

bool updateAvailable[] = {
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
};


void drawUpdaterScreen(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Updater Screen");
	displayTime();
	volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE001 : Back   \ue005 : Go to Page 2; Touch : Select an Option.");
	
	// Draw the Main Bottom Screen Background.
	drawBgBot();
	drawBarsBot();

	// TWL Menu Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[0].x, downloadButtonPos[0].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[1].x, downloadButtonPos[1].y);
	volt_draw_text(0, 58, BIG_SIZE, BIG_SIZE, BLACK, "TWLMENU++");
	volt_draw_text(140, 58, 0.7f, 0.7f, BLACK, "Release");
	volt_draw_text(229, 58, 0.7f, 0.7f, BLACK, "Nightly");

	// NDS-Bootstrap Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[2].x, downloadButtonPos[2].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[3].x, downloadButtonPos[3].y);
	volt_draw_text(0, 98, 0.7f, 0.7f, BLACK, "NDS-Bootstrap");
	volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, "Release");
	volt_draw_text(229, 98, 0.7f, 0.7f, BLACK, "Nightly");

// Universal-Manager Buttons.

	volt_draw_texture(UpdaterButton, downloadButtonPos[4].x, downloadButtonPos[4].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[5].x, downloadButtonPos[5].y);
	volt_draw_text(0, 138, 0.7f, 0.7f, BLACK, "UNIV-Manager");
	volt_draw_text(140, 138, 0.7f, 0.7f, BLACK, "Release");
	volt_draw_text(229, 138, 0.7f, 0.7f, BLACK, "Nightly");

	// Luma 3DS Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[6].x, downloadButtonPos[6].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[7].x, downloadButtonPos[7].y);
	volt_draw_text(0, 178, 0.7f, 0.7f, BLACK, "Luma3DS");
	volt_draw_text(140, 178, 0.7f, 0.7f, BLACK, "Release");
	volt_draw_text(229, 178, 0.7f, 0.7f, BLACK, "Nightly");
	// Draw The Pages and Back Icon.
	volt_draw_text(170, 4, 0.50, 0.50, WHITE, "Current Page:");
	volt_draw_text(260, 4, 0.50, 0.50, WHITE, "1"); //Draw First Page Number.
	volt_draw_text(270, 4, 0.50, 0.50, BLACK, "2"); //Draw Second Page Number.
	volt_draw_texture(BackIcon, 288, 208);
	volt_draw_texture(UpdaterIcon, 0, 208);
	for (int i = (int)(sizeof(downloadButtonPos)/sizeof(downloadButtonPos[7]))-1; i >= 0; i--) {
		if(updateAvailable[i]) {
			volt_draw_texture(Dot, downloadButtonPos[i].x+75, downloadButtonPos[i].y-6);
		}
	}
	volt_end_draw();
}

void updaterLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = mainScreen;
	} else if (hDown & KEY_R) {
		screenMode = updaterScreen2;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, downloadFunctionButtonPos[0])) {
			if(confirmPopup("Are you sure you want to update TWiLightMenu\nTo Release?")) {
			updateTWiLight(false);
			}
		} else if (touching(touch, downloadFunctionButtonPos[1])) {
			if(confirmPopup("Are you sure you want to update TWiLightMenu\nTo Nightly?")) {
			updateTWiLight(true);
			}
		} else if (touching(touch, downloadFunctionButtonPos[2])) {
			if(confirmPopup("Are you sure you want to update NDS-Bootstrap\nTo Release?")) {
			updateBootstrap(false);
			}
		} else if (touching(touch, downloadFunctionButtonPos[3])) {
			if(confirmPopup("Are you sure you want to update NDS-Bootstrap\nTo Nightly?")) {
			updateBootstrap(true);
			}
		} else if (touching(touch, downloadFunctionButtonPos[4])) {
			if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Release?")) {
			updateUniversalManager(false);
			}
		} else if (touching(touch, downloadFunctionButtonPos[5])) {
			if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Nightly?")) {
			updateUniversalManager(true);
			}
		} else if (touching(touch, downloadFunctionButtonPos[6])) {
			if(confirmPopup("Are you sure you want to update Luma3DS\nTo Release?")) {
			updateLuma(false);
			}
		} else if (touching(touch, downloadFunctionButtonPos[7])) {
			if(confirmPopup("Are you sure you want to update Luma3DS\nTo Nightly?")) {
			updateLuma(true);
			}
		} else if (touching(touch, downloadFunctionButtonPos[11])) {
			if(confirmPopup("Are you sure you want to check for Updates?\nThis only works the First Time.")) {
			displayMsg("Checking for Updates.. please wait.");
			checkForUpdates();
			}
		}
	}
}

void drawUpdaterScreen2(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Updater Screen");
	displayTime();
	volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\ue004 : Go to Page 1; Touch : Select an Option.");
	
	// Draw the Main Bottom Screen Background.
	drawBgBot();
	drawBarsBot();

	volt_draw_texture(UpdaterButton, downloadButtonPos[8].x, downloadButtonPos[8].y);
	 // GodMode9 Buttons.
	volt_draw_text(0, 58, 0.7f, 0.7f, BLACK, "GodMode9");
	volt_draw_text(140, 58, 0.7f, 0.7f, BLACK, "Release");

	volt_draw_texture(UpdaterButton, downloadButtonPos[9].x, downloadButtonPos[9].y);
	// PKSM Buttons.
	volt_draw_text(0, 98, 0.7f, 0.7f, BLACK, "PKSM");
	volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, "Release");
	// Checkpoint Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[10].x, downloadButtonPos[10].y);
	volt_draw_text(0, 138, 0.7f, 0.7f, BLACK, "Checkpoint");
	volt_draw_text(140, 138, 0.7f, 0.7f, BLACK, "Release");
	// Draw The Pages and Back Icon.
	volt_draw_text(170, 4, 0.50, 0.50, WHITE, "Current Page:");
	volt_draw_text(260, 4, 0.50, 0.50, BLACK, "1"); //Draw First Page Number.
	volt_draw_text(270, 4, 0.50, 0.50, WHITE, "2"); //Draw Second Page Number.
	for (int i = (int)(sizeof(downloadButtonPos)/sizeof(downloadButtonPos[10]))-1; i >= 8; i--) {
		if(updateAvailable[i]) {
			volt_draw_texture(Dot, downloadButtonPos[i].x+75, downloadButtonPos[i].y-6); // Needs to be fixed later.
		}
	}
	volt_end_draw();
}

void updaterLogic2(u32 hDown, touchPosition touch) {
	if (hDown & KEY_L) {
		screenMode = updaterScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, downloadFunctionButtonPos[8])) {
			if(confirmPopup("Are you sure you want to update GodMode9\nRelease?")) {
			downloadGodMode9();
			}
		} else if (touching(touch, downloadFunctionButtonPos[9])) {
			if(confirmPopup("Are you sure you want to update PKSM\nRelease?")) {
			updatePKSM();
			}
		} else if (touching(touch, downloadFunctionButtonPos[10])) {
			if(confirmPopup("Are you sure you want to update Checkpoint\nRelease?")) {
			updateCheckpoint();
			}
}
}
}