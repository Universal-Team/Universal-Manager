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

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};
extern bool touching(touchPosition touch, ButtonPos button);

ButtonPos mainScreenButtonPos[] = {
    {0, 25, 149, 52, -1},
    {170, 25, 149, 52, -1},
	{0, 90, 149, 52, -1},
	{170, 90, 149, 52, -1},
	{0, 150, 149, 52, -1},
    {170, 150, 149, 52, -1},

    {0, 25, 149, 52, -1},
	{170, 25, 149, 52, -1},
	{0, 90, 149, 52, -1},
	{170, 90, 149, 52, -1},
	{0, 150, 149, 52, -1},
};

int mainMenuPage = 0;
static int mainSelection1 = 0;
static int mainSelection2 = 0;
extern int fadealpha;
extern bool fadein;

// Version numbers.
char universal_manager_vertext[13];

static void drawMainMenuSelection1(void) {
	if (mainSelection1 == 0) {
		Gui::Button(button_tl_selector_idx, 0, 26);
	} else if (mainSelection1 == 1) {
		Gui::Button(button_selector_idx, 166, 31);
	} else if (mainSelection1 == 2) {
		Gui::Button(button_selector_idx, 3, 95);
	} else if (mainSelection1 == 3) {
		Gui::Button(button_selector_idx, 166, 95);
	} else if (mainSelection1 == 4) {
		Gui::Button(button_selector_idx, 3, 158);
	} else if (mainSelection1 == 5) {
		Gui::Button(button_br_selector_idx, 161, 153);
	}
}

static void drawMainMenuSelection2(void) {
	if (mainSelection2 == 0) {
		Gui::Button(button_tl_selector_idx, 0, 26);
	} else if (mainSelection2 == 1) {
		Gui::Button(button_selector_idx, 166, 31);
	} else if (mainSelection2 == 2) {
		Gui::Button(button_selector_idx, 3, 95);
	} else if (mainSelection2 == 3) {
		Gui::Button(button_selector_idx, 166, 95);
	} else if (mainSelection2 == 4) {
		Gui::Button(button_selector_idx, 3, 158);
	}
}

