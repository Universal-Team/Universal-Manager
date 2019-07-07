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
#include "download/download.hpp"
#include <algorithm>
#include <unistd.h>
#include "settings.hpp"

bool updatingSelf = false;

struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

//#################################################
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

ButtonPos mainFunction[] = {
	{38, 48, 87, 33, -1}, // CFW Screen
	{129, 48, 87, 33, -1}, // TWL Screen
	{220, 48, 87, 33, -1}, // Other Screen
	{38, 88, 87, 33, -1}, // Universal Screen
	{293, 213, 27, 27, -1}, // Back Button.
};

//###################################################
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

ButtonPos CFWFunction[] = {
	{129, 48, 87, 33, -1}, // Luma3DS Release
	{220, 48, 87, 33, -1}, // Luma3DS Nightly
	{129, 88, 87, 33, -1}, // GodMode9 Release
	{293, 213, 27, 27, -1}, // Back Button.
};
//############################################################
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

ButtonPos TWLFunction[] = {
	{129, 48, 87, 33, -1}, // TWLMENU Release
	{220, 48, 87, 33, -1}, // TWLMENU Nightly
	{129, 88, 87, 33, -1}, // NDSB Release
	{220, 88, 87, 33, -1}, // NDSB Nightly
	{129, 128, 87, 33, -1}, // usrcheat.dat
	{293, 213, 27, 27, -1}, // Back Button.
};
//#############################################################
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

ButtonPos OTHERFunction[] = {
	{129, 48, 87, 33, -1}, // Themes
	{220, 48, 87, 33, -1}, // Scripts
	{293, 213, 27, 27, -1}, // Back Button.
};
//###############################################################
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

ButtonPos UNIVFunction[] = {
	{129, 48, 87, 33, -1}, // Univ Manager Release
	{220, 48, 87, 33, -1}, // Univ Manager Nightly
	{129, 88, 87, 33, -1}, // Pokemon Chest Release
	{220, 88, 87, 33, -1}, // Pokemon Chest Nightly
	{129, 128, 87, 33, -1}, // Relaunch Release
	{220, 128, 87, 33, -1}, // Relaunch Nightly
	{293, 213, 27, 27, -1}, // Back Button.
};
//###################################################################

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
	false,
	false,
};

extern bool touching(touchPosition touch, ButtonPos button);


void drawUpdaterSubMenu(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, FONT_SIZE_18, WHITE, "Updater Sub Menu");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	for (int i = (int)(sizeof(mainButtons)/sizeof(mainButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(mainButtonsTex[i], mainButtons[i].x, mainButtons[i].y);
	}
	Draw_Text(49, 58, 0.7f, WHITE, "CFW");
	Draw_Text(140, 58, 0.7f, WHITE, "TWL");
	Draw_Text(229, 58, 0.7f, WHITE, "Other");
	Draw_Text(49, 98, 0.7f, WHITE, "UNIV");
}

void updaterSubMenuLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = mainScreen;
	} else if (hDown & KEY_X) {
		if(confirmPopup("Do you want to check for Updates?")) {
		DisplayMsg("Checking for Updates...\nPlease wait...");
		checkForUpdates();
		}
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press \uE002 to check for Updates.");
	} else if(hDown & KEY_TOUCH) {
				if (touching(touch, mainFunction[0])) {
			screenMode = CFWScreen;
		} else if (touching(touch, mainFunction[1])) {
			screenMode = TWLScreen;
		} else if (touching(touch, mainFunction[2])) {
			screenMode = OtherScreen;
		} else if (touching(touch, mainFunction[3])) {
			screenMode = UniversalScreen;
		} else if(touching(touch, mainFunction[4])) {
			screenMode = mainScreen;
		}
	}
}

void drawUpdaterTWL(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, FONT_SIZE_18, WHITE, "TWL Updater Screen");
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// TWL Menu Buttons.
	for (int i = (int)(sizeof(TWLButtons)/sizeof(TWLButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(TWLButtonsTex[i], TWLButtons[i].x, TWLButtons[i].y);
	}
	Gui::sprite(sprites_TitleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "TWLMENU++");
	Draw_Text(140, 58, 0.7f, WHITE, "Release");
	Draw_Text(229, 58, 0.7f, WHITE, "Nightly");

	// NDS-Bootstrap Buttons.
	Gui::sprite(sprites_TitleButton_idx, 0, 88);
	Draw_Text(0, 98, 0.60f, WHITE, "NDS-Bootstrap");
	Draw_Text(140, 98, 0.7f, WHITE, "Release");
	Draw_Text(229, 98, 0.7f, WHITE, "Nightly");

	Gui::sprite(sprites_TitleButton_idx, 0, 128);
	Draw_Text(0, 138, 0.7f, WHITE, "Extras");
	Draw_Text(140, 138, 0.7f, WHITE, "Cheats");

	Draw_Text(140, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, WHITE, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, BLACK, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 2, RED);

	for(int i=0;i<(sizeof(TWLButtons)/sizeof(TWLButtons[0]));i++) {
    if(updateAvailable[i]) {
        Gui::sprite(sprites_dot_idx, TWLButtons[i].x+75, TWLButtons[i].y-6);
    }
}
}

void updaterTWLLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_L) {
		screenMode = CFWScreen;
	} else if (hDown & KEY_R) {
		screenMode = OtherScreen;
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press L / R to switch Pages.");
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
			screenMode = updaterSubMenu;
		}
	}
}

