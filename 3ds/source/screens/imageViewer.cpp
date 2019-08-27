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

#include "screens/imageScreen.hpp"
#include "screens/screenCommon.hpp"
#include "utils/fileBrowse.h"
#include "utils/settings.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>


void Image::FreeImage(C2D_Image *image) {
	C3D_TexDelete(image->tex);
	linearFree((Tex3DS_SubTexture *)image->subtex);
	C2D_TargetClear(top, C2D_Color32(33, 39, 43, 255));
	C2D_TargetClear(bottom, C2D_Color32(33, 39, 43, 255));
}

bool Image::Draw_Image(void) const
{
	return C2D_DrawImageAt(image, positionX, positionY, 0.5, nullptr, imageScale, imageScale);
}

void Image::Draw(void) const
{
	if (ImageMode == 0) {
		DrawBrowse();
	} else if (ImageMode == 1) {
		DrawViewer();
	}
}

void Image::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (ImageMode == 0) {
		BrowseLogic(hDown, hHeld);
	} else if (ImageMode == 1) {
		ViewerLogic(hDown, hHeld);
	}
}


void Image::DrawBrowse(void) const
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


void Image::DrawViewer(void) const
{
	C2D_TargetClear(top, C2D_Color32(33, 39, 43, 255));
	C2D_TargetClear(bottom, C2D_Color32(33, 39, 43, 255));
	set_screen(top);
	Draw_Image(); 
	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, C2D_Color32(33, 39, 43, 255));
}


void Image::BrowseLogic(u32 hDown, u32 hHeld) { 
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
			ImageMode = 1;
			}
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
			Gui::screenBack();
			return;
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
		helperBox(" Press \uE000 to open an Image. \n \n Press \uE001 to go back a Folder. \n\n Press \uE002 to exit to the Main Menu.");
	}
}

void Image::ViewerLogic(u32 hDown, u32 hHeld) {
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
		ImageMode = 0;
		FreeImage(&image);
	} else if(hDown & KEY_SELECT) {
		imageScale = 1.0f;
		positionX = 0;
		positionY = 0;
	}
}