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
#include "scripts.hpp"
#include "fileBrowse.h"
#include "keyboard.hpp"
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
uint selectedScpt = 0;
std::vector<DirEntry> scpts;

uint selectedScptItem = 0;
int movingScptItem = -1;
std::vector<std::string> scptContents;

void drawScriptMainScreen(void) {
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(90, 0, FONT_SIZE_18, WHITE, "Script Main Screen");
	mkdir("sdmc:/Universal-Manager/scripts/", 0777);
	
	if(dirChanged) {
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/scripts/");
		getDirectoryContents(scpts);
		chdir(startPath);
	}

	std::string scptList;
	for (uint i=(selectedScpt<5) ? 0 : selectedScpt-5;i<scpts.size()&&i<((selectedScpt<5) ? 6 : selectedScpt+1);i++) {
		if (selectedScpt == 0) {
			Gui::sprite(sprites_selected_idx, 0, 25);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 1) {
			Gui::sprite(sprites_selected_idx, 0, 55);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 2) {
			Gui::sprite(sprites_selected_idx, 0, 90);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 3) {
			Gui::sprite(sprites_selected_idx, 0, 125);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 4) {
			Gui::sprite(sprites_selected_idx, 0, 160);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 5) {
			Gui::sprite(sprites_selected_idx, 0, 195);
			scptList += scpts[i].name + "\n\n";
		} else {
			Gui::sprite(sprites_selected_idx, 0, 195);
			scptList += scpts[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((scpts.size()<6) ? 6-scpts.size() : 0);i++) {
		scptList += "\n";
	}
	Draw_Text(26, 27, 0.56f, WHITE, scptList.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
}


void testWrite(void) {
	FILE* scpt = fopen(("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name).c_str(), "a");
	std::string scriptLine1 = Input::getLine();
	fputs((scriptLine1.c_str()), scpt);
	fclose(scpt);
}

void scriptMainScreenLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	gspWaitForVBlank();
	if(hDown & KEY_A) {
		if(confirmPopup("Do you want to run this Script : \n\n "+scpts[selectedScpt].name+"")) {
			runScript("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name);
		} 
	} else if (hDown & KEY_B) {
		screenMode = mainScreen;
	} else if (hDown & KEY_Y) {
		std::string newScript = Input::getLine();
		if(newScript != "") {
			FILE* scpt = fopen(("sdmc:/Universal-Manager/scripts/"+newScript+".scpt").c_str(), "w");
			fclose(scpt);
	}
	} else if (hDown & KEY_X) {
		if (selectedScpt != 0) {
			if(confirmPopup("Are you sure you want to delete this Script?")) {
				remove(("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name).c_str());
			}
		}
	} else if (hHeld & KEY_UP) {
		if (selectedScpt > 0 && !keyRepeatDelay) {
			selectedScpt--;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedScpt < scpts.size()-1) {
			selectedScpt++;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	} else if (hDown & KEY_SELECT) {
		helperBox(" Press A to start the selected Script. \n \n Press B to return to the Main Menu Screen. \n \n Press X to Delete the selected scpt File. \n \n Press Y to create scpt Files.");
	}
}
