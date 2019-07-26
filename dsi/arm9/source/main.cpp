#include <cstdio>
#include <ctype.h>
#include <dirent.h>
#include <fat.h>
#include <fstream>
#include <iostream>
#include <nds.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "fileBrowse.h"
#include "graphics/graphics.h"
#include "nitrofs.h"
#include "screens/screenCommon.hpp"
#include "config.h"

int SCREEN_MODE = 0;

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

static touchPosition touch;

bool touching(touchPosition touch, ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}

int main(int argc, char **argv) {
	initGraphics();
	keysSetRepeat(25,5);

	if(!fatInitDefault()) {
		// Draws the bottom screen red if fatInitDefault() fails
		drawRectangle(0, 0, 256, 192, BGR15(0, 0, 0xff), false);
		printTextTinted("fatInit Failed.", BLACK, 60, 0, false);
		while(1) swiWaitForVBlank();
	}

	if(!nitroFSInit(argv[0])) {
		// Draws the bottom screen blue if nitroFSInit() fails
		drawRectangle(0, 0, 256, 192, BGR15(0xff, 0, 0), false);
		printTextTinted("NitroFS Failed.", BLACK, 60, 0, false);
		while(1) swiWaitForVBlank();
	}

	// Make directories
	mkdir("sd:/_nds", 0777);
	mkdir("sd:/_nds/Universal-Manager", 0777);

	loadFont();
	Config::loadConfig();
	Config::saveConfig();
	loadGraphics();
	drawRectangle(0, 20, 256, 152, Config::Bg, true); //	Top Screen.
	drawRectangle(0, 0, 256, 20, Config::Barcolor, true);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, true);

	drawRectangle(0, 20, 256, 152, Config::Bg, false); //	Bottom Screen.
	drawRectangle(0, 0, 256, 20, Config::Barcolor, false);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, false);
	printTextTinted("Press A to start. :)", BLACK, 60, 0, false);

	u16 hDown = 0;
		while(1) {
				// Draws a screen based on screenMode
		switch(SCREEN_MODE) {
//#########################################################################################################
			case mainScreen:
				drawMainMenu();				// Draws the Main Menu screen
				break;
//#########################################################################################################
			case fileScreen:
				drawFileManagerSubMenu();	// Draws the File Manager screen
				break;
//#########################################################################################################
			case settingsScreen:
				drawSettingsScreen();	// Draws the File Manager screen
				break;
		}

			do {
				scanKeys();
				swiWaitForVBlank();
				hDown = keysDown();
				touchRead(&touch);
			} while(!hDown);


				// Scans inputs for the current screen
		switch(SCREEN_MODE) {
//#########################################################################################################
			case mainScreen:
				mainMenuLogic(hDown, touch);
				break;
//#########################################################################################################
			case fileScreen:
				fileManagerSubMenuLogic(hDown);
				break;
//#########################################################################################################
			case settingsScreen:
				settingsLogic(hDown, touch);
				break;
//##########################################################################################################
				}

	}
	return 0;
}