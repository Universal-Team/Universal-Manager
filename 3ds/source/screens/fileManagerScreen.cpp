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

extern touchPosition touch;

bool touching(touchPosition touch, Structs::TextBtn button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}


void FileManager::Draw(void) const
{
	if (fileMode == 0) {
		DrawBrowse();
	} else if (fileMode == 1) {
		DisplayActionBox();
	}
}

void FileManager::DrawBrowse(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	Gui::DrawString((400-(Gui::GetStringWidth(0.68f, path)))/2, 0, 0.68f, WHITE, path);
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
        Gui::DrawString(26, 32, 0.53f, WHITE, dirs.c_str());
    } else if (Config::selector == 1 || Config::selector == 2) {
        Gui::DrawString(26, 32, 0.53f, BLACK, dirs.c_str());
    }

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}


void FileManager::DisplayActionBox(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-(Gui::GetStringWidth(0.68f, currentSelectedFile.c_str())))/2, 218, 0.65f, WHITE, currentSelectedFile.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
	C2D_DrawRectSolid(54, 30, 0.5f, 211, 180, Config::barColor);
	Gui::sprite(0, sprites_actionBox_idx, 54, 30);

	// Buttons.
	if (currentPage == 1) {
		Gui::sprite(3, button_fileManager_button_idx, functionPos[0].x, functionPos[0].y);
		Gui::DrawString(functionPos[0].x+6, functionPos[0].y+10, 0.6f, WHITE, functionPos[0].text.c_str());
		Gui::sprite(3, button_fileManager_button_idx, functionPos[1].x, functionPos[1].y);
		Gui::DrawString(functionPos[1].x+6, functionPos[1].y+10, 0.6f, WHITE, functionPos[1].text.c_str());
		Gui::sprite(3, button_fileManager_button_idx, functionPos[2].x, functionPos[2].y);
		Gui::DrawString(functionPos[2].x+6, functionPos[2].y+10, 0.6f, WHITE, functionPos[2].text.c_str());
		Gui::sprite(3, button_fileManager_button_idx, functionPos[3].x, functionPos[3].y);
		Gui::DrawString(functionPos[3].x+6, functionPos[3].y+10, 0.6f, WHITE, functionPos[3].text.c_str());
		Gui::sprite(3, button_fileManager_button_idx, functionPos[4].x, functionPos[4].y);
		Gui::DrawString(functionPos[4].x+6, functionPos[4].y+10, 0.6f, WHITE, functionPos[4].text.c_str());
		Gui::sprite(3, button_fileManager_button_idx, functionPos[5].x, functionPos[5].y);
		Gui::DrawString(functionPos[5].x+6, functionPos[5].y+10, 0.6f, WHITE, functionPos[5].text.c_str());
	} else if (currentPage == 2) {
		Gui::sprite(3, button_fileManager_button_idx, functionPos[6].x, functionPos[6].y);
		Gui::DrawString(functionPos[6].x+6, functionPos[6].y+10, 0.6f, WHITE, functionPos[6].text.c_str());
	}

	// Selector.
	for(uint i=0; i<(sizeof(functionPos)/sizeof(functionPos[0]));i++) {
		Gui::drawGUISelector(button_fileButtonSelector_idx, functionPos[currentSelection].x, functionPos[currentSelection].y, 0.005f);
	}
}



void FileManager::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (fileMode == 0) {
		FileBrowseLogic(hDown, hHeld, touch);
	} else if (fileMode == 1) {
		ActionBoxLogic();
	}
}

void FileManager::FileBrowseLogic(u32 hDown, u32 hHeld, touchPosition touch) {
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
		currentSelectedFile = "Current File: ";
		currentSelectedFile += dirContents[selectedFile].name.c_str();
		fileMode = 1;
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
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

void FileManager::createFile(void) {
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	std::string currentPath = path;
	currentPath += Input::getLine();
	std::ofstream file { currentPath.c_str() };
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

void FileManager::ActionBoxLogic(void) {
		hidScanInput();
		hidTouchRead(&touch);

		if(keysDown() & KEY_UP) {
			if(currentSelection > 0)	currentSelection--;


		} else if(keysDown() & KEY_DOWN) {
			if (currentPage == 1) {
				if(currentSelection < 5)	currentSelection++;
			} else if (currentPage == 2) {
			}

		} else if (keysDown() & KEY_L) {
			if (currentPage == 2) {
				currentSelection = 0;
				currentPage = 1;
			}
		} else if (keysDown() & KEY_R) {
			if (currentPage == 1) {
				currentSelection = 0;
				currentPage = 2;
			}

		} else if (keysDown() & KEY_TOUCH) {
			if (currentPage == 1) {
				if (touching(touch, functionPos[0])) {
						if(confirmPopup("Do you want to rename this File?")) { 
							renameFile();
							refresh = true;
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}


				} else if (touching(touch, functionPos[1])) {
						if(confirmPopup("Do you want to delete this File?")) {
							deleteFile();
							refresh = true;
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}


				} else if (touching(touch, functionPos[2])) {
						if(confirmPopup("Do you want to create a File?")) {
							createFile();
							refresh = true;
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}

						
				} else if (touching(touch, functionPos[3])) {
						if(confirmPopup("Do you want to create a Folder?")) {
							createFolder();
							refresh = true;
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}


				} else if (touching(touch, functionPos[4])) {
						if(confirmPopup("Do you want to extract this Archive?")) {
							extractarchive();
							refresh = true;
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}


				} else if (touching(touch, functionPos[5])) {
						if(confirmPopup("Do you want to Copy/Paste this?")) {
							copyPaste();
							refresh = true;
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}
			}
			} else if (currentPage == 2) {

				if (touching(touch, functionPos[6])) {
						if(confirmPopup("Do you want to Install this File?")) { 
							install();
							fileMode = 0;
							currentSelection = 0;
							currentPage = 1;
						}
			}
			}

		} else if(keysDown() & KEY_A) {
			if (currentPage == 1) {
				switch(currentSelection) {
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
						if(confirmPopup("Do you want to create a File?")) {
							createFile();
							refresh = true;
						}
						break;
					} case 3: { 
						if(confirmPopup("Do you want to create a Folder?")) {
							createFolder();
							refresh = true;
						}
						break;
					} case 4: {
						if(confirmPopup("Do you want to extract this Archive?")) {
							extractarchive();
							refresh = true;
						}
						break;
					} case 5:
						if(confirmPopup("Do you want to Copy/Paste this?")) {
							copyPaste();
							refresh = true;
						}
						break;
			}
			} else if (currentPage == 2) {
				switch(currentSelection) {
					case 0: {
						if(confirmPopup("Do you want to Install this File?")) { 
							install();
						}
						break;
					}
				}
			}
			fileMode = 0;
			currentSelection = 0;
			currentPage = 1;
		} else if(keysDown() & KEY_B) {
			fileMode = 0;
			currentSelection = 0;
			currentPage = 1;
		}
}