void drawUpdaterOther(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, FONT_SIZE_18, WHITE, "Other Updater Screen");
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Themes Download Button.
	for (int i = (int)(sizeof(OTHERButtons)/sizeof(OTHERButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(OTHERButtonsTex[i], OTHERButtons[i].x, OTHERButtons[i].y);
	}

	Gui::sprite(sprites_TitleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "Extras");
	Draw_Text(140, 58, 0.7f, WHITE, "Themes");
	Draw_Text(229, 58, 0.7f, WHITE, "Scripts");

	

	Draw_Text(140, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, WHITE, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, BLACK, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 2, RED);
}

void updaterOtherLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_L) {
		screenMode = TWLScreen;
	} else if (hDown & KEY_R) {
		screenMode = UniversalScreen;
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press L / R to switch Pages.");
	} else if (hDown & KEY_TOUCH) {
			if (touching(touch, OTHERFunction[0])) {
			downloadThemes();
		} else if (touching(touch, OTHERFunction[1])) {
			downloadScripts();
		} else if(touching(touch, OTHERFunction[2])) {
			screenMode = updaterSubMenu;
		}
}
}

void drawUpdaterCFW(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, FONT_SIZE_18, WHITE, "CFW Updater Screen");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Luma 3DS Buttons.
	for (int i = (int)(sizeof(CFWButtons)/sizeof(CFWButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(CFWButtonsTex[i], CFWButtons[i].x, CFWButtons[i].y);
	}

	Gui::sprite(sprites_TitleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "Luma3DS");
	Draw_Text(140, 58, 0.7f, WHITE, "Release");
	Draw_Text(229, 58, 0.7f, WHITE, "Nightly");

	 // GodMode9 Buttons.
	Gui::sprite(sprites_TitleButton_idx, 0, 88);
	Draw_Text(0, 98, 0.65f, WHITE, "GodMode9");
	Draw_Text(140, 98, 0.7f, WHITE, "Release");

	// Draw The Pages and Back Icon.
	Draw_Text(140, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, WHITE, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, BLACK, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 236, 2, RED);

	for(int i=0;i<(sizeof(CFWButtons)/sizeof(CFWButtons[0]));i++) {
    if(updateAvailable[i+4]) {
        Gui::sprite(sprites_dot_idx, CFWButtons[i].x+75, CFWButtons[i].y-6);
    }
}
}

void updaterCFWLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_R) {
		screenMode = TWLScreen;
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press L / R to switch Pages.");
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
			screenMode = updaterSubMenu;
		}
		}

void drawUniversalScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, FONT_SIZE_18, WHITE, "Universal Updater Screen");
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Universal-Manager Buttons.

	for (int i = (int)(sizeof(UNIVButtons)/sizeof(UNIVButtons[0]))-1; i >= 0; i--) {
		Gui::sprite(UNIVButtonsTex[i], UNIVButtons[i].x, UNIVButtons[i].y);
	}

	Gui::sprite(sprites_TitleButton_idx, 0, 48);
	Draw_Text(0, 58, 0.65f, WHITE, "UNIV-Manager");
	Draw_Text(140, 58, 0.7f, WHITE, "Release");
	Draw_Text(229, 58, 0.7f, WHITE, "Nightly");

	// PKMN-Chest.
	Gui::sprite(sprites_TitleButton_idx, 0, 88);
	Draw_Text(0, 98, 0.65f, WHITE, "PKMN-Chest");
	Draw_Text(140, 98, 0.7f, WHITE, "Release");
	Draw_Text(229, 98, 0.7f, WHITE, "Nightly");

	Gui::sprite(sprites_TitleButton_idx, 0, 128);
	Draw_Text(0, 138, 0.65f, WHITE, "RELAUNCH");
	Draw_Text(140, 138, 0.7f, WHITE, "Release");
	Draw_Text(229, 138, 0.7f, WHITE, "Nightly");

	// Draw The Pages and Back Icon.
	Draw_Text(140, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Draw_Text(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	Draw_Text(300, 4, 0.50, WHITE, "4"); //Draw Fourth Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 296, 2, RED);

	for(int i=0;i<(sizeof(UNIVButtons)/sizeof(UNIVButtons[0]));i++) {
    if(updateAvailable[i]) {
        Gui::sprite(sprites_dot_idx, UNIVButtons[i].x+75, UNIVButtons[i].y-6);
    }
}
}

void UniversalLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = updaterSubMenu;
	} else if (hDown & KEY_L) {
		screenMode = OtherScreen;
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press L / R to switch Pages.");
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
			screenMode = updaterSubMenu;
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