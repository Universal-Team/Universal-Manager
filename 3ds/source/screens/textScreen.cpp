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
#include "screens/textScreen.hpp"
#include "utils/fileBrowse.h"
#include "utils/keyboard.hpp"
#include "utils/settings.hpp"
#include "utils/sound.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

void Text::Draw(void) const
{
	if (TextMode == 0) {
		DrawBrowse();
	} else if (TextMode == 1) {
		DrawTextEditor();
	}
}

void Text::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (TextMode == 0) {
		BrowseLogic(hDown, hHeld);
	} else if (TextMode == 1) {
		EditorLogic(hDown, hHeld);
	}
}


void Text::readFile(std::string path)
{
	textEditorText.clear();
	std::string line;
	std::ifstream in(path);
	if(in.good()) {
		while(std::getline(in, line)) {
			textEditorText.push_back(line);
		}
	}
	if(textEditorText.size() == 0)	textEditorText.push_back("");
	in.close();
	textRead = true;
}


void Text::DrawBrowse(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	Gui::DrawString((400-(Gui::GetStringWidth(0.68f, path)))/2, 0, 0.68f, WHITE, path);
	std::string dirs;
	for (uint i=(selectedFile<5) ? 0 : selectedFile-5;i<dirContents.size()&&i<((selectedFile<5) ? 6 : selectedFile+1);i++) {
		(i == selectedFile);

		if (selectedFile == 0) {
			Gui::drawFileSelector(0, 28);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 1) {
			Gui::drawFileSelector(0, 58);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 2) {
			Gui::drawFileSelector(0, 91);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 3) {
			Gui::drawFileSelector(0, 125);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 4) {
			Gui::drawFileSelector(0, 156);
			dirs +=  dirContents[i].name + "\n\n";

		} else if (selectedFile == 5) {
			Gui::drawFileSelector(0, 188);
			dirs +=  dirContents[i].name + "\n\n";
		} else {
			Gui::drawFileSelector(0, 188);
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

    if (Config::selector == 0) {
        Gui::DrawString(26, 32, 0.53f, WHITE, dirs.c_str());
    } else if (Config::selector == 1 || Config::selector == 2) {
        Gui::DrawString(26, 32, 0.53f, BLACK, dirs.c_str());
    }

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void Text::BrowseLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (dirChanged) {
            dirContents.clear();
            std::vector<DirEntry> dirContentsTemp;
            getDirectoryContents(dirContentsTemp);
            for(uint i=0;i<dirContentsTemp.size();i++) {
                  dirContents.push_back(dirContentsTemp[i]);
        }
		dirChanged = false;
	}


	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			currentEditFile = dirContents[selectedFile].name;
			readFile(dirContents[selectedFile].name.c_str());
			TextMode = 1;
		}
		} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			Gui::screenBack();
			return;
 		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
		} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	}
}


void Text::DrawTextEditor(void) const
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLUE2);
	C2D_TargetClear(bottom, BLUE2);
	textRead = false;
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString(200-((Gui::GetStringWidth(FONT_SIZE_18, currentEditFile.c_str())/2)), 0, FONT_SIZE_18, WHITE, currentEditFile.c_str());

	int textX = Draw_GetTextWidthEditor(FONT_SIZE_12, std::to_string(textEditorText.size()).c_str()) + 4;
	for(uint i=0, ii=0;i+textEditorScrnPos<textEditorText.size() && ii<15;i++) {
		std::vector<std::string> lines;
		uint sizeDone = 0;
		do {
			lines.push_back(textEditorText[i+textEditorScrnPos].substr(sizeDone, 60));
			sizeDone += 60;
		} while(sizeDone < textEditorText[i+textEditorScrnPos].size());

		if(i+textEditorScrnPos == textEditorCurPos) {
			Draw_Text_Editor(0, 28+(ii*12), FONT_SIZE_14, Config::selectedText, std::to_string(i+textEditorScrnPos+1).c_str());

			if(showCursor > 0)	C2D_DrawRectSolid(textX+(6.15*(stringPos-(int)((stringPos/60)*60))), 28+((ii+(stringPos/60))*12), 0.5f, 1, 8, Config::selectedText);

			for(uint l=0;l<lines.size();l++) {
				Draw_Text_Editor(textX, 28+(ii*12), FONT_SIZE_14, Config::selectedText, lines[l].c_str());
				ii++;
			}
		} else {
			Draw_Text_Editor(0, 28+(ii*12), FONT_SIZE_14, Config::unselectedText, std::to_string(i+textEditorScrnPos+1).c_str());
			
			for(uint l=0;l<lines.size();l++) {
				Draw_Text_Editor(textX, 28+(ii*12), FONT_SIZE_14, Config::unselectedText, lines[l].c_str());
				ii++;
			}
		}

		rowsDisplayed = i;
	}

	std::string totalLines = "Lines: " + std::to_string(textEditorText.size());
	Gui::DrawString(4, 220, FONT_SIZE_18, WHITE, totalLines.c_str());

	std::string currentLine = "Current Line: " + std::to_string(textEditorCurPos+1);
	Gui::DrawString(400-Gui::GetStringWidth(FONT_SIZE_18, currentLine.c_str())-4, 220, FONT_SIZE_18, WHITE, currentLine.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
	drawKeyboard();
}

