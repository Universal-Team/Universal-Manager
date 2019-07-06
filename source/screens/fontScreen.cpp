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
#include "fileBrowse.h"
#include "settings.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>
using std::string;


extern uint selectedFile;
extern int keyRepeatDelay;
extern bool dirChanged;
extern std::vector<DirEntry> dirContents;
uint selectedBcfnt = 0;
std::vector<DirEntry> bcfnts;
extern C2D_Font customFont;

void drawFontSelection(void) {
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(80, 0, FONT_SIZE_18, WHITE, "Font Select Screen");
	mkdir("sdmc:/Universal-Manager/Fonts/", 0777);
	
	if(dirChanged) {
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/Fonts/");
		getDirectoryContents(bcfnts);
		chdir(startPath);
	}

	std::string bcfntList;
	std::string bcfntList2;
	for (uint i=(selectedBcfnt<5) ? 0 : selectedBcfnt-5;i<bcfnts.size()&&i<((selectedBcfnt<5) ? 6 : selectedBcfnt+1);i++) {
		if (selectedBcfnt == 0) {
			Gui::sprite(sprites_selected_idx, 0, 25);
			bcfntList +=  bcfnts[i].name + "\n\n";

		} else if (selectedBcfnt == 1) {
			Gui::sprite(sprites_selected_idx, 0, 55);
			bcfntList +=  bcfnts[i].name + "\n\n";

		} else if (selectedBcfnt == 2) {
			Gui::sprite(sprites_selected_idx, 0, 90);
			bcfntList +=  bcfnts[i].name + "\n\n";

		} else if (selectedBcfnt == 3) {
			Gui::sprite(sprites_selected_idx, 0, 125);
			bcfntList +=  bcfnts[i].name + "\n\n";

		} else if (selectedBcfnt == 4) {
			Gui::sprite(sprites_selected_idx, 0, 160);
			bcfntList +=  bcfnts[i].name + "\n\n";

		} else if (selectedBcfnt == 5) {
			Gui::sprite(sprites_selected_idx, 0, 195);
			bcfntList +=  bcfnts[i].name + "\n\n";
		} else {
			Gui::sprite(sprites_selected_idx, 0, 195);
			bcfntList += "  " + bcfnts[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((bcfnts.size()<6) ? 6-bcfnts.size() : 0);i++) {
		bcfntList += "\n";
	}
	bcfntList2 += "A : Select Font   SELECT : Select Standard Font   B : Back   Y : SystemFont";
	Draw_Text(26, 27, 0.56f, WHITE, bcfntList.c_str());
	Draw_Text(26, 220, 0.45f, WHITE, bcfntList2.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
}

static void selectFont(void) {
		customFont = C2D_FontLoad(("sdmc:/Universal-Manager/Fonts/"+bcfnts[selectedBcfnt].name).c_str());
		Config::Font = 1;
}

void FontSelectionLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	gspWaitForVBlank();
	if(hDown & KEY_A) {
		if(confirmPopup("Do you want to use this Font : \n\n "+bcfnts[selectedBcfnt].name+"")) {
		selectFont();
		screenMode = uiSettingsScreen;
		} 
	} else if (hDown & KEY_B) {
		screenMode = uiSettingsScreen;
	} else if (hDown & KEY_SELECT) {
		if(confirmPopup("Do you want to use the Default Font?")) {
		Config::Font = 0;
		screenMode = uiSettingsScreen;
		}
		} else if (hDown & KEY_Y) {
		if(confirmPopup("Do you want to use the System Font?")) {
		Config::Font = 2;
		screenMode = uiSettingsScreen;
		}
	} else if (hHeld & KEY_UP) {
		if (selectedBcfnt > 0 && !keyRepeatDelay) {
			selectedBcfnt--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedBcfnt < bcfnts.size()-1) {
			selectedBcfnt++;
			keyRepeatDelay = 3;
		}
	}
}