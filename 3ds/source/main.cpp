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
#include "ptmu_x.h"
#include "mainMenuScreen.hpp"
#include "settings.hpp"

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

bool dspfirmfound = false;

touchPosition touch;

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;


bool touching(touchPosition touch, ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}



int main()
{
	acInit();
	amInit();
	ptmuInit();	// For battery status
	ptmuxInit();	// For AC adapter status
	sdmcInit();
	Config::loadConfig();
	if (Config::Citra == 0) {
	mcuInit();
	} else if (Config::Citra == 1) {
	}
	romfsInit();
	cfguInit();
    gfxInitDefault();
	Gui::init();
	srand(time(NULL));

//	if (Config::Credits == 0) { // Credits Screen if 1 and mainScreen if 0.
//		SCREEN_MODE = 0;
//	} else if (Config::Credits == 1) {
//		SCREEN_MODE = 1;
//	}

	Gui::setScreen(std::make_unique<MainMenu>());

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users


	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Manager", 0777); // main Path.

 	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	 }

	// Loop as long as the status is not exit
    while (aptMainLoop())
    {
        hidScanInput();
        u32 hHeld = hidKeysHeld();
        u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, BLACK);
        C2D_TargetClear(bottom, BLACK);
		Gui::clearTextBufs();
		Gui::mainLoop(hDown, hHeld, touch);

	}

	if (dspfirmfound == true)
	{
		ndspExit();
	}
	Gui::exit();
	gfxExit();
	cfguExit();
	romfsExit();
	sdmcExit();
	acExit();
	amExit();

    return 0;
}
