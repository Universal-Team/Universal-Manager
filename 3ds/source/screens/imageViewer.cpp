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

extern "C" {
#include "C2D_helper.h"
}

extern uint selectedFile;
extern int keyRepeatDelay;
extern bool dirChanged;
extern std::vector<DirEntry> dirContents;
std::string currentImage = "";
std::string filename;
double imageScale = 1.0f;
int positionX = 0, positionY = 0;
ImageSize imageSize;

static C2D_Image image;

static void FreeImage(C2D_Image *image) {
	C3D_TexDelete(image->tex);
	linearFree((Tex3DS_SubTexture *)image->subtex);
	C2D_TargetClear(top, C2D_Color32(33, 39, 43, 255));
	C2D_TargetClear(bottom, C2D_Color32(33, 39, 43, 255));
}

static bool Draw_Image(void)
{
	return C2D_DrawImageAt(image, positionX, positionY, 0.5, nullptr, imageScale, imageScale);
}


void drawImageSelectorScreen(void) {
	drawFileBrowser();
}


void showImage(void) {
	C2D_TargetClear(top, C2D_Color32(33, 39, 43, 255));
	C2D_TargetClear(bottom, C2D_Color32(33, 39, 43, 255));
	set_screen(top);
	Draw_Image(); 
	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, C2D_Color32(33, 39, 43, 255));
}

void ImageSelectorLogic(u32 hDown, u32 hHeld) { 
	if (keyRepeatDelay)	keyRepeatDelay--;
	gspWaitForVBlank();
	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			if(dirContents[selectedFile].name != currentImage) {
			}
			if(confirmPopup("Do you want, to see this Image?\nMake sure it is not taller than 1024x576 pixel.")) {
			imageSize = Draw_LoadImageFile(&image, dirContents[selectedFile].name.c_str());
			screenTransition(showImageScreen);
			}
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			screenTransition(mainScreen);
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
	} else if (hDown & KEY_X) {
		screenTransition(mainScreen);
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE000 to open an Image. \n \n Press \uE001 to go back a Folder. \n\n Press \uE002 to exit to the Main Menu.");
	}
}

void showImageLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hHeld & KEY_CPAD_UP) {
		if(imageSize.height*imageScale < 240) {
			if(positionY > 0)	positionY -= imageScale*2;
		} else {
			if(positionY > 240-(imageSize.height*imageScale))	positionY -= imageScale*2;
		}
	} else if (hHeld & KEY_CPAD_DOWN) {
		if(imageSize.height*imageScale < 240) {
			if(positionY < 240-(imageSize.height*imageScale))	positionY += imageScale*2;
		} else {
			if(positionY < 0)	positionY += imageScale*2;
		}
	} else if (hHeld & KEY_CPAD_LEFT) {
		if(imageSize.width*imageScale < 400) {
			if(positionX > 0)	positionX -= imageScale*2;
		} else {
			if(positionX > 400-(imageSize.width*imageScale))	positionX -= imageScale*2;
		}
	} else if (hHeld & KEY_CPAD_RIGHT) {
		if(imageSize.width*imageScale < 400) {
			if(positionX < 400-(imageSize.width*imageScale))	positionX += imageScale*2;
		} else {
			if(positionX < 0)	positionX += imageScale*2;
		}
	} else if(hHeld & KEY_UP) {
		if(imageScale < 10)	imageScale += 0.1;
	} else if(hHeld & KEY_DOWN) {
		if(imageScale > 0)	imageScale -= 0.1;
	} else if (hDown & KEY_B) {
		screenTransition(ImageSelectorScreen);
		FreeImage(&image);
	} else if(hDown & KEY_SELECT) {
		imageScale = 1.0f;
		positionX = 0;
		positionY = 0;
	}
}