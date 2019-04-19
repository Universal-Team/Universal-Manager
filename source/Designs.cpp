/*
*   This file is part of Universal-Updater
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

#include "Colors.hpp"
#include "graphic.h"
#include "voltlib/volt.h"
#include "utils/datetime.h"
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void draw_Bars_Top_Black(void) {
	volt_draw_rectangle(0, 0, 400, 19, BLACK);
	volt_draw_rectangle(0, 221, 400, 19, BLACK);
}

void draw_Background_Top_Black(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, BLACK);
}

void draw_Bars_Bottom_Black(void) {
    volt_draw_rectangle(0, 0, 320, 19, BLACK);
	volt_draw_rectangle(0, 221, 320, 19, BLACK);
}

void draw_Background_Bottom_Black(void) {
     volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
     //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
     volt_draw_rectangle(0, 0, 320, 240, BLACK);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_White(void) {
	volt_draw_rectangle(0, 0, 400, 19, WHITE);
	volt_draw_rectangle(0, 221, 400, 19, WHITE);
}

void draw_Background_Top_White(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, WHITE);
}

void draw_Bars_Bottom_White(void) {
    volt_draw_rectangle(0, 0, 320, 19, WHITE);
	volt_draw_rectangle(0, 221, 320, 19, WHITE);
}

void draw_Background_Bottom_White(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
    volt_draw_rectangle(0, 0, 320, 240, WHITE);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Blue(void) {
	volt_draw_rectangle(0, 0, 400, 19, BLUE);
	volt_draw_rectangle(0, 221, 400, 19, BLUE);
}

void draw_Background_Top_Blue(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, BLUE);
}

void draw_Bars_Bottom_Blue(void) {
    volt_draw_rectangle(0, 0, 320, 19, BLUE);
	volt_draw_rectangle(0, 221, 320, 19, BLUE);
}

void draw_Background_Bottom_Blue(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
    volt_draw_rectangle(0, 0, 320, 240, BLUE);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Red(void) {
	volt_draw_rectangle(0, 0, 400, 19, RED);
	volt_draw_rectangle(0, 221, 400, 19, RED);
}

void draw_Background_Top_Red(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, RED);
}

void draw_Bars_Bottom_Red(void) {
    volt_draw_rectangle(0, 0, 320, 19, RED);
	volt_draw_rectangle(0, 221, 320, 19, RED);
}

void draw_Background_Bottom_Red(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
    volt_draw_rectangle(0, 0, 320, 240, RED);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Yellow(void) {
	volt_draw_rectangle(0, 0, 400, 19, YELLOW);
	volt_draw_rectangle(0, 221, 400, 19, YELLOW);
}

void draw_Background_Top_Yellow(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, YELLOW);
}

void draw_Bars_Bottom_Yellow(void) {
    volt_draw_rectangle(0, 0, 320, 19, YELLOW);
	volt_draw_rectangle(0, 221, 320, 19, YELLOW);
}

void draw_Background_Bottom_Yellow(void) {
     volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
     //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
     volt_draw_rectangle(0, 0, 320, 240, YELLOW);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Gray(void) {
	volt_draw_rectangle(0, 0, 400, 19, GRAY);
	volt_draw_rectangle(0, 221, 400, 19, GRAY);
}

void draw_Background_Top_Gray(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, GRAY);
}

void draw_Bars_Bottom_Gray(void) {
    volt_draw_rectangle(0, 0, 320, 19, GRAY);
	volt_draw_rectangle(0, 221, 320, 19, GRAY);
}

void draw_Background_Bottom_Gray(void) {
     volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
     //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
     volt_draw_rectangle(0, 0, 320, 240, GRAY);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Light_Brown(void) {
	volt_draw_rectangle(0, 0, 400, 19, LIGHT_BROWN);
	volt_draw_rectangle(0, 221, 400, 19, LIGHT_BROWN);
}

void draw_Background_Top_Light_Brown(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, LIGHT_BROWN);
}

void draw_Bars_Bottom_Light_Brown(void) {
    volt_draw_rectangle(0, 0, 320, 19, LIGHT_BROWN);
	volt_draw_rectangle(0, 221, 320, 19, LIGHT_BROWN);
}

void draw_Background_Bottom_Light_Brown(void) {
     volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
     //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
     volt_draw_rectangle(0, 0, 320, 240, LIGHT_BROWN);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_LIGHT_GREEN(void) {
	volt_draw_rectangle(0, 0, 400, 19, LIGHT_GREEN);
	volt_draw_rectangle(0, 221, 400, 19, LIGHT_GREEN);
}

void draw_Background_Top_LIGHT_GREEN(void) {
    volt_draw_on(GFX_TOP, GFX_LEFT); 
    //volt_set_screen_color(GFX_TOP, TRANSPARENT);
    volt_draw_rectangle(0, 0, 400, 240, LIGHT_GREEN);
}

void draw_Bars_Bottom_LIGHT_GREEN(void) {
    volt_draw_rectangle(0, 0, 320, 19, LIGHT_GREEN);
	volt_draw_rectangle(0, 221, 320, 19, LIGHT_GREEN);
}

void draw_Background_Bottom_LIGHT_GREEN(void) {
     volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
     //volt_set_screen_color(GFX_BOTTOM, TRANSPARENT);
     volt_draw_rectangle(0, 0, 320, 240, LIGHT_GREEN);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////