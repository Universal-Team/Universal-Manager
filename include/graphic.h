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

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <3ds.h>
#include "voltlib/volt.h"
#include "colors.hpp"
#include "textures.hpp"

// Text Sizes.

#define BIG_SIZE 0.72f
#define BUTTON_TEXT 0.7f

void volt_draw_texture_scale_blend(size_t id, int x, int y, float scaleX, float scaleY, u32 color);
void volt_draw_texture_part_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, u32 color);
void volt_draw_texture_part_scale(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY);
void volt_draw_texture_part_scale_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY, u32 color);
void volt_draw_texture_rotate_flip_blend(size_t id, int x, int y, float angle, flipType fliptype, u32 color);
void displayMsg(const char* text);
void graphicsInit(void);

void drawBarsTopLight(void);

void drawBarsTopDark(void);

void drawBarsTopRed(void);

void drawBgTop(void);

void drawBarsBotLight(void);

void drawBarsBotDark(void);

void drawBarsBotRed(void);

void drawBgBot(void);

//void volt_free_texture(size_t id);
#endif // GRAPHIC_H