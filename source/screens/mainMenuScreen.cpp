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

ButtonPos mainScreen1ButtonPos[] = {
    {0, 25, 149, 52, fileManager},
    {170, 25, 149, 52, ftpScreen},
	{0, 90, 149, 52, scriptMainScreen},
	{170, 90, 149, 52, musicMainScreen},
	{0, 150, 149, 52, updaterSubMenu},
    {170, 150, 149, 52, uiSettingsScreen},
};

ButtonPos mainScreen2ButtonPos[] = {
    {0, 25, 149, 52, ImageSelectorScreen},
	{170, 25, 149, 52, creditsScreen},
	{0, 90, 149, 52, TextEditorScreen},
	{170, 90, 149, 52, buttonTesterScreen},
};

// Version numbers.
char universal_manager_vertext[13];

void drawMainMenu(void) {
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text(340, 218, FONT_SIZE_18, WHITE, universal_manager_vertext);

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();

	Gui::sprite(sprites_mainMenuButton_idx, 0, 25);
	Gui::sprite(sprites_fileManagerIcon_idx, 5, 35);
	Draw_Text(40, 42, 0.65f, WHITE, "Filemanager");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 25);
	Gui::sprite(sprites_ftpIcon_idx, 175, 35);
	Draw_Text(230, 42, 0.7f, WHITE, "FTP");

	Gui::sprite(sprites_mainMenuButton_idx, 0, 90);
	Gui::sprite(sprites_scriptIcon_idx, 5, 100);
	Draw_Text(40, 107, 0.65f, WHITE, "Scripts");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 90);
	Gui::sprite(sprites_music_icon_idx, 175, 100);
	Draw_Text(230, 107, 0.7f, WHITE, "Music");

	Gui::sprite(sprites_mainMenuButton_idx, 0, 160);
	Gui::sprite(sprites_updaterIcon_idx, 5, 170);
	Draw_Text(50, 177, 0.7f, WHITE, "Updater");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 160);
	Gui::sprite(sprites_settingsIcon_idx, 175, 170);
	Draw_Text(220, 177, 0.7f, WHITE, "Settings");

	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, WHITE, "1"); //Draw First Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 2, RED);
	Draw_Text(280, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
}

void MainMenu1Logic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_R) {
		screenMode = mainScreen2;
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press FileManager, to open the Filemanager File Browse. \n Press FTP to open the FTP Screen. \n Press Scripts to open the Scripts Screen. \n Press Music to open the Music Screen. \n Press Updater to open the Updater Sub Menu. \n Press Settings to open the Settings.\n\n Press \uE053 to switch to the Second Page.");
		} else if (hDown & KEY_TOUCH) {
			for(uint i=0;i<(sizeof(mainScreen1ButtonPos)/sizeof(mainScreen1ButtonPos[0]));i++) {
				if (touching(touch, mainScreen1ButtonPos[i])) {
					screenMode = mainScreen1ButtonPos[i].link;
			}
		}
	}
}

void drawMainMenu2(void) {
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text(340, 218, FONT_SIZE_18, WHITE, universal_manager_vertext);

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();

	Gui::sprite(sprites_mainMenuButton_idx, 0, 25);
	Gui::sprite(sprites_image_icon_idx, 5, 35);
	Draw_Text(40, 42, 0.65f, WHITE, "Image Viewer");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 25);
	Draw_Text(230, 42, 0.7f, WHITE, "Credits");

	Gui::sprite(sprites_mainMenuButton_idx, 0, 90);
	Gui::sprite(sprites_textEditorIcon_idx, 5, 100);
	Draw_Text(40, 107, 0.65f, WHITE, "Text Editor");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 90);
	Draw_Text(185, 107, 0.7f, WHITE, "Button Tester");

	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(280, 4, 0.50, WHITE, "2"); //Draw Second Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 2, RED);
}

void MainMenu2Logic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_L) {
		screenMode = mainScreen;
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press Image Viewer, to open the Image Selector. \n\n Press Credits to see the Credits. \n\n Press Text Editor to open the Text Editor.\n\n Press \uE052 to switch to the First Page.");
		} else if (hDown & KEY_TOUCH) {
			for(uint i=0;i<(sizeof(mainScreen2ButtonPos)/sizeof(mainScreen2ButtonPos[0]));i++) {
				if (touching(touch, mainScreen2ButtonPos[i])) {
					screenMode = mainScreen2ButtonPos[i].link;
			}
		}
	}
}