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

int subSelection = 0;
int cfwSelection = 0;
int twlSelection = 0;
int othSelection = 0;
int univSelection = 0;

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

void drawSubSelection(void) {
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

void drawCFWSelection(void) {
	if (cfwSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 30, Config::barColor);
	} else if (cfwSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 30, Config::barColor);
	} else if (cfwSelection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 70, Config::barColor);
	}
}

void drawTWLSelection(void) {
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

void drawOTHERSelection(void) {
	if (othSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 191, 30, Config::barColor);
	} else if (othSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 282, 30, Config::barColor);
	}
}

void drawUNIVSelection(void) {
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

void UPDATERSUB::Draw(void) const
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

void UPDATERSUB::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(hDown & KEY_UP) {
		if(subSelection > 0)	subSelection--;
	} else if(hDown & KEY_DOWN) {
		if(subSelection < 3)	subSelection++;
		} else if(hDown & KEY_A) {
			switch(subSelection) {
				case 0: {
					Gui::setScreen(std::make_unique<CFWSCREEN>());
					break;
				} case 1:
					Gui::setScreen(std::make_unique<TWLSCREEN>());
					break;
				  case 2: {
					Gui::setScreen(std::make_unique<OTHERSCREEN>());
					break;
				} case 3: {
					Gui::setScreen(std::make_unique<UNIVSCREEN>());
					break;
				}
			}
	} else if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<MAINMENU>());
	} else if (hDown & KEY_X) {
		if(confirmPopup("Do you want to check for Updates?")) {
		DisplayMsg("Checking for Updates...\nPlease wait...");
		checkForUpdates();
		}

	} else if(hDown & KEY_TOUCH) {
				if (touching(touch, mainFunction[0])) {
			Gui::setScreen(std::make_unique<CFWSCREEN>());
		} else if (touching(touch, mainFunction[1])) {
			Gui::setScreen(std::make_unique<TWLSCREEN>());
		} else if (touching(touch, mainFunction[2])) {
			Gui::setScreen(std::make_unique<OTHERSCREEN>());
		} else if (touching(touch, mainFunction[3])) {
			Gui::setScreen(std::make_unique<UNIVSCREEN>());
		} else if(touching(touch, mainFunction[4])) {
			Gui::screenBack();
			return;
		}
	}
}

void TWLSCREEN::Draw(void) const
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

	for(uint i=0;i<(sizeof(TWLButtons)/sizeof(TWLButtons[0]));i++) {
    if(updateAvailable[i]) {
        Gui::sprite(sprites_dot_idx, TWLButtons[i].x+75, TWLButtons[i].y-6);
    }
}
	drawTWLSelection();
}

void twlSelectionLogic(u32 hDown) {
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


void TWLSCREEN::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	twlSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<UPDATERSUB>());
	} else if (hDown & KEY_L) {
		Gui::setScreen(std::make_unique<CFWSCREEN>());
	} else if (hDown & KEY_R) {
		Gui::setScreen(std::make_unique<OTHERSCREEN>());
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

void OTHERSCREEN::Draw(void) const
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

void otherSelectionLogic(u32 hDown) {
	if(hDown & KEY_UP) {
		if(othSelection > 0)	othSelection--;
	} else if(hDown & KEY_DOWN) {
		if(othSelection < 1)	othSelection++;
		} else if(hDown & KEY_A) {
			switch(othSelection) {
				case 0: {
					downloadThemes();
					break;
				} case 1:
					downloadScripts();
					break;
			}
		}
}

void OTHERSCREEN::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	otherSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::setScreen(std::make_unique<UPDATERSUB>());
	} else if (hDown & KEY_L) {
		Gui::setScreen(std::make_unique<TWLSCREEN>());
	} else if (hDown & KEY_R) {
		Gui::setScreen(std::make_unique<UNIVSCREEN>());
	} else if (hDown & KEY_TOUCH) {
			if (touching(touch, OTHERFunction[0])) {
			downloadThemes();
		} else if (touching(touch, OTHERFunction[1])) {
			downloadScripts();
		} else if(touching(touch, OTHERFunction[2])) {
			Gui::setScreen(std::make_unique<UPDATERSUB>());
		}
}
}

void CFWSCREEN::Draw(void) const
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

	for(uint i=0;i<(sizeof(CFWButtons)/sizeof(CFWButtons[0]));i++) {
    if(updateAvailable[i+4]) {
        Gui::sprite(sprites_dot_idx, CFWButtons[i].x+75, CFWButtons[i].y-6);
    }
}
	drawCFWSelection();
}

void cfwSelectionLogic(u32 hDown) {
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

void CFWSCREEN::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	cfwSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_R) {
		Gui::setScreen(std::make_unique<TWLSCREEN>());
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
			Gui::setScreen(std::make_unique<UPDATERSUB>());
		}
		}

void UNIVSCREEN::Draw(void) const
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

	for(uint i=0;i<(sizeof(UNIVButtons)/sizeof(UNIVButtons[0]));i++) {
    if(updateAvailable[i]) {
        Gui::sprite(sprites_dot_idx, UNIVButtons[i].x+75, UNIVButtons[i].y-6);
    }
}
	drawUNIVSelection();
}

void univSelectionLogic(u32 hDown) {
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

void UNIVSCREEN::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	univSelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_L) {
		Gui::setScreen(std::make_unique<OTHERSCREEN>());
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
			Gui::setScreen(std::make_unique<UPDATERSUB>());
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