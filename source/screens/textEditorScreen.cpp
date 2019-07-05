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
#include "keyboard.hpp"
#include "settings.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>

bool textRead = false;
uint textEditorCurPos = 0;
uint textEditorScrnPos = 0;
std::vector<std::string> textEditorText;
uint rowsDisplayed = 0;

void readFile(void) {
	textEditorText.clear();
	std::string line;
	std::ifstream in("test.txt");
	if(in.good()) {
		while(std::getline(in, line)) {
			textEditorText.push_back(line);
		}
	}
	in.close();
	textRead = true;
}

void drawTextEditorScreen(void) {
	if(!textRead)	readFile();
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::chooseLayoutTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text(200-((Draw_GetTextWidth(FONT_SIZE_18, "Text Editor Screen")/2)), 0, FONT_SIZE_18, WHITE, "Text Editor Screen");

	int textX = Draw_GetTextWidth(FONT_SIZE_12, std::to_string(textEditorText.size()).c_str()) + 4;
	for(uint i=0, ii=0;i+textEditorScrnPos<textEditorText.size() && ii<15;i++) {
		std::vector<std::string> lines;
		uint sizeDone = 0;
		do {
			std::string line = textEditorText[i+textEditorScrnPos].substr(sizeDone);
			while(Draw_GetTextWidth(FONT_SIZE_12, line.c_str()) > 400-textX) {
				line.resize(line.size()-1);
			}
			lines.push_back(line);
			sizeDone += line.size();
		} while(sizeDone < textEditorText[i+textEditorScrnPos].size());

		if(i+textEditorScrnPos == textEditorCurPos ) {
			Draw_Text(0, 28+(ii*12), FONT_SIZE_12, BLACK, std::to_string(i+textEditorScrnPos+1).c_str());

			for(uint l=0;l<lines.size();l++) {
				Draw_Text(textX, 28+(ii*12), FONT_SIZE_12, BLUE, lines[l].c_str());
				ii++;
			}
		} else {
			Draw_Text(0, 28+(ii*12), FONT_SIZE_12, GRAY, std::to_string(i+textEditorScrnPos+1).c_str());
			
			for(uint l=0;l<lines.size();l++) {
				Draw_Text(textX, 28+(ii*12), FONT_SIZE_12, BLACK, lines[l].c_str());
				ii++;
			}
		}

		rowsDisplayed = i;
	}

	std::string totalLines = "Lines: " + std::to_string(textEditorText.size());
	Draw_Text(4, 220, FONT_SIZE_18, WHITE, totalLines.c_str());

	std::string currentLine = "Current Line: " + std::to_string(textEditorCurPos+1);
	Draw_Text(400-Draw_GetTextWidth(FONT_SIZE_18, currentLine.c_str())-4, 220, FONT_SIZE_18, WHITE, currentLine.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::chooseLayoutBot();
	gspWaitForVBlank();
}

void TextEditorLogic(u32 hDown, u32 hHeld) {
	if(hHeld & KEY_UP) {
		if(textEditorCurPos > 0) textEditorCurPos--;
	} else if(hHeld & KEY_DOWN) {
		if(textEditorCurPos < textEditorText.size()-1) textEditorCurPos++;
	} else if(hDown & KEY_B) {
		std::ofstream out("test.txt");
		for(uint i=0;i<textEditorText.size();i++) {
			out << textEditorText[i] << std::endl;
		}

		screenMode = mainScreen2;
	}

	// Scroll screen if needed
	if (textEditorCurPos < textEditorScrnPos) 	{
		textEditorScrnPos = textEditorCurPos;
	}
	if (textEditorCurPos > textEditorScrnPos + rowsDisplayed) {
		textEditorScrnPos = textEditorCurPos - rowsDisplayed;
	}

	char c = Input::checkKeyboard(hDown, hHeld);
	if(c == '\b') {
		if(textEditorText[textEditorCurPos].size() > 0)	textEditorText[textEditorCurPos].resize(textEditorText[textEditorCurPos].size()-1);
		else if(textEditorCurPos != 0) {
			textEditorText.erase(textEditorText.begin()+textEditorCurPos);
			textEditorCurPos--;
		}
	} else if(c == '\n') {
		textEditorCurPos++;
		textEditorText.insert(textEditorText.begin()+textEditorCurPos, "");
	} else if(c != '\0')	textEditorText[textEditorCurPos] += c;
}