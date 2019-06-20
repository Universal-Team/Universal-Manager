#include "animation.hpp"
#include "gui.hpp"
#include "settings.hpp"
#include "keyboard.hpp"
#include <string>
#include <stdio.h>
#include <ctype.h>

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

struct Key {
	std::string character;
	int x;
	int y;
};

Key keys[] = {
	{"1", 0, 0}, {"2", 20, 0}, {"3", 40, 0}, {"4", 60, 0}, {"5", 80, 0}, {"6", 100, 0}, {"7", 120, 0}, {"8", 140, 0}, {"9", 160, 0}, {"0", 180, 0}, {"-", 200, 0}, {"=", 220, 0},
	{"q", 10, 18}, {"w", 30, 18}, {"e", 50, 18}, {"r", 70, 18}, {"t", 90, 18}, {"y", 110, 18}, {"u", 130, 18}, {"i", 150, 18}, {"o", 170, 18}, {"p", 190, 18}, {"[", 210, 18}, {"]", 230, 18},
	{"a", 20, 36}, {"s", 40, 36}, {"d", 60, 36}, {"f", 80, 36}, {"g", 100, 36}, {"h", 120, 36}, {"j", 140, 36}, {"k", 160, 36}, {"l", 180, 36}, {";", 200, 36}, {"'", 220, 36},
	{"z", 30, 54}, {"x", 50, 54}, {"c", 70, 54}, {"v", 90, 54}, {"b", 110, 54}, {"n", 130, 54}, {"m", 150, 54}, {",", 170, 54}, {".", 190, 54}, {"/", 210, 54},

};
Key modifierKeys[] = {
	{"bksp", 240, 0},	// Backspace
	{"caps", 0, 36},	// Caps Lock
	{"entr", 240, 36},	// Enter
	{"lsft", 0, 54},	// Left Shift
	{"rsft", 230, 54},	// Right Shift
};

std::string Input::getLine() { return Input::getLine(-1); }

std::string Input::getLine(uint maxLength) {
	int hDown;
	touchPosition touch;
	std::string string;
	int keyDownDelay = 10, cursorBlink = 20;
	bool caps = false, shift = false, enter = false;
	while(1) {
		do {
			C3D_FrameEnd(0);
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			Gui::DrawBGTop();
			animatedBGTop();
			Gui::chooseLayoutTop();
			C2D_SceneBegin(bottom);
			Gui::DrawBGBot();
			animatedBGBot();
			Gui::chooseLayoutBot();
			Gui::sprite(sprites_keyboard_idx, 0, 130);
			C2D_DrawRectSolid(0, 110, 0.5f, 320, 20, C2D_Color32(0, 0, 68, 200));
			Draw_Text(5, 113, 0.6, WHITE, (string+(cursorBlink-- > 0 ? "_" : "")).c_str());
			if(cursorBlink < -20)	cursorBlink = 20;
			scanKeys();
			hDown = keysDown();
			if(keyDownDelay > 0) {
				keyDownDelay--;
			} else if(keyDownDelay == 0) {
				keyDownDelay--;
				// drawImage(0, 130, keyboardData.width, keyboardData.height, keyboard, false);
				// if(caps) drawRectangle(modifierKeys[1].x, modifierKeys[1].y+(130), 16, 16, GRAY, false);
			}
		} while(!hDown);
		if(keyDownDelay > 0) {
			// drawImage(0, 130, keyboardData.width, keyboardData.height, keyboard, false);
			// if(caps) drawRectangle(modifierKeys[1].x, modifierKeys[1].y+(130), 16, 16, GRAY, false);
		}
		keyDownDelay = 10;

		if(hDown & KEY_TOUCH) {
			touchRead(&touch);
			if(string.length() < maxLength) {
				// Check if a regular key was pressed
				for(uint i=0;i<(sizeof(keys)/sizeof(keys[0]));i++) {
					if((touch.px > keys[i].x-2 && touch.px < keys[i].x+18) && (touch.py > keys[i].y+(130)-2 && touch.py < keys[i].y+18+(130))) {
						// drawRectangle(keys[i].x, keys[i].y+(130), 16, 16, DARK_GRAY, false);
						char c = keys[i].character[0];
						string += (shift || caps ? toupper(c) : c);
						shift = false;
						break;
					}
				}
				// Check if space was pressed
				Key key = {" ", 70, 72};
				if((touch.px > key.x-2 && touch.px < key.x+100) && (touch.py > key.y+(130)-2 && touch.py < key.y+18+(130))) {
					// drawRectangle(key.x, key.y+(130), 96, 16, DARK_GRAY, false);
					string += key.character;
					shift = false;
					// Draw_Text(0, 130, 0.5, BLACK, string.c_str());
				}
			}
			// Check if a modifier key was pressed
			for(uint i=0;i<(sizeof(modifierKeys)/sizeof(modifierKeys[0]));i++) {
				if((touch.px > modifierKeys[i].x-2 && touch.px < modifierKeys[i].x+18) && (touch.py > modifierKeys[i].y+(130)-2 && touch.py < modifierKeys[i].y+18+(130))) {
					if(modifierKeys[i].character == "bksp") {
						// drawRectangle(modifierKeys[i].x, modifierKeys[i].y+(130), 16, 16, DARK_GRAY, false);
						string = string.substr(0, string.length()-1);
						// drawRectangle(0, 130-16, 256, 16, DARKER_GRAY, false);
						// Draw_Text(0, 130, 0.5, BLACK, string.c_str());
					} else if(modifierKeys[i].character == "caps") {
						caps = !caps;
						// if(caps) drawRectangle(modifierKeys[i].x, modifierKeys[i].y+(130), 16, 16, GRAY, false);
					} else if(modifierKeys[i].character == "entr") {
						enter = true;
					} else if(modifierKeys[i].character == "lsft") {
						shift = !shift;
						if(shift) {
							// drawRectangle(modifierKeys[i].x, modifierKeys[i].y+(130), 26, 16, GRAY, false);
							keyDownDelay = -1;
						} else {
							keyDownDelay = 0;
						}
					} else if(modifierKeys[i].character == "rsft") {
						shift = !shift;
						if(shift) {
							// drawRectangle(modifierKeys[i].x, modifierKeys[i].y+(130), 26, 16, GRAY, false);
							keyDownDelay = -1;
						} else {
							keyDownDelay = 0;
						}
					}
					break;
				}
			}
		} else if(hDown & KEY_B) {
			// drawRectangle(modifierKeys[0].x, modifierKeys[0].y+(130), 16, 16, DARK_GRAY, false);
			string = string.substr(0, string.length()-1);
			// drawRectangle(0, 130-16, 256, 16, DARKER_GRAY, false);
			Draw_Text(0, 130, 0.5, BLACK, string.c_str());
		}
		
		if(hDown & KEY_START || enter) {
			// drawRectangle(0, 130-16, 256, keyboardData.height+16, DARK_GRAY, false);
			break;
		}
	}
	return string;
}

int Input::getUint(int max) {
	std::string s = Input::getLine(-1);
	if(s == "" || (atoi(s.c_str()) == 0 && s[0] != '0')) return -1;
	int i = atoi(s.c_str());
	if(i>max)	return 255;
	return i;
}
