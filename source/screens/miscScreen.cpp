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
#include <unistd.h>
#include <vector>

extern "C" {
    #include "ftp.h"
}
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

void drawFTPScreen(void) {
	ftp_init();

	Result ret = 0;
	char buf[137], hostname[128];
	u32 wifiStatus = 0;

	int pBar = 0, xlim = 270;

	ret = gethostname(hostname, sizeof(hostname));

	while(screenMode == ftpScreen) {
		ftp_loop();
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		Gui::DrawBGTop();
		Gui::chooseLayoutTop();
		DisplayTime();
		drawBatteryTop();	
		Draw_Text(135, 0, 0.72f, WHITE, "FTP Mode");
		Gui::DrawBGBot();
		Gui::chooseLayoutBot();
		ret = ACU_GetWifiStatus(&wifiStatus);

		if ((wifiStatus != 0) && R_SUCCEEDED(ret)) {
			Draw_Text(((320 - Draw_GetTextWidth(0.48f, "FTP initialized")) / 2), 40, 0.48f, WHITE, "FTP initialized");
			snprintf(buf, 137, "IP: %s:5000", R_FAILED(ret)? "Failed to get IP" : hostname);

			if (strlen(ftp_accepted_connection) != 0)
				Draw_Text(((320 - Draw_GetTextWidth(0.48f, ftp_accepted_connection)) / 2), 80, 0.48f, WHITE, ftp_accepted_connection);

			if (strlen(ftp_file_transfer) != 0)
				Draw_Text(((320 - Draw_GetTextWidth(0.45f, ftp_file_transfer)) / 2), 150, 0.45f, WHITE, ftp_file_transfer);

			if (isTransfering) {
				C2D_DrawRectSolid(50, 140, 0.5f, 220, 3, BLACK);
				C2D_DrawRectSolid(pBar, 140, 0.5f, 40, 3, WHITE);

				// Boundary stuff
				C2D_DrawRectSolid(0, 140, 0.5f, 50, 3, BLACK);
				C2D_DrawRectSolid(270, 140, 0.5f, 50, 3, BLACK); 
				pBar += 4;
			
				if (pBar >= xlim)
					pBar = 34;
			}
		}
		else {
			Draw_Text(((320 - Draw_GetTextWidth(0.48f, "Failed to initialize FTP.")) / 2), 40, 0.48f, WHITE, "Failed to initialize FTP.");
			snprintf(buf, 18, "WiFi not enabled.");
		}

		Draw_Text(((320 - Draw_GetTextWidth(0.48f, buf)) / 2), 60, 0.48f, WHITE, buf);
		Draw_Text(((320 - Draw_GetTextWidth(0.48f, "Press B to Return to the Main Menu.")) / 2), 220, 0.48f, WHITE, "Press B to Return to the Main Menu.");

		Draw_EndFrame();

		hidScanInput();
		u32 hDown = hidKeysDown();

		if (hDown & KEY_B)
			break;
	}

	memset(ftp_accepted_connection, 0, 20); // Empty accepted connection address
	memset(ftp_file_transfer, 0, 50); // Empty transfer status
	ftp_exit();

	screenMode = mainScreen;
}

// NOTE: This'll get the app stuck in a loop while its running, so background
// processes like the clock won't update while the message bubble is up
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos) {
	Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	C2D_DrawRectSolid(0, 60, 0.5f, 400, 120, WHITE);
	Draw_Text(100, 90, 0.45f, BLACK, msg1.c_str());
	Draw_Text(120, 110, 0.45f, BLACK, msg2.c_str());
	Draw_Text(ynXPos, 160, 0.45f, BLACK, ("B : "+no+"   A : "+yes).c_str());
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
	return confirmPopup(msg, "", "Yes", "No", 100);
}
