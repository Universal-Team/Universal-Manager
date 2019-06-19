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
#include "colors.hpp"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include <sstream>

#include "keyboard.h"
#include "utils/settings.hpp"

struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

extern bool touching(touchPosition touch, ButtonPos button);
std::string musicModes[] = {"DEFAULT", "COVER", "BG"};
std::string animationModes[] = {"Disabled", "Bubbles", "Geometry"};
std::string percentModes[] = {"Hidden", "Shown"}; 
std::string layoutModes[] = {"Bars", "Bars v2"}; 
std::string layout2Modes[] = {"BG1", "BG2", "BG3"};


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

	// Bubble Color.
	{46, 98, 87, 33, -1},
	{129, 98, 87, 33, -1},
	{220, 98, 87, 33, -1},

	// Animation enable.
	{220, 28, 87, 33, -1},

	// Battery percent.
	{220, 168, 87, 33, -1},

		// Bars Layout.
	{10, 168, 87, 33, -1},

		// BG Layout.
	{10, 28, 87, 33, -1},
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
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Gui::staticText((i18n::localize("SETTINGS")), 200, 0, FONT_SIZE_18, FONT_SIZE_18, WHITE, TextPosX::CENTER, TextPosY::TOP);
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	Gui::sprite(sprites_mainMenuButton_idx, 0, 40);
	Gui::staticText((i18n::localize("CREDITS")), 50, 57, 0.7f, 0.7f, WHITE, TextPosX::CENTER, TextPosY::TOP);

	Gui::sprite(sprites_mainMenuButton_idx, 170, 40);
	Gui::staticText((i18n::localize("SETTINGS")), 230, 57, 0.7f, 0.7f, WHITE, TextPosX::CENTER, TextPosY::TOP);
}

void drawUISettingsScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Gui::staticText((i18n::localize("UI_SETTINGS")), 200, 0, FONT_SIZE_18, FONT_SIZE_18, WHITE, TextPosX::CENTER, TextPosY::TOP);
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Bars.
	Gui::staticText((i18n::localize("BARS")), 170, 58, 0.7f, 0.7f, BLACK, TextPosX::CENTER, TextPosY::TOP);
	Gui::sprite(sprites_RedButton_idx, 35, 88);
	draw_text(46, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bars, 2).c_str());
	Gui::sprite(sprites_GreenButton_idx, 129, 88);
	draw_text(140, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bars, 1).c_str());
	Gui::sprite(sprites_BlueButton_idx, 220, 88);
	draw_text(229, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bars, 0).c_str());

	// Background.
	Gui::staticText((i18n::localize("BACKGROUND")), 170, 138, 0.7f, 0.7f, BLACK, TextPosX::CENTER, TextPosY::TOP);
	Gui::sprite(sprites_RedButton_idx, 35, 168);
	draw_text(46, 178, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bg, 2).c_str());
	Gui::sprite(sprites_GreenButton_idx, 129, 168);
	draw_text(140, 178, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bg, 1).c_str());
	Gui::sprite(sprites_BlueButton_idx, 220, 168);
	draw_text(229, 178, 0.7f, 0.7f, BLACK, getColorName(settings.universal.bg, 0).c_str());

	Gui::sprite(sprites_updaterButton_idx, 220, 28);
	draw_text(229, 38, 0.65f, 0.65f, WHITE, musicModes[settings.universal.music].c_str());
	draw_text(110, 38, 0.7f, 0.7f, BLACK, "Music Mode:");

	Gui::staticText((i18n::localize("CURRENT_PAGE")), 170, 0, 0.50, 0.50, WHITE, TextPosX::CENTER, TextPosY::TOP);
	draw_text(260, 4, 0.50, 0.50, WHITE, "1"); //Draw First Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 2, RED);
	draw_text(280, 4, 0.50, 0.50, BLACK, "2"); //Draw Second Page Number.
}

void uiSettingsLogic(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
		SaveUniversalSettings();
		} else if (hDown & KEY_R) {
			screenMode = uiSettingsScreen2;
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
			SaveUniversalSettings();
		} else if (touching(touch, uiSettingsButtonPos[7])) {
			settings.universal.music++;
			if (settings.universal.music > 2) settings.universal.music = 0;
	}
}
}


