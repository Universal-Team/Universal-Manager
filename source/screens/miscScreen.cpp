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
#include "bftps.h"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

extern bool touching(touchPosition touch, ButtonPos button);


ButtonPos ftpFunctionButtonPos[] = {
	// Back Icon.
	{293, 213, 27, 27, -1},
};

ButtonPos ftpButtonPos[] = {
		// Back Icon.
	{293, 213, 27, 27, mainScreen},
};



void drawCredits(void) {
	C2D_SceneBegin(top);
	Gui::sprite(sprites_universal_credits_idx, 0, 0);
	
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();
	DisplayTime();
	drawBatteryBot();
}

void ftpLogic(u32 hDown, touchPosition touch) {
		if (hDown & KEY_B) {
		screenMode = mainScreen;
		bftps_stop();
	} else if(hDown & KEY_TOUCH) {
		for(uint i=0;i<(sizeof(ftpButtonPos)/sizeof(ftpButtonPos[0]));i++) {
			if (touching(touch, ftpButtonPos[i])) {
				screenMode = ftpButtonPos[i].link;
			}
		}
	}
}

const char* my_basename(const char* path) {
    const char *pLastSlash = path;
    while (*path != '\0') {
        if (*path == '/')
            pLastSlash = path+1;
        path++;
    }
    return pLastSlash;
}

void drawFTPScreen(void) {

	bftps_start();

	Result ret = 0;
	char buf[137], buf2[512], buf3[512];
	u32 wifiStatus = 0;

	int position = 0, progress = 0, xlim = 270;


	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	draw_text_center(GFX_TOP, 0, 0.5f, 0.72f, 0.72f, WHITE, "FTP Mode"); // - Fine.
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	ret = ACU_GetWifiStatus(&wifiStatus);

	if ((wifiStatus != 0) && R_SUCCEEDED(ret)) {
		draw_text_center(GFX_BOTTOM, 40, 0.5f, 0.5f, 0.5f, WHITE, "FTP initialized"); // Fine.
                        snprintf(buf, 137, "%s", bftps_name());

 	const bftps_file_transfer_t* transfersInfo = bftps_file_transfer_retrieve();
        if (transfersInfo) {
            const bftps_file_transfer_t* file = transfersInfo;

             // while (file) { for now only show the first file on the list
                 if (file->mode == FILE_SENDING) {
                     float fraction = ((float) file->filePosition / (float) file->fileSize);
                     snprintf(buf3, 512, "Sending %.2f%%", ((float) file->filePosition / ((float) 1024 * (float) 1024)));

                         draw_text_center(GFX_BOTTOM, 110, 0.5f, 0.5f, 0.5f, WHITE, buf3); // Fine.
						 draw_text_center(GFX_BOTTOM, 90, 0.5f, 0.5f, 0.5f, WHITE, my_basename(file->name));
                                    position = 0;
                                    progress = 40+round(fraction * (float)(xlim-40));
                                }                                   
                                else {
                                    snprintf(buf2, 512, "Receiving %.2fMB", ((float) file->filePosition / ((float) 1024 * (float) 1024)));
                                            
                                    //file name should have an elipsis when is to longer
                                    draw_text_center(GFX_BOTTOM, 110, 0.5f, 0.5f, 0.5f, WHITE, buf2);
									draw_text_center(GFX_BOTTOM, 90, 0.5f, 0.5f, 0.5f, WHITE, my_basename(file->name));                                    
                                    progress = 40;
                                    position += 4;			
                                    if (position >= xlim)
					position = 0;
                                }
                                    //aux = aux->next;
                            //}
                            bftps_file_transfer_cleanup(transfersInfo); 
                            
                            C2D_DrawRectSolid(50, 140, 0.5f, 220, 3, WHITE);
                            C2D_DrawRectSolid(position, 140, 0.5f, progress, 3, WHITE);
                            
                            // Boundary stuff
                            C2D_DrawRectSolid(0, 140, 0.5f, 50, 3, BLACK);
                            C2D_DrawRectSolid(270, 140, 0.5f, 50, 3, BLACK); 
                            
                        }
		}
		else {
			Gui::staticText("Failed to initialize FTP.", 150, 40, 0.5f, 0.5f, WHITE, TextPosX::CENTER, TextPosY::TOP); // - Crash.
			Gui::staticText("WiFi not enabled.", 150, 60, 0.5f, 0.5f, WHITE, TextPosX::CENTER, TextPosY::TOP); // - Crash.
		}
		Gui::staticText(buf, 150, 60, 0.5f, 0.5f, WHITE, TextPosX::CENTER, TextPosY::TOP); // - Crash.
		C3D_FrameEnd(0);
}

// NOTE: This'll get the app stuck in a loop while its running, so background
// processes like the clock won't update while the message bubble is up
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos) {
	Gui::clearStaticText();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	C2D_DrawRectSolid(0, 60, 0.5f, 400, 120, WHITE);
	Gui::staticText(msg1.c_str(), 170, 90, 0.45f, 0.45f, BLACK, TextPosX::CENTER, TextPosY::TOP);
	Gui::staticText(msg2.c_str(), 170, 110, 0.45f, 0.45f, BLACK, TextPosX::CENTER, TextPosY::TOP);
	Gui::staticText(("\uE001 : "+no+"   \uE000 : "+yes).c_str(), ynXPos, 160, 0.45f, 0.45f, BLACK, TextPosX::CENTER, TextPosY::TOP);
	Gui::DrawBGBot();
	Gui::chooseLayoutBot();
	C3D_FrameEnd(0);
	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		if(keysDown() & KEY_A) {
			return true;
		} else if(keysDown() & KEY_B) {
			return false;
		}
	}
}
bool confirmPopup(std::string msg) {
	return confirmPopup(msg, "", "Yes", "No", 200);
}
