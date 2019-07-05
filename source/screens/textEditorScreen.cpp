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
#include "fileBrowse.h"
#include "settings.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>

bool textRead = false;
uint textEditorLine = 0;
uint textEditorLines = 0;
uint textEditorCurPos = 0;
uint textEditorScrnPos = 0;
std::vector<std::string> textEditorText = {"test", "1,", "2,", "3!"};

void readFile(void) {
	textEditorText.clear();
	std::string line;
	std::ifstream in("test.txt");
	while(std::getline(in, line)) {
		textEditorText.push_back(line);
	}
	textRead = true;
}

void drawTextEditorScreen(void) {
	readFile();
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(200-((Draw_GetTextWidth(FONT_SIZE_18, "Text Editor Screen")/2)), 0, FONT_SIZE_18, WHITE, "Text Editor Screen");

	int textX = Draw_GetTextWidth(FONT_SIZE_12, std::to_string(textEditorText.size()).c_str()) + 4;
	for(uint i=0;i+textEditorScrnPos<textEditorText.size() && i<15;i++) {
		if(i+textEditorScrnPos == textEditorCurPos) {
			Draw_Text(0, 28+(i*12), FONT_SIZE_12, BLACK, std::to_string(i).c_str());
			Draw_Text(textX, 28+(i*12), FONT_SIZE_12, BLUE, textEditorText[i+textEditorScrnPos].c_str());
		} else {
			Draw_Text(0, 28+(i*12), FONT_SIZE_12, GRAY, std::to_string(i).c_str());
			Draw_Text(textX, 28+(i*12), FONT_SIZE_12, BLACK, textEditorText[i+textEditorScrnPos].c_str());
		}
	}

	std::string totalLines = "Lines: " + std::to_string(textEditorLines);
	Draw_Text(4, 220, FONT_SIZE_18, WHITE, totalLines.c_str());

	std::string currentLine = "Current Line: " + std::to_string(textEditorLine);
	Draw_Text(400-Draw_GetTextWidth(FONT_SIZE_18, currentLine.c_str())-4, 220, FONT_SIZE_18, WHITE, currentLine.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
}

void TextEditorLogic(u32 hDown, u32 hHeld) {
	if(hHeld & KEY_UP) {
		if(textEditorCurPos > 0) textEditorCurPos--;
	} else if(hHeld & KEY_DOWN) {
		if(textEditorCurPos < textEditorText.size()-1) textEditorCurPos++;
	} else if(hDown & KEY_B) {
		screenMode = mainScreen2;
	}

	// Scroll screen if needed
	if (textEditorCurPos < textEditorScrnPos) 	{
		textEditorScrnPos = textEditorCurPos;
	}
	if (textEditorCurPos > textEditorScrnPos + 14) {
		textEditorScrnPos = textEditorCurPos - 14;
	}
}