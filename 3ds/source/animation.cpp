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

#include "animation.hpp"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

static int animated_bubblesYPos[2] = {0};
static bool animated_bubbleMoveDelay = false;

void animatedBGTop(void) {
	if (Config::animation == 0) {
	} else if (Config::animation == 1) {
		if (!animated_bubbleMoveDelay) {
		animated_bubblesYPos[0]--;
		if (animated_bubblesYPos[0] <= -240) animated_bubblesYPos[0] = 0;
	}
	animated_bubbleMoveDelay = !animated_bubbleMoveDelay;

	animated_bubblesYPos[1]--;
	if (animated_bubblesYPos[1] <= -240) animated_bubblesYPos[1] = 0;

	Gui::Draw_ImageBlend2(animation_bubbles_top_1_idx, 0, animated_bubblesYPos[0], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bubbles_top_1_idx, 0, animated_bubblesYPos[0]+240, Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bubbles_top_2_idx, 0, animated_bubblesYPos[1], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bubbles_top_2_idx, 0, animated_bubblesYPos[1]+240, Config::animationColor);
} else if (Config::animation == 2) {
			if (!animated_bubbleMoveDelay) {
		animated_bubblesYPos[0]--;
		if (animated_bubblesYPos[0] <= -240) animated_bubblesYPos[0] = 0;
	}
	animated_bubbleMoveDelay = !animated_bubbleMoveDelay;

	animated_bubblesYPos[1]--;
	if (animated_bubblesYPos[1] <= -240) animated_bubblesYPos[1] = 0;

	Gui::Draw_ImageBlend2(animation_topscreenV1_idx, 0, animated_bubblesYPos[0], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_topscreenV1_idx, 0, animated_bubblesYPos[0]+240, Config::animationColor);
	Gui::Draw_ImageBlend2(animation_topscreen2V1_idx, 0, animated_bubblesYPos[1], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_topscreen2V1_idx, 0, animated_bubblesYPos[1]+240, Config::animationColor);
}
}

void animatedBGBot(void) {
	if (Config::animation == 0) {
	} else if (Config::animation == 1) {
		if (!animated_bubbleMoveDelay) {
		animated_bubblesYPos[0]--;
		if (animated_bubblesYPos[0] <= -240) animated_bubblesYPos[0] = 0;
	}
	animated_bubbleMoveDelay = !animated_bubbleMoveDelay;

	animated_bubblesYPos[1]--;
	if (animated_bubblesYPos[1] <= -240) animated_bubblesYPos[1] = 0;

	Gui::Draw_ImageBlend2(animation_bubbles_bottom_1_idx, 0, animated_bubblesYPos[0], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bubbles_bottom_1_idx, 0, animated_bubblesYPos[0]+240, Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bubbles_bottom_2_idx, 0, animated_bubblesYPos[1], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bubbles_bottom_2_idx, 0, animated_bubblesYPos[1]+240, Config::animationColor);
} else if (Config::animation == 2) {
			if (!animated_bubbleMoveDelay) {
		animated_bubblesYPos[0]--;
		if (animated_bubblesYPos[0] <= -240) animated_bubblesYPos[0] = 0;
	}
	animated_bubbleMoveDelay = !animated_bubbleMoveDelay;

	animated_bubblesYPos[1]--;
	if (animated_bubblesYPos[1] <= -240) animated_bubblesYPos[1] = 0;

	Gui::Draw_ImageBlend2(animation_bottomscreenV1_idx, 0, animated_bubblesYPos[0], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bottomscreenV1_idx, 0, animated_bubblesYPos[0]+240, Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bottomscreen2V1_idx, 0, animated_bubblesYPos[1], Config::animationColor);
	Gui::Draw_ImageBlend2(animation_bottomscreen2V1_idx, 0, animated_bubblesYPos[1]+240, Config::animationColor);
}
}