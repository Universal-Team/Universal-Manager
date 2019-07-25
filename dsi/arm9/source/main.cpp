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

int SCREEN_MODE = 0;

int main(int argc, char **argv) {
	initGraphics();
	keysSetRepeat(25,5);

	if(!fatInitDefault()) {
		// Draws the bottom screen red if fatInitDefault() fails
		drawRectangle(0, 0, 256, 192, BGR15(0, 0, 0xff), false);
		while(1) swiWaitForVBlank();
	}

	if(!nitroFSInit(argv[0])) {
		// Draws the bottom screen blue if nitroFSInit() fails
		drawRectangle(0, 0, 256, 192, BGR15(0xff, 0, 0), false);
		while(1) swiWaitForVBlank();
	}

	loadFont();
	loadGraphics();
	drawRectangle(0, 0, 256, 192, DARK_BLUE, true);
	drawRectangle(0, 0, 256, 192, DARK_BLUE, false);
	printTextTinted("Press A to start.", DARK_GRAY, 32, 0, false);

	u16 hDown = 0;
		while(1) {
			do {
				swiWaitForVBlank();
				scanKeys();
				hDown = keysDown();
			} while(!hDown);
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
		}
				// Scans inputs for the current screen
		switch(SCREEN_MODE) {
//#########################################################################################################
			case mainScreen:
				if (hDown & KEY_A) {
					SCREEN_MODE = fileScreen;
				}
				break;
//#########################################################################################################
			case fileScreen:
				if (hDown & KEY_B) {
					SCREEN_MODE = mainScreen;
				}
				break;
//##########################################################################################################
				}

	}

	return 0;
}