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

#include "colors.hpp"
#include "screens/screenCommon.hpp"
#include "screens/settingsScreen.hpp"
#include "utils/keyboard.hpp"
#include "utils/settings.hpp"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <malloc.h>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <vector>


extern bool touching(touchPosition touch, Structs::ButtonPos button);

	std::string musicModes[] = {"DEFAULT", "COVER", "BG"};
	std::string animationModes[] = {"Disabled", "Bubbles", "Geometry"};
	std::string percentModes[] = {"Hidden", "Shown"}; 
	std::string layoutModes[] = {"Bars", "Bars2"}; 
	std::string layout2Modes[] = {"BG1", "BG2", "BG3"};
	std::string Selectors[] = {"Selector1", "Selector2", "Selector3"};
	
int Settings::getColorValue(int color, int bgr) const
{
	char colorName[10];
	int i;
	std::stringstream ss;

	itoa(color, colorName, 16);
	std::string colorNamePart(colorName, 2*bgr+2, 2);
	ss << std::hex << colorNamePart.c_str();
	ss >> i;

	return i;
}

std::string Settings::getColorName(int color, int bgr) const
{
	char colorName[10];
	int i = getColorValue(color, bgr);
	itoa(i, colorName, 10);
	return colorName;
}

void Settings::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Settings"))/2, 0, 0.72f, WHITE, "Settings");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();
	DrawBottom();
	Gui::DrawString(140, 4, 0.50f, WHITE, "Current Page:");
	Gui::DrawString(240, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Gui::DrawString(260, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	Gui::DrawString(280, 4, 0.50, BLACK, "3"); //Draw Third Page Number.
	DrawCurrentPage();
}

void Settings::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	ButtonLogic(hDown, hHeld);
	TouchLogic(hDown, touch);
}

void Settings::DrawBottom(void) const
{
	if (SettingsPage == 1) {
		Gui::DrawString((320-Gui::GetStringWidth(0.7f, "Bars"))/2, 58, 0.7f, WHITE, "Bars");
		Gui::Button(button_red_idx, 17, 85);
		Gui::DrawString(56, 98, 0.7f, WHITE, getColorName(Config::barColor, 2).c_str());
		Gui::Button(button_green_idx, 112, 85);
		Gui::DrawString(150, 98, 0.7f, WHITE, getColorName(Config::barColor, 1).c_str());
		Gui::Button(button_blue_idx, 207, 85);
		Gui::DrawString(239, 98, 0.7f, WHITE, getColorName(Config::barColor, 0).c_str());

	// Background.
		Gui::DrawString(100, 138, 0.7f, WHITE, "Background");
		Gui::Button(button_red_idx, 17, 168);
		Gui::DrawString(56, 178, 0.7f, WHITE, getColorName(Config::bgColor, 2).c_str());
		Gui::Button(button_green_idx, 112, 168);
		Gui::DrawString(150, 178, 0.7f, WHITE, getColorName(Config::bgColor, 1).c_str());
		Gui::Button(button_blue_idx, 207, 168);
		Gui::DrawString(239, 178, 0.7f, WHITE, getColorName(Config::bgColor, 0).c_str());

		Gui::Button(button_updater_idx, 207, 31);
		Gui::DrawString(225, 38, 0.65f, WHITE, musicModes[Config::musicMode].c_str());
		Gui::DrawString(100, 38, 0.65f, WHITE, "Music Mode:");


	} else if (SettingsPage == 2) {
		Gui::DrawString(70, 73, 0.7f, WHITE, "Animation Color");
		Gui::Button(button_red_idx, 17, 100);
		Gui::DrawString(56, 113, 0.7f, WHITE, getColorName(Config::animationColor, 2).c_str());
		Gui::Button(button_green_idx, 112, 100);
		Gui::DrawString(150, 113, 0.7f, WHITE, getColorName(Config::animationColor, 1).c_str());
		Gui::Button(button_blue_idx, 207, 100);
		Gui::DrawString(239, 113, 0.7f, WHITE, getColorName(Config::animationColor, 0).c_str());

		Gui::Button(button_updater_idx, 207, 31);
		Gui::DrawString(220, 40, 0.65f, WHITE, animationModes[Config::animation].c_str());
		Gui::DrawString(115, 38, 0.7f, WHITE, "Animation:");

		Gui::Button(button_updater_idx, 207, 165);
		Gui::DrawString(229, 178, 0.7f, WHITE, percentModes[Config::percentDisplay].c_str());
		Gui::DrawString(129, 178, 0.7f, WHITE, "Percent :");

	// Bars Layouts.
		Gui::Button(button_updater_idx, 17, 165);
		Gui::DrawString(28, 174, 0.7f, WHITE, layoutModes[Config::layout].c_str());

	// BG Layouts.
		Gui::Button(button_updater_idx, 17, 31);
		Gui::DrawString(28, 40, 0.7f, WHITE, layout2Modes[Config::layoutBG].c_str());


	} else if (SettingsPage == 3) {
		Gui::DrawString(100, 78, 0.7f, WHITE, "Selected Text");
		Gui::Button(button_red_idx, 17, 105);
		Gui::DrawString(56, 118, 0.7f, WHITE, getColorName(Config::selectedText, 2).c_str());
		Gui::Button(button_green_idx, 112, 105);
		Gui::DrawString(150, 118, 0.7f, WHITE, getColorName(Config::selectedText, 1).c_str());
		Gui::Button(button_blue_idx, 207, 105);
		Gui::DrawString(239, 118, 0.7f, WHITE, getColorName(Config::selectedText, 0).c_str());

		Gui::DrawString(100, 148, 0.7f, WHITE, "Unselected Text");
		Gui::Button(button_red_idx, 17, 168);
		Gui::DrawString(56, 178, 0.7f, WHITE, getColorName(Config::unselectedText, 2).c_str());
		Gui::Button(button_green_idx, 112, 168);
		Gui::DrawString(150, 178, 0.7f, WHITE, getColorName(Config::unselectedText, 1).c_str());
		Gui::Button(button_blue_idx, 207, 168);
		Gui::DrawString(239, 178, 0.7f, WHITE, getColorName(Config::unselectedText, 0).c_str());

		// Selectors.
		Gui::DrawString(30, 40, 0.7f, WHITE, "FileBrowse Selector");
		Gui::Button(button_updater_idx, 207, 31);
		Gui::DrawString(213, 40, 0.7f, WHITE, Selectors[Config::selector].c_str());
	}
}

