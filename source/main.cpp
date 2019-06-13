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
#include <citro3d.h>
#include <citro2d.h>
#include <3ds.h>
#include <algorithm>
#include <dirent.h>
#include <malloc.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "gui.hpp"
#include "screenCommon.hpp"

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

// Music and sound effects.
//sound *sfx_scroll = NULL;

static touchPosition touch;
extern C3D_RenderTarget* g_renderTargetTop;
extern C3D_RenderTarget* g_renderTargetBottom;	
int screenMode = 0;


ButtonPos mainScreenButtonPos[] = {
    {0, 40, 149, 52, fileScreen},
};

void screenoff()
{
    gspLcdInit();\
    GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTH);\
    gspLcdExit();
}

void screenon()
{
    gspLcdInit();\
    GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);\
    gspLcdExit();
}


/* void loadSoundEffects(void) {
	if (dspfirmfound) {
		sfx_scroll = new sound("romfs:/sfx/scroll.wav", 2, false);
	}
}*/

bool touching(touchPosition touch, ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}


int main()
{
	aptInit();
	amInit();
	sdmcInit();
	romfsInit();
	srvInit();
	hidInit();
	acInit();
    gfxInitDefault();
	Gui::init();

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Manager", 0777); // main Path.

	//loadSoundEffects();

	// Loop as long as the status is not exit
    while (aptMainLoop())
    {
        hidScanInput();
         u32 hHeld = hidKeysHeld();
         u32 hDown = hidKeysDown();
		 hidTouchRead(&touch);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(g_renderTargetTop, BLUE2);
        C2D_TargetClear(g_renderTargetBottom, BLUE2);

		// Draws a screen based on screenMode
		switch(screenMode) {
//#########################################################################################################
			case mainScreen:
				drawMainMenu();				// Draws the Main Menu screen
				break;
//#########################################################################################################
			case fileScreen:
				drawFileManagerSubMenu();	// Draws the File Manager screen
				break;
		}

				// Scans inputs for the current screen
		switch(screenMode) {
//#########################################################################################################
			case mainScreen:
				 if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(mainScreenButtonPos)/sizeof(mainScreenButtonPos[0]));i++) {
						if (touching(touch, mainScreenButtonPos[i])) {
							screenMode = mainScreenButtonPos[i].link;
						}
					}
				}
				break;
//#########################################################################################################
			case fileScreen:
				if (hDown & KEY_B) {
					screenMode = mainScreen;
				}
				break;
		}


        C3D_FrameEnd(0);
        Gui::clearTextBufs();
    }

	Gui::exit();
	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}
