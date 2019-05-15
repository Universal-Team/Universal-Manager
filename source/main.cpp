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

#include "dumpdsp.hpp"
#include "graphic.h"
#include "screens/screenCommon.hpp"
#include "settings.h"
#include "sfx.hpp"
#include "textures.hpp"
#include "voltlib/volt.h"

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

// Music and sound effects.
sound *sfx_example = NULL;

// 3D offsets. (0 == Left, 1 == Right)
Offset3D offset3D[2] = {0.0f, 0.0f};	

int screenMode = 0;
bool dspfirmfound = false;
static touchPosition touch;

ButtonPos mainScreenButtonPos[] = {
    {0, 40, 149, 52, fileScreen},
    {170, 40, 149, 52, creditsScreen},
    {0, 170, 149, 52, updaterScreen},
    {170, 170, 149, 52, settingsScreen},
};

ButtonPos fileScreenButtonPos[] = {
    {100, 40, 149, 52, musicPlayerScreen},
	{100, 120, 149, 52, imageScreen},
    {288, 208, 32, 32, mainScreen},
};

ButtonPos settingsScreenButtonPos[] = {
    {0, 40, 149, 52, creditsScreen},
    {288, 208, 32, 32, mainScreen},
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

void displayMsg(const char* text) {
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	volt_draw_rectangle(0, 25, 400, 215, BLACK);
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, text);
	volt_end_draw();
}

void notImplemented(void) {
	displayMsg("Not implemented Yet.\n");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

void loadSoundEffects(void) {
	if (dspfirmfound) {
		sfx_example = new sound("romfs:/sfx/example.wav", 2, false);
	}
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

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	volt_init();

	volt_set_3D(1);

	graphicsInit();

	if (access("sdmc:/3ds/dspfirm.cdc", F_OK) != -1) {
		ndspInit();
		dspfirmfound = true;
	} else {
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "Dumping DSP firm...");
		volt_end_draw();
		dumpDsp();
		if (access("sdmc:/3ds/dspfirm.cdc", F_OK) != -1) {
			ndspInit();
			dspfirmfound = true;
		} else {
			for (int i = 0; i < 90; i++) {
				volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
				volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "DSP firm dumping failed.\n"
						"Running without sound.");
				volt_end_draw();
			}	
		}
	}

	loadSoundEffects();

	int fadealpha = 255;

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		offset3D[0].topbg = CONFIG_3D_SLIDERSTATE * -7.0f;
		offset3D[1].topbg = CONFIG_3D_SLIDERSTATE * 7.0f;

		// Scan hid shared memory for input events
		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		hidTouchRead(&touch);

		for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
			if (topfb == GFX_LEFT) volt_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
			else volt_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
			if (fadealpha > 0) volt_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		}

		// Draws a screen based on screenMode
		switch(screenMode) {
			case mainScreen:
				drawMainMenu();				// Draws the Main Menu screen
				break;
			case fileScreen:
				drawFileManagerSubMenu();	// Draws the File Manager screen
				break;
			case creditsScreen:
				drawCredits();				// Draws the Credits screen
				break;
			case updaterScreen:
				drawUpdaterScreen();		// Draws the Updater screen
				break;
			case musicPlayerScreen:
				drawMusicPlayerUI();		// Draws the Music Player menu screen
				break;
			case musicPlayScreen:
				drawMusicPlay();			// Draws the Music Player play screen
				break;
			case musicPauseScreen:
				drawMusicPause();			// Draws the Music Player pause screen
				break;
			case settingsScreen:
				drawSettingsScreen();		// Draws the Settings Screen
				break;
			case imageScreen:
				drawImageViewerUI();
				break;
		}

		// Scans inputs for the current screen
		switch(screenMode) {
			case mainScreen:
				if (hDown & KEY_A) {
					screenMode = fileScreen;
				} else if (hDown & KEY_X) {
					screenMode = creditsScreen;
				} else if (hDown & KEY_Y) {
					screenMode = updaterScreen;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(mainScreenButtonPos)/sizeof(mainScreenButtonPos[0]));i++) {
						if (touch.px >= mainScreenButtonPos[i].x && touch.px <= (mainScreenButtonPos[i].x + mainScreenButtonPos[i].w) && touch.py >= mainScreenButtonPos[i].y && touch.py <= (mainScreenButtonPos[i].y + mainScreenButtonPos[i].h)) {
							screenMode = mainScreenButtonPos[i].link;
						}
					}
				}
				break;
			case fileScreen:
				if (hDown & KEY_B) {
					screenMode = mainScreen;
				} else if (hDown & KEY_A) {
					screenMode = musicPlayerScreen;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(fileScreenButtonPos)/sizeof(fileScreenButtonPos[0]));i++) {
						if (touch.px >= fileScreenButtonPos[i].x && touch.px <= (fileScreenButtonPos[i].x + fileScreenButtonPos[i].w) && touch.py >= fileScreenButtonPos[i].y && touch.py <= (fileScreenButtonPos[i].y + fileScreenButtonPos[i].h)) {
							screenMode = fileScreenButtonPos[i].link;
						}
					}
				}
				break;
			case creditsScreen:
			if (hDown & KEY_B) {
				screenMode = settingsScreen;
				break;
			}
			case updaterScreen:
				if (hDown & KEY_B) {
					screenMode = mainScreen;
				}
				break;
			case musicPlayerScreen:
				if (hDown & KEY_X) {
					screenMode = fileScreen;
				}
				break;
			case musicPlayScreen:
			case musicPauseScreen:
				if (hDown & KEY_B) {
					screenMode = musicPlayerScreen;
				}
				break;
			case settingsScreen:
			if (hDown & KEY_B) {
				screenMode = mainScreen;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(settingsScreenButtonPos)/sizeof(settingsScreenButtonPos[0]));i++) {
						if (touch.px >= settingsScreenButtonPos[i].x && touch.px <= (settingsScreenButtonPos[i].x + settingsScreenButtonPos[i].w) && touch.py >= settingsScreenButtonPos[i].y && touch.py <= (settingsScreenButtonPos[i].y + settingsScreenButtonPos[i].h)) {
							screenMode = settingsScreenButtonPos[i].link;
						}
					}
				}
				break;
			}
			}
	
	delete sfx_example;
	if (dspfirmfound) {
		ndspExit();
	}
	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}
