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

struct Key {
	std::string character;
	int x;
	int y;
};

// To-Do -> Make the positions correctly.
Key calculatorKeys[] = {

	// Numbers.
	{"1", 0, 0},
	{"2", 0, 0},
	{"3", 0, 0},
	{"4", 0, 40},
	{"5", 0, 40},
	{"6", 0, 40},
	{"7", 0, 80},
	{"8", 0, 80},
	{"9", 0, 80},
	{"0", 0, 120},
	{".", 0, 120},

	// Operations.
	{"+", 0, 40},
	{"-", 0, 40},
	{"/", 0, 40},
	{"*", 0, 40},
	{"=", 0, 40},
};

// To-Do.
static void drawCalculatorKeyboard(void) {
}


void drawCalculatorScreen(void) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	Draw_Text(125, 0, 0.72f, WHITE, "Calculator");
	C2D_SceneBegin(bottom);
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
	drawCalculatorKeyboard();
}

// To-Do -> Calculator Logic.
void calculatorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = utilsScreen;
	}
}