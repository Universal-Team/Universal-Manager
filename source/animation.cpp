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
#include "graphic.h"
#include "textures.hpp"
#include "voltlib/volt.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

static int animated_bubblesYPos[2] = {0};
static bool animated_bubbleMoveDelay = false;

size_t animatedTextureTop[2] = {47, 48};
size_t animatedTextureBottom[2] = {49, 50};


void animation_Init(void) {
    volt_load_texture_png(animatedTextureTop[0], "romfs:/graphics/animated/Bubbles_Top_1.png");
	volt_load_texture_png(animatedTextureTop[1], "romfs:/graphics/animated/Bubbles_Top_2.png");

	volt_load_texture_png(animatedTextureBottom[0], "romfs:/graphics/animated/Bubbles_Bottom_1.png");
	volt_load_texture_png(animatedTextureBottom[1], "romfs:/graphics/animated/Bubbles_Bottom_2.png");
}

void animatedBGTop(void) {
		if (settings.universal.animation == 0) {
	} else if (settings.universal.animation == 1) {
		if (!animated_bubbleMoveDelay) {
		animated_bubblesYPos[0]--;
		if (animated_bubblesYPos[0] <= -240) animated_bubblesYPos[0] = 0;
	}
	animated_bubbleMoveDelay = !animated_bubbleMoveDelay;

	animated_bubblesYPos[1]--;
	if (animated_bubblesYPos[1] <= -240) animated_bubblesYPos[1] = 0;

	volt_draw_texture_blend(animatedTextureTop[0], 0, animated_bubblesYPos[0], settings.universal.color);
	volt_draw_texture_blend(animatedTextureTop[0], 0, animated_bubblesYPos[0]+240, settings.universal.color);
	volt_draw_texture_blend(animatedTextureTop[1], 0, animated_bubblesYPos[1], settings.universal.color);
	volt_draw_texture_blend(animatedTextureTop[1], 0, animated_bubblesYPos[1]+240, settings.universal.color);
}
}

void animatedBGBot(void) {
	if (settings.universal.animation == 0) {
	} else if (settings.universal.animation == 1) {
		if (!animated_bubbleMoveDelay) {
		animated_bubblesYPos[0]--;
		if (animated_bubblesYPos[0] <= -240) animated_bubblesYPos[0] = 0;
	}
	animated_bubbleMoveDelay = !animated_bubbleMoveDelay;

	animated_bubblesYPos[1]--;
	if (animated_bubblesYPos[1] <= -240) animated_bubblesYPos[1] = 0;

	volt_draw_texture_blend(animatedTextureBottom[0], 0, animated_bubblesYPos[0], settings.universal.color);
	volt_draw_texture_blend(animatedTextureBottom[0], 0, animated_bubblesYPos[0]+240, settings.universal.color);
	volt_draw_texture_blend(animatedTextureBottom[1], 0, animated_bubblesYPos[1], settings.universal.color);
	volt_draw_texture_blend(animatedTextureBottom[1], 0, animated_bubblesYPos[1]+240, settings.universal.color);
}
}

/**
 * Make the dialog box appear.				// Ported from TWLoader.
 * @param text Dialog box text.
 */
/*void DialogBoxAppear(int x, int y, const char *text) {
	if (showdialogbox)
		return;

	// Save the dialog text so we can make
	// use if it if nullptr is specified.
	if (text) {
		dialog_text = text;
	}

	int movespeed = 22;
	for (int i = 0; i < 240; i += movespeed) {
		if (movespeed <= 1) {
			movespeed = 1;
		} else {
			movespeed -= 0.2;
		}
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		if (screenmode == SCREEN_MODE_SETTINGS) {
			pp2d_draw_texture(settingstex, 0, 0);
		}
		volt_draw_texture(dialogboxtex, 0, i-240);			
		volt_draw_text(x, y+i-240, 0.5f, 0.5f, BLACK, dialog_text.c_str());
		volt_end_draw();
	}
	showdialogbox = true;
}*/

/**
 * Make the dialog box disappear.
 * @param text Dialog box text.
 */
/*void DialogBoxDisappear(int x, int y, const char *text) {
	if (!showdialogbox)
		return;

	// Save the dialog text so we can make
	// use if it if nullptr is specified.
	if (text) {
		dialog_text = text;
	}

	int movespeed = 1;
	for (int i = 0; i < 240; i += movespeed) {
		movespeed += 1;
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		if (screenmode == SCREEN_MODE_SETTINGS) {
			volt_draw_texture(settingstex, 0, 0);
		}
		volt_draw_texture(dialogboxtex, 0, i);
		volt_draw_text(x, y+i, 0.5f, 0.5f, BLACK, dialog_text.c_str());
		volt_end_draw();
	}
	showdialogbox = false;
}*/