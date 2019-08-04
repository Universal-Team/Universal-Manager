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
#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "extract.hpp"
#include "fileBrowse.h"
#include "fileOperations.h"
#include "keyboard.hpp"

extern "C" {
#include "C2D_helper.h"
#include "cia.h"
}


struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	std::string text;
};

extern uint selectedFile;
extern std::vector<DirEntry> dirContents;

DirEntry clipboard;

ButtonPos functionPos[] = {
	{59, 70, 93, 35, "Rename"},
	{165, 70, 93, 35, "Delete"},
	{59, 110, 93, 35, "Copy/Paste"},
	{165, 110, 93, 35, "Create"},
	{59, 150, 93, 35, "Extract"},
	{165, 150, 93, 35, "Install"},
};

static void renameFile(void) {
	std::string newName = Input::getLine();
	if(newName != "")	rename(dirContents[selectedFile].name.c_str(), newName.c_str());
}

static void deleteFile(void) {
	DisplayMsg("Delete is in progress...\nPlease wait...");
	remove(dirContents[selectedFile].name.c_str());
}

static void copyPaste(void) {
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	if(clipboard.name == "") {
	clipboard = dirContents[selectedFile];
	clipboard.path = path;
	} else {
	if(strcmp(path, clipboard.path.c_str()) != 0) {
	if(clipboard.isDirectory)
	mkdir(clipboard.name.c_str(), 0777);
	DisplayMsg("Paste is in progress...\nPlease wait...");
	fcopy((clipboard.path+clipboard.name).c_str(), (path+clipboard.name).c_str());
	clipboard.name = "";
		}
	}
}

static void createFolder(void) {
	std::string newName = Input::getLine();
	mkdir(newName.c_str(), 0777);
}

static void extractArchive(void) {
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	std::string outPath = path + dirContents[selectedFile].name.substr(0, dirContents[selectedFile].name.find_last_of(".")) + "/";
	mkdir(outPath.c_str(), 0777);
	DisplayMsg("Extract is in progress...\nPlease wait...");
	extractArchive(dirContents[selectedFile].name, "/", outPath);
}

static void install(void) {
	DisplayMsg("Install is in progress...\nPlease wait...");
	installCia(dirContents[selectedFile].name.c_str());
}

bool displayActionBox(void) {
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	set_screen(bottom);
	C2D_DrawRectSolid(54, 30, 0.5f, 211, 180, Config::barColor);
	Gui::sprite(sprites_actionBox_idx, 54, 30);

	// Buttons.
	for(uint i=0; i<(sizeof(functionPos)/sizeof(functionPos[0]));i++) {
		Gui::sprite(sprites_fileManagerUnselected_idx, functionPos[i].x, functionPos[i].y);
		Draw_Text(functionPos[i].x+12, functionPos[i].y+10, 0.6f, WHITE, functionPos[i].text.c_str());
	}

	Gui::clearTextBufs();
	C3D_FrameEnd(0);
	int selection = 0;
	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		if(keysDown() & KEY_UP) {
			if(selection > 0)	selection--;
		} else if(keysDown() & KEY_DOWN) {
			if(selection < 5)	selection++;
		} else if(keysDown() & KEY_A) {
			switch(selection) {
				case 0: {
					if(confirmPopup("Do you want to rename this File?")) { 
					renameFile();
					}
					break;
				} case 1:
					if(confirmPopup("Do you want to delete this File?")) {
					deleteFile();
					}
					break;
				case 2: { 
					copyPaste();
					break;
				} case 3: { 
					createFolder();
					break;
				} case 4: {
					if(confirmPopup("Do you want to extract this Archive?")) {
					extractArchive();
					}
					break;
				} case 5:
					if(confirmPopup("Do you want to install this CIA?")) {
					install();
					}
					break;
			}
		return true;
		} else if(keysDown() & KEY_B) {
			return false;
		}
	}
}