void Settings::DrawCurrentPage(void) const
{
	if (SettingsPage == 1) {
		Gui::DrawString(240, 4, 0.50, WHITE, "1");
	} else if (SettingsPage == 2) {
		Gui::DrawString(260, 4, 0.50, WHITE, "2");
	} else if (SettingsPage == 3) {
		Gui::DrawString(280, 4, 0.50, WHITE, "3");
	}
}


void Settings::ButtonLogic(u32 hDown, u32 hHeld)
{
	if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE052 / \uE053 to switch Pages."); 
	} else if (hDown & KEY_B) {
		Config::saveConfig();
		Gui::screenBack();
		return;


	} else if (hDown & KEY_L) {
		if (SettingsPage == 2) {
			SettingsPage = 1;
		} else if (SettingsPage == 3) {
			SettingsPage = 2;
		}


	} else if (hDown & KEY_R) {
		if (SettingsPage == 1) {
			SettingsPage = 2;
		} else if (SettingsPage == 2) {
			SettingsPage = 3;
		}
	}
}


void Settings::TouchLogic(u32 hDown, touchPosition touch)
{
	int red;
	int green;
	int blue;

	if (hDown & KEY_TOUCH) {
		if (SettingsPage == 1) {
	if (touching(touch, uiSettingsButtonPos[0])) {
			int temp = Input::getUint(255, "Please Type in the Red RGB Value.");
			if(temp != -1) {
				red = temp;
				Config::barColor = RGBA8(red, getColorValue(Config::barColor, 1), getColorValue(Config::barColor, 0), 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[1])) {
			int temp = Input::getUint(255, "Please Type in the Green RGB Value.");
			if(temp != -1) {
				green = temp;
				Config::barColor = RGBA8(getColorValue(Config::barColor, 2), green, getColorValue(Config::barColor, 0), 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[2])) {
			int temp = Input::getUint(255, "Please Type in the Blue RGB Value.");
			if(temp != -1) {
				blue = temp;
				Config::barColor = RGBA8(getColorValue(Config::barColor, 2), getColorValue(Config::barColor, 1), blue, 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[3])) {
			int temp = Input::getUint(255, "Please Type in the Red RGB Value.");
			if(temp != -1) {
				red = temp;
				Config::bgColor = RGBA8(red, getColorValue(Config::bgColor, 1), getColorValue(Config::bgColor, 0), 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[4])) {
			int temp = Input::getUint(255, "Please Type in the Green RGB Value.");
			if(temp != -1) {
				green = temp;
				Config::bgColor = RGBA8(getColorValue(Config::bgColor, 2), green, getColorValue(Config::bgColor, 0), 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[5])) {
			int temp = Input::getUint(255, "Please Type in the Blue RGB Value.");
			if(temp != -1) {
				blue = temp;
				Config::bgColor = RGBA8(getColorValue(Config::bgColor, 2), getColorValue(Config::bgColor, 1), blue, 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[6])) {
			Config::saveConfig();
			Gui::screenBack();
			return;


		} else if (touching(touch, uiSettingsButtonPos[7])) {
			Config::musicMode++;
			if (Config::musicMode > 2) Config::musicMode = 0;
			}





		} else if (SettingsPage == 2) {
		if (touching(touch, uiSettingsButtonPos[8])) {
			int temp = Input::getUint(255, "Please Type in the Red RGB Value.");
			if(temp != -1) {
				red = temp;
				Config::animationColor = RGBA8(red, getColorValue(Config::animationColor, 1), getColorValue(Config::animationColor, 0), 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[9])) {
			int temp = Input::getUint(255, "Please Type in the Green RGB Value.");
			if(temp != -1) {
				green = temp;
				Config::animationColor = RGBA8(getColorValue(Config::animationColor, 2), green, getColorValue(Config::animationColor, 0), 255);
			}


		} else if (touching(touch, uiSettingsButtonPos[10])) {
			int temp = Input::getUint(255, "Please Type in the Blue RGB Value.");
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
			Config::saveConfig();
			Gui::screenBack();
			return;
		}





	} else if (SettingsPage == 3) {
	if (touching(touch, uiSettingsButtonPos[15])) {
		int temp = Input::getUint(255, "Please Type in the Red RGB Value.");
			if(temp != -1) {
				red = temp;
				Config::selectedText = RGBA8(red, getColorValue(Config::selectedText, 1), getColorValue(Config::selectedText, 0), 255);
			}


	} else if (touching(touch, uiSettingsButtonPos[16])) {
		int temp = Input::getUint(255, "Please Type in the Green RGB Value.");
			if(temp != -1) {
				green = temp;
				Config::selectedText = RGBA8(getColorValue(Config::selectedText, 2), green, getColorValue(Config::selectedText, 0), 255);
			}


	} else if (touching(touch, uiSettingsButtonPos[17])) {
			int temp = Input::getUint(255, "Please Type in the Blue RGB Value.");
			if(temp != -1) {
				blue = temp;
				Config::selectedText = RGBA8(getColorValue(Config::selectedText, 2), getColorValue(Config::selectedText, 1), blue, 255);
			}


	} else if (touching(touch, uiSettingsButtonPos[18])) {
		int temp = Input::getUint(255, "Please Type in the Red RGB Value.");
			if(temp != -1) {
				red = temp;
				Config::unselectedText = RGBA8(red, getColorValue(Config::unselectedText, 1), getColorValue(Config::unselectedText, 0), 255);
			}


	} else if (touching(touch, uiSettingsButtonPos[19])) {
		int temp = Input::getUint(255, "Please Type in the Green RGB Value.");
			if(temp != -1) {
				green = temp;
				Config::unselectedText = RGBA8(getColorValue(Config::unselectedText, 2), green, getColorValue(Config::unselectedText, 0), 255);
			}


	} else if (touching(touch, uiSettingsButtonPos[20])) {
			int temp = Input::getUint(255, "Please Type in the Blue RGB Value.");
			if(temp != -1) {
				blue = temp;
				Config::unselectedText = RGBA8(getColorValue(Config::unselectedText, 2), getColorValue(Config::unselectedText, 1), blue, 255);
			}


	} else if (touching(touch, uiSettingsButtonPos[21])) {
		Config::selector++;
		if (Config::selector > 2) Config::selector = 0;


	} else if (touching(touch, uiSettingsButtonPos[6])) {
			Config::saveConfig();
			Gui::screenBack();
			return;
			}
		}
	}
}