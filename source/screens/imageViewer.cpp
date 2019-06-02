/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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
#include <unistd.h>
#include <vector>
#include "fileBrowse.h"

extern uint selectedFile;
extern int keyRepeatDelay;
extern bool dirChanged;
extern std::vector<DirEntry> dirContents;
std::string currentImage = "";
std::string filename;


void drawPNGImageViewerUI(void) {
	// Theme Stuff.
	drawBgTop();
	drawBarsTop();
	volt_draw_text_center(GFX_TOP, 3, 0.72f, 0.72f, WHITE, "Image Viewer Menu [PNG]");
	drawBatteryTop();

	if (dirChanged) {
		dirContents.clear();
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp);
		for(uint i=0;i<dirContentsTemp.size();i++) {
				if ((strcasecmp(dirContentsTemp[i].name.substr(dirContentsTemp[i].name.length()-3, 3).c_str(), "png") == 0 ||
				dirContentsTemp[i].isDirectory)) {
				dirContents.push_back(dirContentsTemp[i]);
			}
		}
		dirChanged = false;
	}
	std::string dirs;
	for (uint i=(selectedFile<12) ? 0 : selectedFile-12;i<dirContents.size()&&i<((selectedFile<12) ? 13 : selectedFile+1);i++) {
		if (i == selectedFile) {
			dirs += "> " + dirContents[i].name + "\n";
		} else {
			dirs += "  " + dirContents[i].name + "\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<13) ? 13-dirContents.size() : 0);i++) {
		dirs += "\n";
	}
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, dirs.c_str());

	drawBgBot();
	drawBarsBot();
	displayTime();
	volt_end_draw();
}

void drawBMPImageViewerUI(void) {
	// Theme Stuff.
	drawBgTop();
	drawBarsTop();
	volt_draw_text_center(GFX_TOP, 3, 0.72f, 0.72f, WHITE, "Image Viewer Menu [BMP]");
	drawBatteryTop();

	if (dirChanged) {
		dirContents.clear();
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp);
		for(uint i=0;i<dirContentsTemp.size();i++) {
				if ((strcasecmp(dirContentsTemp[i].name.substr(dirContentsTemp[i].name.length()-3, 3).c_str(), "bmp") == 0 ||
				dirContentsTemp[i].isDirectory)) {
				dirContents.push_back(dirContentsTemp[i]);
			}
		}
		dirChanged = false;
	}
	std::string dirs;
	for (uint i=(selectedFile<12) ? 0 : selectedFile-12;i<dirContents.size()&&i<((selectedFile<12) ? 13 : selectedFile+1);i++) {
		if (i == selectedFile) {
			dirs += "> " + dirContents[i].name + "\n";
		} else {
			dirs += "  " + dirContents[i].name + "\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<13) ? 13-dirContents.size() : 0);i++) {
		dirs += "\n";
	}
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, dirs.c_str());

	drawBgBot();
	drawBarsBot();
	displayTime();
	volt_end_draw();
}


void showImage(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	volt_draw_texture(SDImage, 0, 0);

	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
	drawBgBot();
	drawBarsBot();
	volt_draw_text(110, 4, 0.45f, 0.45f, WHITE, "Press B to exit.");
	volt_end_draw();
}

void BMPSelectorLogic(u32 hDown, u32 hHeld) { 
	if (keyRepeatDelay)	keyRepeatDelay--; 
	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			if(dirContents[selectedFile].name != currentImage) {
			}
			if(confirmPopup("Do you want, to see this Image?\nMake sure it is maximal 400x240pixel.")) {
			volt_free_texture(SDImage);
			volt_load_texture_bmp(SDImage, dirContents[selectedFile].name.c_str());
			screenMode = showImageScreen;
			}
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			screenMode = fileScreen;
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
		} else if (hDown & KEY_X) {
		screenMode = PNGScreen;
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			keyRepeatDelay = 3;
		}
	}
}

void PNGSelectorLogic(u32 hDown, u32 hHeld) { 
	if (keyRepeatDelay)	keyRepeatDelay--; 
	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			if(dirContents[selectedFile].name != currentImage) {
			}
			if(confirmPopup("Do you want, to see this Image?\nMake sure it is maximal 400x240pixel.")) {
			volt_free_texture(SDImage);
			volt_load_texture_png(SDImage, dirContents[selectedFile].name.c_str());
			screenMode = showImageScreen;
			}
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			screenMode = fileScreen;
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
	} else if (hDown & KEY_X) {
		screenMode = BMPScreen;
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			keyRepeatDelay = 3;
		}
	}
}

void showImageLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = PNGScreen;
	} 
}