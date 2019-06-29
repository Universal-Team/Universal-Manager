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
	for (uint i=(selectedBcfnt<12) ? 0 : selectedBcfnt-12;i<bcfnts.size()&&i<((selectedBcfnt<12) ? 13 : selectedBcfnt+1);i++) {
		if (i == selectedBcfnt) {
			bcfntList += "> " + bcfnts[i].name.substr(0, bcfnts[i].name.find_last_of(".")) + "\n";
		} else {
			bcfntList += "  " + bcfnts[i].name.substr(0, bcfnts[i].name.find_last_of(".")) + "\n";
		}
	}
	for (uint i=0;i<((bcfnts.size()<13) ? 13-bcfnts.size() : 0);i++) {
		bcfntList += "\n";
	}
	bcfntList2 += "A : Select Font Select : Select Standard Font B : Back";
	Draw_Text(26, 32, 0.45f, WHITE, bcfntList.c_str());
	Draw_Text(26, 220, 0.45f, WHITE, bcfntList2.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
}

static void selectFont(void) {
		C2D_FontFree(customFont);
		customFont = C2D_FontLoad(bcfnts[selectedBcfnt].name.c_str());
		settings.universal.font = 1;
		screenMode = uiSettingsScreen;
}

void FontSelectionLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	if(hDown & KEY_A) {
		if(confirmPopup("Do you want to use this Font : \n\n "+bcfnts[selectedBcfnt].name+"")) {
		selectFont();
		} 
	} else if (hDown & KEY_B) {
		screenMode = uiSettingsScreen;
	} else if (hDown & KEY_SELECT) {
		settings.universal.font = 0;
		screenMode = uiSettingsScreen;
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