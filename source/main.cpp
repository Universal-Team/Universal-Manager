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
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "graphic.h"
#include "settings.h"
#include "Textures.hpp"
#include "voltlib/volt.h"


#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

static touchPosition touch;

// 3D offsets. (0 == Left, 1 == Right)
Offset3D offset3D[2] = {0.0f, 0.0f};	

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

		void drawMainMenu (void) {
			volt_draw_on(GFX_TOP, GFX_LEFT);
			volt_draw_rectangle(0, 0, 400, 240, GRAY);
			volt_draw_rectangle(0, 0, 400, 25, BLACK);
			volt_draw_text(130, 4, 0.72f, 0.72f, WHITE, "Universal-Manager");
			volt_draw_on(GFX_BOTTOM, GFX_LEFT);
			volt_draw_rectangle(0, 0, 320, 240, GRAY);
			volt_draw_texture(MainMenuButton, 0, 40);
			volt_draw_texture(FileManagerIcon, 5, 50);
			volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "FileManager");

			volt_draw_texture(MainMenuButton, 170, 40);
			volt_draw_texture(FTPIcon, 175, 50);
			volt_draw_text(210, 57, 0.7f, 0.7f, BLACK, "FTP Client");

			volt_draw_texture(MainMenuButton, 0, 170);
			volt_draw_texture(UpdaterIcon, 5, 180);
			volt_draw_text(40, 187, 0.7f, 0.7f, BLACK, "Updater");

			volt_draw_texture(MainMenuButton, 170, 170);
			volt_draw_texture(SettingsIcon, 175, 180);
			volt_draw_text(210, 187, 0.7f, 0.7f, BLACK, "Settings");
			volt_end_draw();
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

	int fadealpha = 255;
	bool fadein = true;

	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		offset3D[0].topbg = CONFIG_3D_SLIDERSTATE * -7.0f;
		offset3D[1].topbg = CONFIG_3D_SLIDERSTATE * 7.0f;

		// Scan hid shared memory for input events
		hidScanInput();
		
		const u32 hDown = hidKeysDown();

		hidTouchRead(&touch);

		for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
			if (topfb == GFX_LEFT) volt_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
			else volt_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
			//volt_draw_texture(topbgtex, offset3D[topfb].topbg, 0);
			if (fadealpha > 0) volt_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		}
		drawMainMenu();
	}

	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}
