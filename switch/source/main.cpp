/*
*   This file is part of Universal-Manager-NX
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


#include "colors.hpp"
#include "gui.hpp"
#include "logging.hpp"

#include "screens/fileManagerScreen.hpp"
#include "screens/mainMenuScreen.hpp"
#include "screens/screenCommon.hpp"

#include <algorithm>
#include <dirent.h>
#include <malloc.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <switch.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

extern "C" {
    #include "touch_helper.h"
}

int fadealpha = 255;
bool fadein = true;

static Result servicesInit(void)
{
    Result res = 0;
    romfsInit();
    mkdir("sdmc:/switch", 0777);
    mkdir("sdmc:/switch/Universal-Manager", 0777);
    Logging::createLogFile();

    Gui::Init();
	Textures_Load();
    Logging::writeToLog("Universal-Manager launched Successfully!");

    return 0;
}

static void servicesExit(void)
{
//    nsExit();
	Gui::Exit();
//    plExit();
    romfsExit();
    Textures_Free();
}

int main(void)
{
    Result res = servicesInit();
    if (R_FAILED(res)) {
        servicesExit();
        return res;
    }
    
	    TouchInfo touchInfo;
	    Touch_Init(&touchInfo);
        Gui::setScreen(std::make_unique<MainMenu>());

        while (appletMainLoop() && !(hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS)) {
        hidScanInput();
        u64 hDown = hidKeysDown(CONTROLLER_P1_AUTO);

        Gui::ClearScreen(BLACK);

		Gui::mainLoop(hDown);

        Gui::RenderScreen();

		if (fadein == true) {
			fadealpha -= 3;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}
        }

    servicesExit();
    return 0;
}