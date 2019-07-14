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
#include <string>
#include <unistd.h>
#include <vector>
#include "sound.h"
//#include "keyboard.hpp" // Maybe for the Future?

void drawPongScreen(void) {
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	Draw_Text(0, 0, 0.72f, WHITE, "Score : 0");
	Draw_Text(300, 0, 0.72f, WHITE, "Score : 0");

	// Draw Paddle + ball.
	Gui::Draw_ImageBlend(sprites_paddle_idx, 0, 25, Config::barColor);
	Gui::Draw_ImageBlend(sprites_paddle_idx, 390, 60, Config::barColor);
	Gui::Draw_ImageBlend(sprites_ball_idx, 200, 100, Config::barColor);

	Gui::DrawBGBot();
	Gui::DrawBarsBot();
}

// To-Do -> Do the whole Pong Logic inside it.
void pongLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		screenMode = mainScreen;
	} else if (hDown & KEY_A) {
		playPongSfx();
	} else if (hDown & KEY_X) {
		playScoreSfx();
	}
}