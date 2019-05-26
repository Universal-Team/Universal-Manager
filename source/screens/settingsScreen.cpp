#include "screens/screenCommon.hpp"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include <sstream>

#include "keyboard.h"
#include "universal-settings.hpp"

struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

extern bool touching(touchPosition touch, ButtonPos button);


ButtonPos uiSettingsButtonPos[] = {
	// Bars
	
	{46, 98, 87, 33, -1},
	{129, 98, 87, 33, -1},
	{220, 98, 87, 33, -1},
	// Background

	{46, 168, 87, 33, -1},
	{129, 168, 87, 33, -1},
	{220, 168, 87, 33, -1},

	{288, 208, 32, 32, settingsScreen},

	// Music BG
	{220, 28, 87, 33, -1},
};

int getColorValue(int color, int bgr) {
	char colorName[10];
	int i;
	std::stringstream ss;

	itoa(color, colorName, 16);
	std::string colorNamePart(colorName, 2*bgr+2, 2);
	ss << std::hex << colorNamePart.c_str();
	ss >> i;

	return i;
}

std::string getColorName(int color, int bgr) {
	char colorName[10];
	int i = getColorValue(color, bgr);
	itoa(i, colorName, 10);
	return colorName;
}

void drawSettingsScreen(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Settings");

	drawBgBot();
	drawBarsBot();

	volt_draw_texture(MainMenuButton, 0, 40);
	volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Credits");

	volt_draw_texture(MainMenuButton, 170, 40);
	volt_draw_text(200, 57, 0.7f, 0.7f, BLACK, "UI Settings");

	volt_draw_texture(BackIcon, 288, 208);
	volt_end_draw();
}

void drawUISettingsScreen(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "UI Settings");
	drawBgBot();
	drawBarsBot();

	// Bars.
	volt_draw_text(120, 58, 0.7f, 0.7f, BLACK, "Bars");
	volt_draw_texture(UpdaterButton, 35, 88);
	volt_draw_text(46, 98, 0.7f, 0.7f, BLACK, "Red");
	volt_draw_texture(UpdaterButton, 129, 88);
	volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, "Green");
	volt_draw_texture(UpdaterButton, 220, 88);
	volt_draw_text(229, 98, 0.7f, 0.7f, BLACK, "Blue");

	// Background.
	volt_draw_text(120, 138, 0.7f, 0.7f, BLACK, "Background");
	volt_draw_texture(UpdaterButton, 35, 168);
	volt_draw_text(46, 178, 0.7f, 0.7f, BLACK, "Red");
	volt_draw_texture(UpdaterButton, 129, 168);
	volt_draw_text(140, 178, 0.7f, 0.7f, BLACK, "Green");
	volt_draw_texture(UpdaterButton, 220, 168);
	volt_draw_text(229, 178, 0.7f, 0.7f, BLACK, "Blue");

	volt_draw_texture(UpdaterButton, 220, 28);
	volt_draw_text(229, 38, 0.7f, 0.7f, BLACK, "Music");

	// Maybe Text Color later? 

	volt_draw_texture(BackIcon, 288, 208);
	volt_end_draw();
}

void uiSettingsLogic(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, uiSettingsButtonPos[0])) {
			red = keyboardInputInt("Red");
			settings.universal.bars = RGBA8(red, getColorValue(settings.universal.bars, 1), getColorValue(settings.universal.bars, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[1])) {
			green = keyboardInputInt("Green (0-255)");
			settings.universal.bars = RGBA8(getColorValue(settings.universal.bars, 2), green, getColorValue(settings.universal.bars, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[2])) {
			blue = keyboardInputInt("Blue (0-255)");
			settings.universal.bars = RGBA8(getColorValue(settings.universal.bars, 2), getColorValue(settings.universal.bars, 1), blue, 255);
		} else if (touching(touch, uiSettingsButtonPos[3])) {
			red = keyboardInputInt("Red");
			settings.universal.bg = RGBA8(red, getColorValue(settings.universal.bg, 1), getColorValue(settings.universal.bg, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[4])) {
			green = keyboardInputInt("Green (0-255)");
			settings.universal.bg = RGBA8(getColorValue(settings.universal.bg, 2), green, getColorValue(settings.universal.bg, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[5])) {
			blue = keyboardInputInt("Blue (0-255)");
			settings.universal.bg = RGBA8(getColorValue(settings.universal.bg, 2), getColorValue(settings.universal.bg, 1), blue, 255);
		} else if (touching(touch, uiSettingsButtonPos[6])) {
			screenMode = uiSettingsButtonPos[6].link;
		} else if (touching(touch, uiSettingsButtonPos[7])) {
			settings.universal.music++;
			if (settings.universal.music > 2) settings.universal.music = 0;
		}
	}
}