void Text::EditorLogic(u32 hDown, u32 hHeld) {
	if(showCursor > -30) {
		showCursor--;
	} else {
		showCursor = 30;
	}

	if(hHeld & KEY_CPAD_UP || hDown & KEY_UP) {
		if(textEditorCurPos > 0) textEditorCurPos--;
		if(stringPos > textEditorText[textEditorCurPos].length())	stringPos = textEditorText[textEditorCurPos].length();
		showCursor = 30;
	} else if(hHeld & KEY_CPAD_DOWN || hDown & KEY_DOWN) {
		if(textEditorCurPos < textEditorText.size()-1) textEditorCurPos++;
		if(stringPos > textEditorText[textEditorCurPos].length())	stringPos = textEditorText[textEditorCurPos].length();
		showCursor = 30;
	} else if(hHeld & KEY_CPAD_LEFT || hDown & KEY_LEFT) {
		if(stringPos > 0)	stringPos--;
		showCursor = 30;
	} else if(hHeld & KEY_CPAD_RIGHT || hDown & KEY_RIGHT) {
		if(stringPos < textEditorText[textEditorCurPos].length())	stringPos++;
		showCursor = 30;
	} else if(hDown & KEY_L) {
		stringPos = 0;
	} else if(hDown & KEY_R) {
		stringPos = textEditorText[textEditorCurPos].length();
	} else if (hDown & KEY_START) {
		if(confirmPopup("Do you want to save your changes?")) {
			std::ofstream out(currentEditFile);
			for(uint i=0;i<textEditorText.size();i++) {
				out << textEditorText[i] << std::endl;
			}
			out.close();
		}
	} else if(hDown & KEY_B) {
		if(confirmPopup("Discard all changes since last save?", "", "Discard", "Cancel", 100)) {
			stringPos = 0;
			textEditorCurPos = 0;
			TextMode = 0;
		}
	} else if(hDown & KEY_X) {
		if(textEditorText.size() > 1) {
			textEditorText.erase(textEditorText.begin()+textEditorCurPos);
			if(textEditorCurPos != 0) {
				textEditorCurPos--;
			}
		}
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
		if(textEditorText[textEditorCurPos].size() > 0) {
			if(stringPos > 0) {
				textEditorText[textEditorCurPos].erase(stringPos-1, 1);
				stringPos--;
			}
		} else if(textEditorText.size() > 1) {
			textEditorText.erase(textEditorText.begin()+textEditorCurPos);
			if(textEditorCurPos != 0)	textEditorCurPos--;
			stringPos = textEditorText[textEditorCurPos].length();
		}
		if(stringPos > textEditorText[textEditorCurPos].length())	stringPos = textEditorText[textEditorCurPos].length();
	} else if(c == '\n') {
		textEditorCurPos++;
		textEditorText.insert(textEditorText.begin()+textEditorCurPos, "");
		stringPos = 0;
	} else if(c != '\0') {
		textEditorText[textEditorCurPos].insert(stringPos, 1, c);
		stringPos++;
	}
}