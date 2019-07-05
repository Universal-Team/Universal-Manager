#include "animation.hpp"
#include "gui.hpp"
#include "settings.hpp"
#include "keyboard.hpp"
#include <string>
#include <stdio.h>
#include <ctype.h>

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

bool caps = false, shift = false, enter = false;

struct Key {
	std::string character;
	int x;
	int y;
	int w;
};

Key keys[] = {
	{"1", 0, 0}, {"2", 25, 0}, {"3", 50, 0}, {"4", 75, 0}, {"5", 100, 0}, {"6", 125, 0}, {"7", 150, 0}, {"8", 175, 0}, {"9", 200, 0}, {"0", 225, 0}, {"-", 250, 0}, {"=", 275, 0},
	{"q", 12, 22}, {"w", 37, 22}, {"e", 62, 22}, {"r", 87, 22}, {"t", 112, 22}, {"y", 137, 22}, {"u", 162, 22}, {"i", 187, 22}, {"o", 212, 22}, {"p", 237, 22}, {"[", 262, 22}, {"]", 287, 22},
	{"a", 25, 45}, {"s", 50, 45}, {"d", 75, 45}, {"f", 100, 45}, {"g", 125, 45}, {"h", 150, 45}, {"j", 175, 45}, {"k", 200, 45}, {"l", 225, 45}, {";", 250, 45}, {"'", 275, 45},
	{"z", 35, 67}, {"x", 60, 67}, {"c", 85, 67}, {"v", 110, 67}, {"b", 135, 67}, {"n", 160, 67}, {"m", 185, 67}, {",", 210, 67}, {".", 235, 67}, {"/", 260, 67},
};
Key modifierKeys[] = {
	{"bksp", 300, 0, 20},	// Backspace
	{"caps", 0, 45, 20},	// Caps Lock
	{"entr", 300, 45, 20},	// Enter
	{"lsft", 0, 67, 30},	// Left Shift
	{"rsft", 285, 67, 30},	// Right Shift
};

std::string Input::getLine() { return Input::getLine(-1); }

std::string Input::getLine(uint maxLength) {
	int hDown;
	touchPosition touch;
	std::string string;
	int keyDownDelay = 10, cursorBlink = 20;
	caps = false, shift = false, enter = false;
	while(1) {
		do {
			C3D_FrameEnd(0);
			Gui::clearTextBufs();
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
				Key key = {" ", 85, 90};
				if((touch.px > key.x-2 && touch.px < key.x+120) && (touch.py > key.y+(130)-2 && touch.py < key.y+20+(130))) {
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

int keyDownDelay = 5;

char Input::checkKeyboard(int hDown, int hHeld) {
	Gui::sprite(sprites_keyboard_idx, 0, 130);
	if(caps)	C2D_DrawRectSolid(modifierKeys[1].x, modifierKeys[1].y+(130), 0.5f, 20, 20, BLUE);
	if(shift)	C2D_DrawRectSolid(modifierKeys[3].x, modifierKeys[3].y+(130), 0.5f, 30, 20, BLUE);
	if(hDown & KEY_TOUCH)	keyDownDelay = 15;
	if(keyDownDelay > 0) {
		keyDownDelay--;
	} else if(keyDownDelay == 0) {
		keyDownDelay = 3;
	}

	if(hDown & KEY_TOUCH || (hHeld & KEY_TOUCH && keyDownDelay == 0)) {
		touchPosition touch;
		touchRead(&touch);
		// Check if a regular key was pressed
		for(uint i=0;i<(sizeof(keys)/sizeof(keys[0]));i++) {
			if((touch.px > keys[i].x-2 && touch.px < keys[i].x+22) && (touch.py > keys[i].y+(130)-2 && touch.py < keys[i].y+22+(130))) {
				char c = keys[i].character[0];
				c = shift || caps ? toupper(c) : c;
				shift = false;
				return c;
			}
		}
		// Check if space was pressed
		Key key = {" ", 85, 90};
		if((touch.px > key.x-2 && touch.px < key.x+120) && (touch.py > key.y+(130)-2 && touch.py < key.y+20+(130))) {
			shift = false;
			return key.character[0];
		}
		// Check if a modifier key was pressed
		for(uint i=0;i<(sizeof(modifierKeys)/sizeof(modifierKeys[0]));i++) {
			if((touch.px > modifierKeys[i].x-2 && touch.px < modifierKeys[i].x+modifierKeys[i].w+2) && (touch.py > modifierKeys[i].y+(130)-2 && touch.py < modifierKeys[i].y+22+(130))) {
				if(modifierKeys[i].character == "bksp") {
					return '\b';
				} else if(modifierKeys[i].character == "caps") {
					caps = !caps;
				} else if(modifierKeys[i].character == "entr") {
					return '\n';
				} else if(modifierKeys[i].character == "lsft") {
					shift = !shift;
				} else if(modifierKeys[i].character == "rsft") {
					shift = !shift;
				}
				break;
			}
		}
	}
	return '\0';
}
