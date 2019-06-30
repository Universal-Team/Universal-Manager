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

#include "keyboard.hpp"
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
std::string customFont[] = {"ROMFS", "SD"};


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

		// Custom Font.
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
	Draw_Text(140, 0, FONT_SIZE_18, WHITE, "Settings");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	Gui::sprite(sprites_mainMenuButton_idx, 0, 40);
	Draw_Text(40, 57, 0.7f, WHITE, "Credits");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 40);
	Draw_Text(210, 57, 0.7f, WHITE, "Settings");
}

void drawUISettingsScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(120, 0, FONT_SIZE_18, WHITE, "UI-Settings");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Bars.
	Draw_Text(100, 58, 0.7f, WHITE, "Bars");
	Gui::sprite(sprites_RedButton_idx, 35, 88);
	Draw_Text(56, 98, 0.7f, BLACK, getColorName(Config::barColor, 2).c_str());
	Gui::sprite(sprites_GreenButton_idx, 129, 88);
	Draw_Text(150, 98, 0.7f, BLACK, getColorName(Config::barColor, 1).c_str());
	Gui::sprite(sprites_BlueButton_idx, 220, 88);
	Draw_Text(239, 98, 0.7f, BLACK, getColorName(Config::barColor, 0).c_str());

	// Background.
	Draw_Text(100, 138, 0.7f, WHITE, "Background");
	Gui::sprite(sprites_RedButton_idx, 35, 168);
	Draw_Text(56, 178, 0.7f, BLACK, getColorName(Config::bgColor, 2).c_str());
	Gui::sprite(sprites_GreenButton_idx, 129, 168);
	Draw_Text(150, 178, 0.7f, BLACK, getColorName(Config::bgColor, 1).c_str());
	Gui::sprite(sprites_BlueButton_idx, 220, 168);
	Draw_Text(239, 178, 0.7f, BLACK, getColorName(Config::bgColor, 0).c_str());

	Gui::sprite(sprites_updaterButton_idx, 220, 28);
	Draw_Text(229, 38, 0.65f, WHITE, musicModes[Config::musicMode].c_str());
	Draw_Text(110, 38, 0.65f, BLACK, "Music Mode:");
	
	// Font
	Gui::sprite(sprites_updaterButton_idx, 10, 28);
	Draw_Text(19, 38, 0.7f, WHITE, customFont[Config::Font].c_str());

	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, WHITE, "1"); //Draw First Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 2, RED);
	Draw_Text(280, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
}

void uiSettingsLogic(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
		Config::saveConfig();
		} else if (hDown & KEY_R) {
			screenMode = uiSettingsScreen2;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, uiSettingsButtonPos[0])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				red = temp;
				Config::barColor = RGBA8(red, getColorValue(Config::barColor, 1), getColorValue(Config::barColor, 0), 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[1])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				green = temp;
				Config::barColor = RGBA8(getColorValue(Config::barColor, 2), green, getColorValue(Config::barColor, 0), 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[2])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				blue = temp;
				Config::barColor = RGBA8(getColorValue(Config::barColor, 2), getColorValue(Config::barColor, 1), blue, 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[3])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				red = temp;
				Config::bgColor = RGBA8(red, getColorValue(Config::bgColor, 1), getColorValue(Config::bgColor, 0), 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[4])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				green = temp;
				Config::bgColor = RGBA8(getColorValue(Config::bgColor, 2), green, getColorValue(Config::bgColor, 0), 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[5])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				blue = temp;
				Config::bgColor = RGBA8(getColorValue(Config::bgColor, 2), getColorValue(Config::bgColor, 1), blue, 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[6])) {
			screenMode = uiSettingsButtonPos[6].link;
			Config::saveConfig();
		} else if (touching(touch, uiSettingsButtonPos[7])) {
			Config::musicMode++;
			if (Config::musicMode > 2) Config::musicMode = 0;
	} else if(touching(touch, uiSettingsButtonPos[15])) {
			screenMode = FontSelectionScreen;
}
}
}


void drawUISettingsScreen2(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(110, 0, FONT_SIZE_18, WHITE, "Animation Settings");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBotBack();

	// Bars.
	Draw_Text(70, 58, 0.7f, BLACK, "Animation Color");
	Gui::sprite(sprites_RedButton_idx, 35, 88);
	Draw_Text(56, 98, 0.7f, BLACK, getColorName(Config::animationColor, 2).c_str());
	Gui::sprite(sprites_GreenButton_idx, 129, 88);
	Draw_Text(150, 98, 0.7f, BLACK, getColorName(Config::animationColor, 1).c_str());
	Gui::sprite(sprites_BlueButton_idx, 220, 88);
	Draw_Text(239, 98, 0.7f, BLACK, getColorName(Config::animationColor, 0).c_str());

	Gui::sprite(sprites_updaterButton_idx, 220, 28);
	Draw_Text(229, 38, 0.65f, WHITE, animationModes[Config::animation].c_str());
	Draw_Text(110, 38, 0.7f, BLACK, "Animation:");

	Gui::sprite(sprites_updaterButton_idx, 220, 168);
	Draw_Text(229, 178, 0.7f, WHITE, percentModes[Config::percentDisplay].c_str());
	Draw_Text(129, 178, 0.7f, BLACK, "Percent :");
	// Bars Layouts.
	Gui::sprite(sprites_updaterButton_idx, 10, 168);
	Draw_Text(19, 178, 0.7f, WHITE, layoutModes[Config::layout].c_str());
	// BG Layouts.
	Gui::sprite(sprites_updaterButton_idx, 10, 28);
	Draw_Text(19, 38, 0.7f, WHITE, layout2Modes[Config::layoutBG].c_str());

	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(280, 4, 0.50, WHITE, "2"); //Draw Second Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 2, RED);
}

void uiSettingsLogic2(u32 hDown, touchPosition touch) {
	int red;
	int green;
	int blue;
		if (hDown & KEY_B) {
		screenMode = settingsScreen;
		Config::saveConfig();
		} else if (hDown & KEY_L) {
			screenMode = uiSettingsScreen;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, uiSettingsButtonPos[8])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				red = temp;
				Config::animationColor = RGBA8(red, getColorValue(Config::animationColor, 1), getColorValue(Config::animationColor, 0), 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[9])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				green = temp;
				Config::animationColor = RGBA8(getColorValue(Config::animationColor, 2), green, getColorValue(Config::animationColor, 0), 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[10])) {
			int temp = Input::getUint(255);
			if(temp != -1) {
				blue = temp;
				Config::animationColor = RGBA8(getColorValue(Config::animationColor, 2), getColorValue(Config::animationColor, 1), blue, 255);
			}
		} else if (touching(touch, uiSettingsButtonPos[11])) {
			Config::animation++;
			if (Config::animation > 2) Config::animation = 0;
			} else if (touching(touch, uiSettingsButtonPos[12])) {
			Config::percentDisplay++;
			if (Config::percentDisplay > 1) Config::percentDisplay = 0;
			} else if (touching(touch, uiSettingsButtonPos[13])) {
			Config::layout++;
			if (Config::layout > 1) Config::layout = 0;
			} else if (touching(touch, uiSettingsButtonPos[14])) {
			Config::layoutBG++;
			if (Config::layoutBG > 2) Config::layoutBG = 0;
			} else if (touching(touch, uiSettingsButtonPos[6])) {
			screenMode = uiSettingsButtonPos[6].link;
			Config::saveConfig();
}
	}
}