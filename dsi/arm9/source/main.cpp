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
#include "gui.hpp"
#include "mainMenuScreen.hpp"
#include "structs.hpp"

touchPosition touch;

bool touching(touchPosition touch, Structs::ButtonPos button) {
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

	Gui::setScreen(std::make_unique<MainMenu>());

	u16 hDown = 0;
	do {
		scanKeys();
		swiWaitForVBlank();
		hDown = keysDown();
	} while(!(hDown & KEY_A));

	do {
		scanKeys();
		swiWaitForVBlank();
		hDown = keysDown();
		touchRead(&touch);
		Gui::mainLoop(hDown, touch);
	} while(1);

	return 0;
}