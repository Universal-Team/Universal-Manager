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
	{293, 213, 27, 27, -1},
};

ButtonPos buttonTesterButtonPos[] = {
		// Back Icon.
	{293, 213, 27, 27, -1},
};

ButtonPos gamesSubMenuButtonPos[] = {
		// Pong. :P
    {0, 40, 149, 52, -1},
		// Tic-Tac-Toe. ;P
	{170, 40, 149, 52, -1},
		// Back Icon.
	{293, 213, 27, 27, -1},
};

ButtonPos utilsButtonPos[] = {
		// Calendar.
    {0, 40, 149, 52, -1},
		// Button Tester. ;P
	{170, 40, 149, 52, -1},
		// Calculator.
	{0, 150, 149, 52, -1},
		// Back Icon.
	{293, 213, 27, 27, -1},
};

int gameSelection = 0;
int utilsSelection = 0;

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
		u32 hHeld = hidKeysHeld();

		if (hHeld & KEY_SELECT) {
		}
		if (hDown & KEY_B)
			break;
	}

	memset(ftp_accepted_connection, 0, 20); // Empty accepted connection address
	memset(ftp_file_transfer, 0, 50); // Empty transfer status
	ftp_exit();

	screenTransition(mainScreen);
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
	Draw_Text(200-(Draw_GetTextWidth(0.45f, msg1.c_str())/2), 90, 0.45f, BLACK, msg1.c_str());
	Draw_Text(200-(Draw_GetTextWidth(0.45f, msg1.c_str())/2), 110, 0.45f, BLACK, msg2.c_str());
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

void helperBox(std::string msg1) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	set_screen(top);
	Gui::sprite(sprites_textbox_idx, 10, 25);
	Draw_Text(35, 42, 0.45f, BLACK, msg1.c_str());
	C3D_FrameEnd(0);
}

bool confirmPopup(std::string msg) {
	return confirmPopup(msg, "", "Yes", "No", 100);
}

void drawButtonTesterScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(130, 0, 0.72f, WHITE, "Button Tester");
	Draw_Text(80, 218, 0.72f, WHITE, "Currently Pressed Button : ");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();
}

void buttonTesterLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE000");

	} else if (hDown & KEY_B) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE001");

	} else if (hDown & KEY_X) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE002");

	} else if (hDown & KEY_Y) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE003");

	} else if (hDown & KEY_UP) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE079");

		} else if (hDown & KEY_DOWN) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE07A");

		} else if (hDown & KEY_LEFT) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE07B");

		} else if (hDown & KEY_RIGHT) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE07C");

		} else if (hDown & KEY_L) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE052");

		} else if (hDown & KEY_R) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE053");

		} else if (hDown & KEY_ZL) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE054");

		} else if (hDown & KEY_ZR) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE055");

		} else if (hDown & KEY_START) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE045");

		} else if (hDown & KEY_SELECT) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE046");

	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, buttonTesterButtonPos[0])) {
			screenTransition(utilsScreen);
	}
}
}


static void drawGameSelection(void) {
	if (gameSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 30, 38, Config::barColor);
	} else if (gameSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 200, 38, Config::barColor);
	}
}


void drawGamesSubMenuScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text(130, 218, 0.72f, WHITE, "Games Tab");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Buttons.
	Gui::sprite(sprites_mainMenuButton_idx, gamesSubMenuButtonPos[0].x, gamesSubMenuButtonPos[0].y);
	Draw_Text(40, 57, 0.7f, WHITE, "Pong");

	Gui::sprite(sprites_mainMenuButton_idx, gamesSubMenuButtonPos[1].x, gamesSubMenuButtonPos[1].y);
	Draw_Text(210, 57, 0.65f, WHITE, "Tic-Tac-Toe");

	drawGameSelection();
}

static void gameSelectionLogic(u32 hDown) {
		if (hDown & KEY_UP) {
			if(gameSelection > 0)	gameSelection--;
		} else if (hDown & KEY_DOWN) {
			if(gameSelection < 1)	gameSelection++;
		} else if (hDown & KEY_A) {
			switch(gameSelection) {
				case 0: {
					screenTransition(pongScreen);
					break;
				} case 1:
					screenTransition(tictactoeScreen);
					break;
		}
		}
}

void gamesSubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	gameSelectionLogic(hDown);
	if (hDown & KEY_B) {
		screenMode = mainScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, gamesSubMenuButtonPos[0])) {
			screenTransition(pongScreen);
		} else if (touching(touch, gamesSubMenuButtonPos[1])) {
			screenTransition(tictactoeScreen);
		} else if (touching(touch, gamesSubMenuButtonPos[2])) {
			screenTransition(mainScreen);
		}
	}
}


static void drawUtilsSelection(void) {
	if (utilsSelection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 30, 38, Config::barColor);
	} else if (utilsSelection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 200, 38, Config::barColor);
	} else if (utilsSelection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 30, 148, Config::barColor);
	}
}


void drawUtilsScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(100, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text(130, 218, 0.72f, WHITE, "Utils Tab");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Buttons.
	Gui::sprite(sprites_mainMenuButton_idx, utilsButtonPos[0].x, utilsButtonPos[0].y);
	Gui::sprite(sprites_calendarIcon_idx, utilsButtonPos[0].x+5, utilsButtonPos[0].y+10);
	Draw_Text(40, 57, 0.7f, WHITE, "Calendar");

	Gui::sprite(sprites_mainMenuButton_idx, utilsButtonPos[1].x, utilsButtonPos[1].y);
	Gui::sprite(sprites_buttonIcon_idx, utilsButtonPos[1].x+5, utilsButtonPos[1].y+10);
	Draw_Text(210, 57, 0.65f, WHITE, "Btn Tester");

	Gui::sprite(sprites_mainMenuButton_idx, utilsButtonPos[2].x, utilsButtonPos[2].y);
	Draw_Text(37, 167, 0.65f, WHITE, "Calculator");

	drawUtilsSelection();
}

static void utilsSelectionLogic(u32 hDown) {
		if (hDown & KEY_UP) {
			if(utilsSelection > 0)	utilsSelection--;
		} else if (hDown & KEY_DOWN) {
			if(utilsSelection < 2)	utilsSelection++;
		} else if (hDown & KEY_A) {
			switch(utilsSelection) {
				case 0: {
					screenTransition(calendarScreen);
					break;
				} case 1:
					screenTransition(buttonTesterScreen);
					break;
				case 2: {
					screenTransition(calculatorScreen);
					break;
				}
		}
		}
}

void utilsLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	utilsSelectionLogic(hDown);
	if (hDown & KEY_B) {
		screenTransition(mainScreen);
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, utilsButtonPos[0])) {
			screenTransition(calendarScreen);
		} else if (touching(touch, utilsButtonPos[1])) {
			screenTransition(buttonTesterScreen);
		} else if (touching(touch, utilsButtonPos[2])) {
			screenTransition(calculatorScreen);
		} else if (touching(touch, utilsButtonPos[3])) {
			screenTransition(mainScreen);
		}
	}
}