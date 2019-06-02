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
	{129, 48, 87, 33, -1},
	{220, 48, 87, 33, -1},
	// Luma 3DS.
	{129, 48, 87, 33, -1},
	{220, 48, 87, 33, -1},
	// GodMode9
	{129, 88, 87, 33, -1},
	// PKSM
	{129, 88, 87, 33, -1},
	// Checkpoint
	{129, 128, 87, 33, -1},
	// Check for Update!
	{0, 208, 32, 32, -1},
	// Back Icon.
	{293, 213, 27, 27, -1},

	{129, 168, 87, 33, -1},
};

ButtonPos downloadButtonPos[] = {
	// TWLMenu
	{129, 48, 87, 33},
	{220, 48, 87, 33},
	// NDS-Bootstrap
	{129, 88, 87, 33},
	{220, 88, 87, 33},
	// Universal-Manager
	{129, 48, 87, 33},
	{220, 48, 87, 33},
	// Luma 3DS
	{129, 48, 87, 33},
	{220, 48, 87, 33},

	// GodMode9
	{129, 88, 87, 33},
	// PKSM
	{129, 88, 87, 33},
	// Checkpoint
	{129, 128, 87, 33},
	// Music Player Theme.
	{129, 168, 87, 33},

	// Sub Menu.
	{38, 48, 87, 33, CFWScreen},
	{129, 48, 87, 33, TWLScreen},
	{220, 48, 87, 33, OtherScreen},

	{129, 168, 87, 33},
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


void drawUpdaterSubMenu(void) {
	drawBgTop();
	drawBarsTop();
	displayTime();
	volt_draw_text_center(GFX_TOP, 4, 0.72f, 0.72f, WHITE, "Updater Sub Menu");

	drawBgBot();
	drawBarsBot();

	volt_draw_texture(UpdaterButton, downloadButtonPos[12].x, downloadButtonPos[12].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[13].x, downloadButtonPos[13].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[14].x, downloadButtonPos[14].y);
	volt_draw_text(49, 58, 0.7f, 0.7f, BLACK, "CFW");
	volt_draw_text(140, 58, 0.7f, 0.7f, BLACK, "TWL");
	volt_draw_text(229, 58, 0.7f, 0.7f, BLACK, "Other");
	//volt_draw_texture(UpdaterIcon, 0, 208); // Not working Yet correctly.
	volt_end_draw();
}

void updaterSubMenuLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = mainScreen;
	} else if(hDown & KEY_TOUCH) {
		for(uint i=0;i<(sizeof(downloadButtonPos)/sizeof(downloadButtonPos[0]));i++) {
			if (touching(touch, downloadButtonPos[i])) {
				screenMode = downloadButtonPos[i].link;
			}
		}
/*	} else if (touching(touch, downloadFunctionButtonPos[11])) {
			if(confirmPopup("Are you sure you want to check for Updates?\nThis only works the First Time.")) {
			displayMsg("Checking for Updates.. please wait.");
			checkForUpdates();
			}
		}
*/} else if (touching(touch, downloadFunctionButtonPos[12])) {
			screenMode = mainScreen;
		}
}

void drawUpdaterTWL(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text_center(GFX_TOP, 4, 0.72f, 0.72f, WHITE, "TWL Update Screen");
	displayTime();
	
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

	// Draw The Pages and Back Icon.
	volt_draw_text(170, 4, 0.50, 0.50, WHITE, "Current Page:");
	volt_draw_text(260, 4, 0.50, 0.50, BLACK, "1"); //Draw First Page Number.
	volt_draw_text(270, 4, 0.50, 0.50, WHITE, "2"); //Draw Second Page Number.
	volt_draw_text(280, 4, 0.50, 0.50, BLACK, "3"); //Draw Third Page Number.
	/*for (int i = (int)(sizeof(downloadButtonPos)/sizeof(downloadButtonPos[7]))-1; i >= 0; i--) {
		if(updateAvailable[i]) {
			volt_draw_texture(Dot, downloadButtonPos[i].x+75, downloadButtonPos[i].y-6);
		}
	}
	*/volt_end_draw();
}

void updaterTWLLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_L) {
		screenMode = CFWScreen;
	} else if (hDown & KEY_R) {
		screenMode = OtherScreen;
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
		} else if (touching(touch, downloadFunctionButtonPos[12])) {
			screenMode = updaterSubMenu;
		}
	}
}
	/*	
		} else if (touching(touch, downloadFunctionButtonPos[11])) {
			if(confirmPopup("Are you sure you want to check for Updates?\nThis only works the First Time.")) {
			displayMsg("Checking for Updates.. please wait.");
			checkForUpdates();
			}
		}
	}
}

*/void drawUpdaterOther(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text_center(GFX_TOP, 4, 0.72f, 0.72f, WHITE, "Other Updater Screen");
	displayTime();
	
	// Draw the Main Bottom Screen Background.
	drawBgBot();
	drawBarsBot();

		// Universal-Manager Buttons.

	volt_draw_texture(UpdaterButton, downloadButtonPos[4].x, downloadButtonPos[4].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[5].x, downloadButtonPos[5].y);
	volt_draw_text(0, 58, BIG_SIZE, BIG_SIZE, BLACK, "UNIV-Manager");
	volt_draw_text(140, 58, 0.7f, 0.7f, BLACK, "Release");
	volt_draw_text(229, 58, 0.7f, 0.7f, BLACK, "Nightly");

	volt_draw_texture(UpdaterButton, downloadButtonPos[9].x, downloadButtonPos[9].y);
	// PKSM Buttons.
	volt_draw_text(0, 98, 0.7f, 0.7f, BLACK, "PKSM");
	volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, "Release");
	// Checkpoint Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[10].x, downloadButtonPos[10].y);
	volt_draw_text(0, 138, 0.7f, 0.7f, BLACK, "Checkpoint");
	volt_draw_text(140, 138, 0.7f, 0.7f, BLACK, "Release");

	// Themes Download Button.
	volt_draw_texture(UpdaterButton, downloadButtonPos[15].x, downloadButtonPos[15].y);
	volt_draw_text(0, 178, 0.7f, 0.7f, BLACK, "extras");
	volt_draw_text(140, 178, 0.7f, 0.7f, BLACK, "Themes");
	

	// Draw The Pages and Back Icon.
	volt_draw_text(170, 4, 0.50, 0.50, WHITE, "Current Page:");
	volt_draw_text(260, 4, 0.50, 0.50, BLACK, "1"); //Draw First Page Number.
	volt_draw_text(270, 4, 0.50, 0.50, BLACK, "2"); //Draw Second Page Number.
	volt_draw_text(280, 4, 0.50, 0.50, WHITE, "3"); //Draw Third Page Number.
	/*for (int i = (int)(sizeof(downloadButtonPos)/sizeof(downloadButtonPos[10]))-1; i >= 8; i--) {
		if(updateAvailable[i]) {
			volt_draw_texture(Dot, downloadButtonPos[i].x+75, downloadButtonPos[i].y-6); // Needs to be fixed later.
		}
	}
	*/volt_end_draw();
}

void updaterOtherLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_L) {
		screenMode = TWLScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, downloadFunctionButtonPos[4])) {
			if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Release?")) {
			updateUniversalManager(false);
			}
		} else if (touching(touch, downloadFunctionButtonPos[5])) {
			if(confirmPopup("Are you sure you want to update Universal-Manager\nTo Nightly?")) {
			updateUniversalManager(true);
			}
		} else if (touching(touch, downloadFunctionButtonPos[9])) {
			if(confirmPopup("Are you sure you want to update PKSM\nRelease?")) {
			updatePKSM();
			}
		} else if (touching(touch, downloadFunctionButtonPos[10])) {
			if(confirmPopup("Are you sure you want to update Checkpoint\nRelease?")) {
			updateCheckpoint();
			}
		} else if (touching(touch, downloadFunctionButtonPos[12])) {
			screenMode = updaterSubMenu;
		} else if (touching(touch, downloadFunctionButtonPos[13])) {
			downloadThemes();
		}
}
}

void drawUpdaterCFW(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text_center(GFX_TOP, 4, 0.72f, 0.72f, WHITE, "CFW Update Screen");
	displayTime();
	
	// Draw the Main Bottom Screen Background.
	drawBgBot();
	drawBarsBot();

	// Luma 3DS Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[6].x, downloadButtonPos[6].y);
	volt_draw_texture(UpdaterButton, downloadButtonPos[7].x, downloadButtonPos[7].y);
	volt_draw_text(0, 58, 0.7f, 0.7f, BLACK, "Luma3DS");
	volt_draw_text(140, 58, 0.7f, 0.7f, BLACK, "Release");
	volt_draw_text(229, 58, 0.7f, 0.7f, BLACK, "Nightly");

	 // GodMode9 Buttons.
	volt_draw_texture(UpdaterButton, downloadButtonPos[8].x, downloadButtonPos[8].y);
	volt_draw_text(0, 98, 0.7f, 0.7f, BLACK, "GodMode9");
	volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, "Release");

	// Draw The Pages and Back Icon.
	volt_draw_text(170, 4, 0.50, 0.50, WHITE, "Current Page:");
	volt_draw_text(260, 4, 0.50, 0.50, WHITE, "1"); //Draw First Page Number.
	volt_draw_text(270, 4, 0.50, 0.50, BLACK, "2"); //Draw Second Page Number.
	volt_draw_text(280, 4, 0.50, 0.50, BLACK, "3"); //Draw Third Page Number.
	/*for (int i = (int)(sizeof(downloadButtonPos)/sizeof(downloadButtonPos[7]))-1; i >= 0; i--) {
		if(updateAvailable[i]) {
			volt_draw_texture(Dot, downloadButtonPos[i].x+75, downloadButtonPos[i].y-6);
		}
	}
	*/volt_end_draw();
}

void updaterCFWLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_R) {
		screenMode = TWLScreen;
	} else if (touching(touch, downloadFunctionButtonPos[6])) {
			if(confirmPopup("Are you sure you want to update Luma3DS\nTo Release?")) {
			updateLuma(false);
			}
		} else if (touching(touch, downloadFunctionButtonPos[7])) {
			if(confirmPopup("Are you sure you want to update Luma3DS\nTo Nightly?")) {
			updateLuma(true);
			}
		} else if (touching(touch, downloadFunctionButtonPos[8])) {
			if(confirmPopup("Are you sure you want to update GodMode9\nRelease?")) {
			downloadGodMode9();
			}
		} else if (touching(touch, downloadFunctionButtonPos[12])) {
			screenMode = updaterSubMenu;
		}
		}