/*
*   This file is part of Universal-Manager-DSi
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

#include "../graphics/graphics.h"

enum SCREEN_MODE {
		mainScreen = 0,
		fileScreen = 1
};

extern int SCREEN_MODE;

// Main Menu Screen.
void drawMainMenu(void);
void mainMenuLogic(u16 hDown);

// File Manager Sub Menu Screen.
void drawFileManagerSubMenu(void);
void fileManagerSubMenuLogic(u16 hDown);

// Graphic loading.
void loadGraphics(void);
extern std::vector<u16> battery0, battery25, battery50, battery75, battery100, batteryCharge, menuButton;
extern ImageData battery0Data, battery25Data, battery50Data, battery75Data, battery100Data, batteryChargeData, menuButtonData;
