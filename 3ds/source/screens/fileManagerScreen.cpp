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

#include "screens/fileManagerScreen.hpp"
#include "screens/screenCommon.hpp"
#include "utils/settings.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>

void FileManager::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	Draw_Text((400-(Draw_GetTextWidth(0.68f, path)))/2, 0, 0.68f, WHITE, path);
	std::string dirs;
	for (uint i=(selectedFile<5) ? 0 : selectedFile-5;i<dirContents.size()&&i<((selectedFile<5) ? 6 : selectedFile+1);i++) {
		(i == selectedFile);

		if (selectedFile == 0) {
			Gui::drawFileSelector(0, 28);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 1) {
			Gui::drawFileSelector(0, 58);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 2) {
			Gui::drawFileSelector(0, 91);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 3) {
			Gui::drawFileSelector(0, 125);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 4) {
			Gui::drawFileSelector(0, 156);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 5) {
			Gui::drawFileSelector(0, 188);
			dirs +=  dirContents[i].name + "\n\n";
		} else {
			Gui::drawFileSelector(0, 188);
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

    if (Config::selector == 0) {
        Draw_Text(26, 32, 0.53f, WHITE, dirs.c_str());
    } else if (Config::selector == 1 || Config::selector == 2) {
        Draw_Text(26, 32, 0.53f, BLACK, dirs.c_str());
    }

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void FileManager::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (dirChanged) {
            dirContents.clear();
            std::vector<DirEntry> dirContentsTemp;
            getDirectoryContents(dirContentsTemp);
            for(uint i=0;i<dirContentsTemp.size();i++) {
                  dirContents.push_back(dirContentsTemp[i]);
        }
		dirChanged = false;
	}

			if (refresh) {
            dirContents.clear();
            std::vector<DirEntry> dirContentsTemp;
            getDirectoryContents(dirContentsTemp);
            for(uint i=0;i<dirContentsTemp.size();i++) {
                  dirContents.push_back(dirContentsTemp[i]);
        }
		refresh = false;
	}

		if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		}
		} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			Gui::screenBack();
			return;
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
	} else if (hDown & KEY_X) {
		displayActionBox();
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			keyRepeatDelay = 6;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			keyRepeatDelay = 6;
		}
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE001 to go back a Folder \n \n Press \uE002 to open the Action Menu.");
	}
}

// FileManager Operations!

void FileManager::renameFile(void) {
	std::string newName = Input::getLine();
	if(newName != "")	rename(dirContents[selectedFile].name.c_str(), newName.c_str());
}

void FileManager::deleteFile(void) {
	DisplayMsg("Delete is in progress...\nPlease wait...");
	remove(dirContents[selectedFile].name.c_str());
}

void FileManager::copyPaste(void) {
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

void FileManager::createFolder(void) {
	std::string newName = Input::getLine();
	mkdir(newName.c_str(), 0777);
}

void FileManager::extractarchive(void) {
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	std::string outPath = path + dirContents[selectedFile].name.substr(0, dirContents[selectedFile].name.find_last_of(".")) + "/";
	mkdir(outPath.c_str(), 0777);
	DisplayMsg("Extract is in progress...\nPlease wait...");
	extractArchive(dirContents[selectedFile].name, "/", outPath);
}

void FileManager::install(void) {
	DisplayMsg("Install is in progress...\nPlease wait...");
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	std::string installPath = path;
	installPath += dirContents[selectedFile].name.c_str();
	installCia(installPath.c_str());
}


bool FileManager::displayActionBox(void)
{
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
					refresh = true;
					}
					break;
				} case 1:
					if(confirmPopup("Do you want to delete this File?")) {
					deleteFile();
					refresh = true;
					}
					break;
				case 2: { 
					copyPaste();
					refresh = true;
					break;
				} case 3: { 
					createFolder();
					refresh = true;
					break;
				} case 4: {
					if(confirmPopup("Do you want to extract this Archive?")) {
					extractarchive();
					refresh = true;
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