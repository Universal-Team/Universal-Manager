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
std::string musicModes[] = {"DEFAULT", "COVER", "BG"};
std::string animationModes[] = {"Disabled", "Enabled"};
std::string percentModes[] = {"Hidden", "Shown"};


ButtonPos uiSettingsButtonPos[] = {
	// Bars
	
	{46, 98, 87, 33, -1},
	{129, 98, 87, 33, -1},
	{220, 98, 87, 33, -1},
	// Background

	{46, 168, 87, 33, -1},
	{129, 168, 87, 33, -1},
	{220, 168, 87, 33, -1},

	{293, 213, 27, 27, settingsScreen},

	// Music BG
	{220, 28, 87, 33, -1},

	// Saving.
	{10, 28, 87, 33, -1},

	// Bubble Colors.
	{46, 98, 87, 33, -1},
	{129, 98, 87, 33, -1},
	{220, 98, 87, 33, -1},

	// Animation Mode.
	{220, 28, 87, 33, -1},

	// Percent Mode.
	{220, 168, 87, 33, -1},
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
	animatedBGTop();
	drawBarsTop();
	displayTime();
	volt_draw_text_center(GFX_TOP, 3, 0.72f, 0.72f, WHITE, "Settings");
	drawBatteryTop();

	drawBgBot();
	animatedBGBot();
	drawBarsBotBack();

	volt_draw_texture(MainMenuButton, 0, 40);
	volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Credits");

	volt_draw_texture(MainMenuButton, 170, 40);
	volt_draw_text(200, 57, 0.7f, 0.7f, BLACK, "UI Settings");
	volt_end_draw();
}

void drawUISettingsScreen(void) {
	drawBgTop();
	animatedBGTop();
	drawBarsTop();
	displayTime();
	volt_draw_text_center(GFX_TOP, 3, 0.72f, 0.72f, WHITE, "UI Settings");
	drawBatteryTop();
	drawBgBot();
	animatedBGBot();
	drawBarsBotBack();

	// Bars.
	volt_draw_text(120, 58, 0.7f, 0.7f, BLACK, "Bars");
	volt_draw_texture(RedButton, 35, 88);
	volt_draw_text(46, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bars, 2).c_str());
	volt_draw_texture(GreenButton, 129, 88);
	volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bars, 1).c_str());
	volt_draw_texture(BlueButton, 220, 88);
	volt_draw_text(229, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bars, 0).c_str());

	// Background.
	volt_draw_text(120, 138, 0.7f, 0.7f, BLACK, "Background");
	volt_draw_texture(RedButton, 35, 168);
	volt_draw_text(46, 178, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bg, 2).c_str());
	volt_draw_texture(GreenButton, 129, 168);
	volt_draw_text(140, 178, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bg, 1).c_str());
	volt_draw_texture(BlueButton, 220, 168);
	volt_draw_text(229, 178, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bg, 0).c_str());

	volt_draw_texture(UpdaterButton, 220, 28);
	volt_draw_text(229, 38, 0.65f, 0.65f, BLACK, musicModes[settings.universal.music].c_str());
	volt_draw_text(110, 38, 0.7f, 0.7f, BLACK, "Music Mode:");

	// Save Settings. ;P
	volt_draw_texture(UpdaterButton, 10, 28);
	volt_draw_text(19, 38, 0.7f, 0.7f, BLACK, "Save");

	// Maybe Text Color later? 
	volt_end_draw();
}

/*void drawUISettingsScreen2(void) {
	drawBgTop();
	animatedBGTop();
	drawBarsTop();
	displayTime();
	volt_draw_text_center(GFX_TOP, 3, 0.72f, 0.72f, WHITE, "UI Settings Page 2");
	drawBatteryTop();
	drawBgBot();
	animatedBGBot();
	drawBarsBotBack();

		// Bubbles.
	//volt_draw_text(120, 58, 0.7f, 0.7f, BLACK, "Bubble");
	volt_draw_texture_blend(RGBButton, 35, 88, RED);
	//volt_draw_text(46, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.color, 2).c_str());
	volt_draw_texture_blend(RGBButton, 129, 88, GREEN);
	//volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.color, 1).c_str());
	volt_draw_texture_blend(RGBButton, 220, 88, BLUE);
	//volt_draw_text(229, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.color, 0).c_str());

	volt_draw_texture(UpdaterButton, 220, 28);
	//volt_draw_text(229, 38, 0.65f, 0.65f, BLACK, animationModes[settings.universal.animation].c_str());
	volt_draw_text(110, 38, 0.7f, 0.7f, BLACK, "Animation :");

	volt_draw_texture(UpdaterButton, 220, 168);
	//volt_draw_text(229, 178, 0.65f, 0.65f, BLACK, percentModes[settings.universal.battery].c_str());
	volt_draw_text(110, 178, 0.7f, 0.7f, BLACK, "Percentage :");
}*/

void uiSettingsLogic(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
		//} else if (hDown & KEY_R) {
		//	screenMode = uiSettingsScreen2;
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
		} else if (touching(touch, uiSettingsButtonPos[8])) {
			SaveUniversalSettings();
			saveMsg();
	}
}
}

/*void uiSettingsLogic2(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
		} else if (hDown & KEY_L) {
			screenMode = uiSettingsScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, uiSettingsButtonPos[9])) {
			red = keyboardInputInt("Red");
			settings.universal.color = RGBA8(red, getColorValue(settings.universal.color, 1), getColorValue(settings.universal.color, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[10])) {
			green = keyboardInputInt("Green (0-255)");
			settings.universal.color = RGBA8(getColorValue(settings.universal.color, 2), green, getColorValue(settings.universal.color, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[11])) {
			blue = keyboardInputInt("Blue (0-255)");
			settings.universal.color = RGBA8(getColorValue(settings.universal.color, 2), getColorValue(settings.universal.color, 1), blue, 255);
		} else if (touching(touch, uiSettingsButtonPos[12])) {
				settings.universal.animation++;
			if (settings.universal.animation > 1) settings.universal.animation = 0;
		} else if (touching(touch, uiSettingsButtonPos[13])) {
				settings.universal.battery++;
			if (settings.universal.battery > 1) settings.universal.battery = 0;
		} else if (touching(touch, uiSettingsButtonPos[6])) {
			screenMode = uiSettingsButtonPos[6].link;
	}
}
}*/