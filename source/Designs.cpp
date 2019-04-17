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
    volt_set_screen_color(GFX_TOP, BLACK);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
}

void draw_Bars_Bottom_Black(void) {
    volt_draw_rectangle(0, 0, 320, 19, BLACK);
	volt_draw_rectangle(0, 221, 320, 19, BLACK);
}

void draw_Background_Bottom_Black(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, BLACK);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_White(void) {
	volt_draw_rectangle(0, 0, 400, 19, WHITE);
	volt_draw_rectangle(0, 221, 400, 19, WHITE);
}

void draw_Background_Top_White(void) {
    volt_set_screen_color(GFX_TOP, WHITE);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
}

void draw_Bars_Bottom_White(void) {
    volt_draw_rectangle(0, 0, 320, 19, WHITE);
	volt_draw_rectangle(0, 221, 320, 19, WHITE);
}

void draw_Background_Bottom_White(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, WHITE);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Blue(void) {
	volt_draw_rectangle(0, 0, 400, 19, BLUE);
	volt_draw_rectangle(0, 221, 400, 19, BLUE);
}

void draw_Background_Top_Blue(void) {
    volt_set_screen_color(GFX_TOP, BLUE);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
}

void draw_Bars_Bottom_Blue(void) {
    volt_draw_rectangle(0, 0, 320, 19, BLUE);
	volt_draw_rectangle(0, 221, 320, 19, BLUE);
}

void draw_Background_Bottom_Blue(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, BLUE);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void draw_Bars_Top_Red(void) {
	volt_draw_rectangle(0, 0, 400, 19, RED);
	volt_draw_rectangle(0, 221, 400, 19, RED);
}

void draw_Background_Top_Red(void) {
    volt_set_screen_color(GFX_TOP, RED);
    volt_draw_on(GFX_TOP, GFX_LEFT); 
}

void draw_Bars_Bottom_Red(void) {
    volt_draw_rectangle(0, 0, 320, 19, RED);
	volt_draw_rectangle(0, 221, 320, 19, RED);
}

void draw_Background_Bottom_Red(void) {
    volt_draw_on(GFX_BOTTOM, GFX_LEFT); // Draw on Bottom Screen.
    volt_set_screen_color(GFX_BOTTOM, RED);
}
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////