void drawMainMenu(void) {
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Universal-Manager"))/2, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text(340, 218, FONT_SIZE_18, WHITE, universal_manager_vertext);
	if (fadealpha > 0) Draw_Rect(0, 0, 400, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();


	// First Page.
	if (mainMenuPage == 0) {
	Gui::Button(button_button_tl_idx, 1, 27);
	Gui::sprite(sprites_fileManagerIcon_idx, 9, 41);
	Draw_Text(40, 48, 0.65f, WHITE, "Filemanager");

	Gui::Button(button_button_2_idx, 162, 27);
	Gui::sprite(sprites_ftpIcon_idx, 172, 40);
	Draw_Text(230, 42, 0.7f, WHITE, "FTP");

	Gui::Button(button_button_3_idx, 1, 91);
	Gui::sprite(sprites_scriptIcon_idx, 4, 102);
	Draw_Text(40, 107, 0.65f, WHITE, "Scripts");

	Gui::Button(button_button_4_idx, 162, 91);
	Gui::sprite(sprites_music_icon_idx, 175, 106);
	Draw_Text(230, 107, 0.7f, WHITE, "Music");

	Gui::Button(button_button_5_idx, 1, 154);
	Gui::sprite(sprites_updaterIcon_idx, 9, 167);
	Draw_Text(50, 177, 0.7f, WHITE, "Updater");

	Gui::Button(button_button_br_idx, 162, 151);
	Gui::sprite(sprites_settingsIcon_idx, 172, 165);
	Draw_Text(220, 177, 0.7f, WHITE, "Settings");

	Draw_Text(150, 4, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, WHITE, "1"); //Draw First Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 3, RED);
	Draw_Text(280, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	drawMainMenuSelection1();
	if (fadealpha > 0) Draw_Rect(0, 0, 320, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect


	// Second Page.
	} else if (mainMenuPage == 1) {
	Gui::Button(button_button_tl_idx, 1, 27);
	Gui::sprite(sprites_image_icon_idx, 6, 37);
	Draw_Text(40, 42, 0.65f, WHITE, "Image Viewer");

	Gui::Button(button_button_2_idx, 162, 27);
	Draw_Text(230, 42, 0.7f, WHITE, "Credits");

	Gui::Button(button_button_3_idx, 1, 91);
	Gui::sprite(sprites_textEditorIcon_idx, 9, 103);
	Draw_Text(40, 107, 0.65f, WHITE, "Text Editor");

	Gui::Button(button_button_4_idx, 162, 91);
	Gui::sprite(sprites_utilsIcon_idx, 175, 102);
	Draw_Text(215, 107, 0.7f, WHITE, "Utils");

	Gui::Button(button_button_5_idx, 1, 154);
	Draw_Text(50, 177, 0.7f, WHITE, "Games");

	Draw_Text(150, 4, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(280, 4, 0.50, WHITE, "2"); //Draw Second Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 3, RED);
	drawMainMenuSelection2();
}
}

static void mainMenuSelectionLogic1(u32 hDown) {
		if (mainMenuPage == 0 && hDown & KEY_UP) {
			if(mainSelection1 > 0)	mainSelection1--;
		} else if (mainMenuPage == 0 && hDown & KEY_DOWN) {
			if(mainSelection1 < 5)	mainSelection1++;
		} else if (mainMenuPage == 0 && hDown & KEY_A) {
			switch(mainSelection1) {
				case 0: {
					screenTransition(fileManager);
					break;
				} case 1:
					screenTransition(ftpScreen);
					break;
				  case 2: {
					screenTransition(scriptMainScreen);
					break;
				} case 3: {
					screenTransition(musicMainScreen);
					break;
				} case 4: {
					screenTransition(updaterSubMenu);
					break;
				} case 5:
					screenTransition(SettingsScreen);
					break;
			}
		}
}

static void mainMenuSelectionLogic2(u32 hDown) {
		if (mainMenuPage == 1 && hDown & KEY_UP) {
			if(mainSelection2 > 0)	mainSelection2--;
		} else if (mainMenuPage == 1 && hDown & KEY_DOWN) {
			if(mainSelection2 < 4)	mainSelection2++;
		} else if (mainMenuPage == 1 && hDown & KEY_A) {
			switch(mainSelection2) {
				case 0: {
					screenTransition(ImageSelectorScreen);
					break;
				} case 1:
					screenTransition(creditsScreen);
					break;
				  case 2: {
					screenTransition(textFileBrowse);
					break;
				} case 3: {
					screenTransition(utilsScreen);
					break;
				} case 4: {
					screenTransition(gameSubMenuScreen);
					break;
				}
			}
		}
}

void MainMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	mainMenuSelectionLogic1(hDown);
	mainMenuSelectionLogic2(hDown);
	if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE052 / \uE053 to switch Pages.");

		// Page Switching.
	} else if (mainMenuPage == 0 && hDown & KEY_R) {
		mainMenuPage = 1;
		} else if (mainMenuPage == 1 && hDown & KEY_L) {
		mainMenuPage = 0;

		// First Page.
		} else if (mainMenuPage == 0 && hDown & KEY_TOUCH) {
			if (touching(touch, mainScreenButtonPos[0])) {
				screenTransition(fileManager);
			} else if (touching(touch, mainScreenButtonPos[1])) {
				screenTransition(ftpScreen);
			} else if (touching(touch, mainScreenButtonPos[2])) {
				screenTransition(scriptMainScreen);
			} else if (touching(touch, mainScreenButtonPos[3])) {
				screenTransition(musicMainScreen);
			} else if (touching(touch, mainScreenButtonPos[4])) {
				screenTransition(updaterSubMenu);
			} else if (touching(touch, mainScreenButtonPos[5])) {
				screenTransition(SettingsScreen);
			}

			// Second Page.
		} else if (mainMenuPage == 1 && hDown & KEY_TOUCH) {
			if (touching(touch, mainScreenButtonPos[6])) {
				screenTransition(ImageSelectorScreen);
			} else if (touching(touch, mainScreenButtonPos[7])) {
				screenTransition(creditsScreen);
			} else if (touching(touch, mainScreenButtonPos[8])) {
				screenTransition(textFileBrowse);
			} else if (touching(touch, mainScreenButtonPos[9])) {
				screenTransition(utilsScreen);
			} else if (touching(touch, mainScreenButtonPos[10])) {
				screenTransition(gameSubMenuScreen);
		}
	}
}