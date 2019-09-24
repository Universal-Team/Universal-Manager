#include "gui.hpp"
#include "logging.hpp"

#include "graphics/graphics.h"

#include "screens/mainMenuScreen.hpp"
#include "screens/screenCommon.hpp"

#include "utils/config.h"
#include "utils/fileBrowse.h"
#include "utils/nitrofs.h"
#include "utils/structs.hpp"

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

	// If the log does not exists -> Create it.
	Logging::createLogFile();

	if(!fatInitDefault()) {
		// Draws the bottom screen red if fatInitDefault() fails
		drawRectangle(0, 0, 256, 192, BGR15(0, 0, 0xff), false);
		printf("fatInit Failed and can't be logged.");
		while(1) swiWaitForVBlank();
	}

	if(!nitroFSInit(argv[0])) {
		// Draws the bottom screen blue if nitroFSInit() fails
		Logging::writeToLog("NitroFS Failed! Please use another start point, which passes NitroFS.", true);
		drawRectangle(0, 0, 256, 192, BGR15(0xff, 0, 0), false);
		
		printf("NitroFS Failed.");
		while(1) swiWaitForVBlank();
	}

	// Make directories
	mkdir("/_nds", 0777);
	mkdir("/_nds/Universal-Manager", 0777);
	mkdir("/_nds/Universal-Manager/scripts", 0777);

	loadFont();
	Config::loadConfig();
	Config::saveConfig();
	loadGraphics();

	// Log, that Universal-Manager starting is finished.
	Logging::writeToLog("Universal-Manager launched successfully!", true);

	drawRectangle(0, 20, 256, 152, Config::Bg, true); //	Top Screen.
	drawRectangle(0, 0, 256, 20, Config::Barcolor, true);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, true);

	drawRectangle(0, 20, 256, 152, Config::Bg, false); //	Bottom Screen.
	drawRectangle(0, 0, 256, 20, Config::Barcolor, false);
	drawRectangle(0, 172, 256, 20, Config::Barcolor, false);
	printTextCenteredScaled("Press A to start. :)", 1.0, 1.0, 0, 5, false);

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