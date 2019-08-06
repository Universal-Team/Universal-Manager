
#include "screens/screenCommon.hpp"
#include "keyboard.hpp"
#include "settings.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include "textEditorScreen.hpp"

void TextEditor::Draw(void) const
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
	Draw_Text(200-((Draw_GetTextWidth(FONT_SIZE_18, currentEditFile.c_str())/2)), 0, FONT_SIZE_18, WHITE, currentEditFile.c_str());

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
	Draw_Text(4, 220, FONT_SIZE_18, WHITE, totalLines.c_str());

	std::string currentLine = "Current Line: " + std::to_string(textEditorCurPos+1);
	Draw_Text(400-Draw_GetTextWidth(FONT_SIZE_18, currentLine.c_str())-4, 220, FONT_SIZE_18, WHITE, currentLine.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void TextEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
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
			Gui::screenBack();
			return;
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