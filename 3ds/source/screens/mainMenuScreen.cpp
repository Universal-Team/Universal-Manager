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
#include "mainMenuScreen.hpp"
#include "fileManagerScreen.hpp"
#include "utilsScreen.hpp"
#include "gameSubMenu.hpp"
#include "musicMainScreen.hpp"
#include "scriptMainScreen.hpp"
#include "creditsScreen.hpp"
#include "textBrowseScreen.hpp"
#include "imageScreen.hpp"
#include "settingsScreen.hpp"
#include "updaterScreen.hpp"
#include "ftpScreen.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern bool exiting;

MainMenu::MainMenu()
{
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
}

void MainMenu::drawSelection1(void) const
{
	if (Selection1 == 0) {
		Gui::Button(button_tl_selector_idx, 0, 26);
	} else if (Selection1 == 1) {
		Gui::Button(button_selector_idx, 166, 31);
	} else if (Selection1 == 2) {
		Gui::Button(button_selector_idx, 3, 95);
	} else if (Selection1 == 3) {
		Gui::Button(button_selector_idx, 166, 95);
	} else if (Selection1 == 4) {
		Gui::Button(button_selector_idx, 3, 158);
	} else if (Selection1 == 5) {
		Gui::Button(button_br_selector_idx, 161, 153);
	}
}

void MainMenu::drawSelection2(void) const
{
	if (Selection2 == 0) {
		Gui::Button(button_tl_selector_idx, 0, 26);
	} else if (Selection2 == 1) {
		Gui::Button(button_selector_idx, 166, 31);
	} else if (Selection2 == 2) {
		Gui::Button(button_selector_idx, 3, 95);
	} else if (Selection2 == 3) {
		Gui::Button(button_selector_idx, 166, 95);
	} else if (Selection2 == 4) {
		Gui::Button(button_selector_idx, 3, 158);
	}
}

void MainMenu::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Universal-Manager"))/2, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text(340, 218, FONT_SIZE_18, WHITE, universal_manager_vertext);

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();


	// First Page.
	if (currentPage == 0) {
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
	drawSelection1();


	// Second Page.
	} else if (currentPage == 1) {
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
	drawSelection2();
}
}

void MainMenu::SelectionLogic1(u32 hDown) {
		if (currentPage == 0 && hDown & KEY_UP) {
			if(Selection1 > 0)	Selection1--;
		} else if (currentPage == 0 && hDown & KEY_DOWN) {
			if(Selection1 < 5)	Selection1++;
		}  else if (currentPage == 0 && hDown & KEY_A) {
			switch(Selection1) {
				case 0: {
					Gui::setScreen(std::make_unique<FileManager>());
					break;
				}   case 1:
					Gui::setScreen(std::make_unique<FTP>());
					break;
				  case 2: {
					Gui::setScreen(std::make_unique<ScriptMain>());
					break;
				} case 3: {
					Gui::setScreen(std::make_unique<MusicMain>());
					break;
				} case 4: {
					Gui::setScreen(std::make_unique<Updater>());
					break;
				} case 5:
					Gui::setScreen(std::make_unique<Settings>());
					break;
			}
		}
}

void MainMenu::SelectionLogic2(u32 hDown) {
		if (currentPage == 1 && hDown & KEY_UP) {
			if(Selection2 > 0)	Selection2--;
		} else if (currentPage == 1 && hDown & KEY_DOWN) {
			if(Selection2 < 4)	Selection2++;
		}  else if (currentPage == 1 && hDown & KEY_A) {
			switch(Selection2) {
				case 0: {
					Gui::setScreen(std::make_unique<ImageSelector>());
					break;
				} case 1:
					Gui::setScreen(std::make_unique<Credits>());
					break;
				  case 2: {
					Gui::setScreen(std::make_unique<TextBrowse>());
					break;
				} case 3: {
					Gui::setScreen(std::make_unique<Utils>());
					break;
				} case 4: {
					Gui::setScreen(std::make_unique<GameSub>());
					break;
				}
			}
		} 
}

void MainMenu::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic1(hDown);
	SelectionLogic2(hDown);
		// Page Switching.
	if (currentPage == 0 && hDown & KEY_R) {
		currentPage = 1;
		} else if (currentPage == 1 && hDown & KEY_L) {
		currentPage = 0;

		} else if (hDown & KEY_START) {
			exiting = true;

		// First Page.
		} else if (currentPage == 0 && hDown & KEY_TOUCH) {
			if (touching(touch, mainScreenButtonPos[0])) {
				Gui::setScreen(std::make_unique<FileManager>());
			} else if (touching(touch, mainScreenButtonPos[1])) {
				Gui::setScreen(std::make_unique<FTP>());
			} else if (touching(touch, mainScreenButtonPos[2])) {
				Gui::setScreen(std::make_unique<ScriptMain>());
			} else if (touching(touch, mainScreenButtonPos[3])) {
				Gui::setScreen(std::make_unique<MusicMain>());
			} else if (touching(touch, mainScreenButtonPos[4])) {
				Gui::setScreen(std::make_unique<Updater>());
			} else if (touching(touch, mainScreenButtonPos[5])) {
				Gui::setScreen(std::make_unique<Settings>());
			}

			// Second Page.
		} else if (currentPage == 1 && hDown & KEY_TOUCH) {
			if (touching(touch, mainScreenButtonPos[6])) {
				Gui::setScreen(std::make_unique<ImageSelector>());
			} else if (touching(touch, mainScreenButtonPos[7])) {
				Gui::setScreen(std::make_unique<Credits>());
			} else if (touching(touch, mainScreenButtonPos[8])) {
				Gui::setScreen(std::make_unique<TextBrowse>());
 			} else if (touching(touch, mainScreenButtonPos[9])) {
				Gui::setScreen(std::make_unique<Utils>());
			} else if (touching(touch, mainScreenButtonPos[10])) {
				Gui::setScreen(std::make_unique<GameSub>());
		}
	}
}