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

#include <stdio.h>
#include <stdlib.h>
#include "screens/screenCommon.hpp"

extern "C" {
	#include "textures.h"
}

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

extern bool touching(touchPosition touch, ButtonPos button);
extern SDL_Texture *MainMenuButton;
// Version numbers.
char universal_manager_vertext[13];

void drawMainMenu(void) {
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	SDL_DrawRect(0, 0, 1280, 720, GRAY);
	SDL_DrawRect(0, 0, 1280, 100, BARCOLOR);
	SDL_DrawRect(0, 620, 1280, 100, BARCOLOR);
	SDL_DrawText(250, 0, 72, WHITE, "Universal-Manager-NX");
	SDL_DrawText(1020, 630, 72, WHITE, universal_manager_vertext);

	// Buttons.
	SDL_DrawImage(MainMenuButton, 40, 150);
	SDL_DrawImage(MainMenuButton, 40, 300);
	SDL_DrawImage(MainMenuButton, 40, 450);
}

void MainMenuLogic(u64 hDown, touchPosition touch) {
	if (hDown & KEY_A) {
	}
}