void drawUISettingsScreen2(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Gui::staticText((i18n::localize("ANIMATION_SETTINGS")), 200, 0, FONT_SIZE_18, FONT_SIZE_18, WHITE, TextPosX::CENTER, TextPosY::TOP);
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Bars.
	draw_text(120, 58, 0.7f, 0.7f, BLACK, "Bubbles");
	Gui::sprite(sprites_RedButton_idx, 35, 88);
	draw_text(46, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.animationcolor, 2).c_str());
	Gui::sprite(sprites_GreenButton_idx, 129, 88);
	draw_text(140, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.animationcolor, 1).c_str());
	Gui::sprite(sprites_BlueButton_idx, 220, 88);
	draw_text(229, 98, 0.7f, 0.7f, BLACK, getColorName(settings.universal.animationcolor, 0).c_str());

	Gui::sprite(sprites_updaterButton_idx, 220, 28);
	draw_text(229, 38, 0.65f, 0.65f, WHITE, animationModes[settings.universal.animation].c_str());
	draw_text(110, 38, 0.7f, 0.7f, BLACK, "Animation:");

	Gui::sprite(sprites_updaterButton_idx, 220, 168);
	draw_text(229, 178, 0.7f, 0.7f, WHITE, percentModes[settings.universal.battery].c_str());
	draw_text(129, 178, 0.7f, 0.7f, BLACK, "Percent :");
	// Bars Layouts.
	Gui::sprite(sprites_updaterButton_idx, 10, 168);
	draw_text(19, 178, 0.7f, 0.7f, WHITE, layoutModes[settings.universal.layout].c_str());
	// BG Layouts.
	Gui::sprite(sprites_updaterButton_idx, 10, 28);
	draw_text(19, 38, 0.7f, 0.7f, WHITE, layout2Modes[settings.universal.bgl].c_str());

	Gui::staticText((i18n::localize("CURRENT_PAGE")), 170, 0, 0.50, 0.50, WHITE, TextPosX::CENTER, TextPosY::TOP);
	draw_text(260, 4, 0.50, 0.50, BLACK, "1"); //Draw First Page Number.
	draw_text(280, 4, 0.50, 0.50, WHITE, "2"); //Draw Second Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 2, RED);
}

void uiSettingsLogic2(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
		SaveUniversalSettings();
		} else if (hDown & KEY_L) {
			screenMode = uiSettingsScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, uiSettingsButtonPos[8])) {
			red = keyboardInputInt("Red");
			settings.universal.animationcolor = RGBA8(red, getColorValue(settings.universal.animationcolor, 1), getColorValue(settings.universal.animationcolor, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[9])) {
			green = keyboardInputInt("Green (0-255)");
			settings.universal.animationcolor = RGBA8(getColorValue(settings.universal.animationcolor, 2), green, getColorValue(settings.universal.animationcolor, 0), 255);
		} else if (touching(touch, uiSettingsButtonPos[10])) {
			blue = keyboardInputInt("Blue (0-255)");
			settings.universal.animationcolor = RGBA8(getColorValue(settings.universal.animationcolor, 2), getColorValue(settings.universal.animationcolor, 1), blue, 255);
		} else if (touching(touch, uiSettingsButtonPos[11])) {
			settings.universal.animation++;
			if (settings.universal.animation > 2) settings.universal.animation = 0;
			} else if (touching(touch, uiSettingsButtonPos[12])) {
			settings.universal.battery++;
			if (settings.universal.battery > 1) settings.universal.battery = 0;
			} else if (touching(touch, uiSettingsButtonPos[13])) {
			settings.universal.layout++;
			if (settings.universal.layout > 1) settings.universal.layout = 0;
			} else if (touching(touch, uiSettingsButtonPos[14])) {
			settings.universal.bgl++;
			if (settings.universal.bgl > 2) settings.universal.bgl = 0;
			} else if (touching(touch, uiSettingsButtonPos[6])) {
			screenMode = uiSettingsButtonPos[6].link;
			SaveUniversalSettings();
}
	}
}