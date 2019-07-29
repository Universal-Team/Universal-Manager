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
#include "scripts.hpp"
#include "fileBrowse.h"
#include "keyboard.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>
using std::string;

static int scriptSelection1 = 0;
static int scriptSelection2 = 0;
extern uint selectedFile;
extern int keyRepeatDelay;
extern bool dirChanged;
extern std::vector<DirEntry> dirContents;
uint selectedScpt = 0;
std::vector<DirEntry> scpts;

std::ofstream scpt;

uint selectedScptItem = 0;
int movingScptItem = -1;
std::vector<std::string> scptContents;

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};
extern bool touching(touchPosition touch, ButtonPos button);

ButtonPos scriptCreatorFunctionButtonPos[] = {
	// First Page.
    {0, 25, 149, 52, -1},
    {170, 25, 149, 52, -1},
	{0, 90, 149, 52, -1},
	{170, 90, 149, 52, -1},
	{0, 150, 149, 52, -1},
    {170, 150, 149, 52, -1},
	// Second Page.
	{0, 25, 149, 52, -1},
};

void drawScriptMainScreen(void) {
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Script Main Screen"))/2, 0, 0.72f, WHITE, "Script Main Screen");
	mkdir("sdmc:/Universal-Manager/scripts/", 0777);
	
	if(dirChanged) {
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/scripts/");
		getDirectoryContents(scpts);
		chdir(startPath);
	}

	std::string scptList;
	for (uint i=(selectedScpt<5) ? 0 : selectedScpt-5;i<scpts.size()&&i<((selectedScpt<5) ? 6 : selectedScpt+1);i++) {
		if (selectedScpt == 0) {
			Gui::sprite(sprites_selected_idx, 0, 25);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 1) {
			Gui::sprite(sprites_selected_idx, 0, 56);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 2) {
			Gui::sprite(sprites_selected_idx, 0, 91);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 3) {
			Gui::sprite(sprites_selected_idx, 0, 125);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 4) {
			Gui::sprite(sprites_selected_idx, 0, 160);
			scptList += scpts[i].name + "\n\n";

		} else if (selectedScpt == 5) {
			Gui::sprite(sprites_selected_idx, 0, 190);
			scptList += scpts[i].name + "\n\n";
		} else {
			Gui::sprite(sprites_selected_idx, 0, 190);
			scptList += scpts[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((scpts.size()<6) ? 6-scpts.size() : 0);i++) {
		scptList += "\n";
	}
	Draw_Text(26, 32, 0.53f, WHITE, scptList.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}


void testWrite(void) {
	FILE* scpt = fopen(("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name).c_str(), "a");
	std::string scriptLine1 = Input::getLine();
	fputs((scriptLine1.c_str()), scpt);
	fclose(scpt);
}

void scriptMainScreenLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	gspWaitForVBlank();
	if(hDown & KEY_A) {
		if(confirmPopup("Do you want to run this Script : \n\n "+scpts[selectedScpt].name+"")) {
			runScript("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name);
		} 
	} else if (hDown & KEY_B) {
		screenTransition(mainScreen);
	} else if (hDown & KEY_Y) {
		std::string newScript = Input::getLine();
		if(newScript != "") {
			FILE* scpt = fopen(("sdmc:/Universal-Manager/scripts/"+newScript+".scpt").c_str(), "w");
			fclose(scpt);
	}
	} else if (hDown & KEY_X) {
		if (selectedScpt != 0) {
			if(confirmPopup("Are you sure you want to delete this Script?")) {
				remove(("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name).c_str());
			}
		}
	} else if (hHeld & KEY_UP) {
		if (selectedScpt > 0 && !keyRepeatDelay) {
			selectedScpt--;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedScpt < scpts.size()-1) {
			selectedScpt++;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press A to start the selected Script. \n \n Press B to return to the Main Menu Screen. \n \n Press X to Delete the selected scpt File. \n \n Press Y to create scpt Files.");
	} else if (hDown & KEY_START) {
		if(confirmPopup("Do you want to edit this Script : \n\n "+scpts[selectedScpt].name+"")) {
		scpt.open(("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name).c_str(), std::ofstream::app);
		screenTransition(scriptCreatorFunctions);
	}
	}
}

int screenPage = 0;

static void drawSelection1(void) {
	if (scriptSelection1 == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 80, 10, Config::barColor);
	} else if (scriptSelection1 == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 250, 10, Config::barColor);
	} else if (scriptSelection1 == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 80, 75, Config::barColor);
	} else if (scriptSelection1 == 3) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 250, 75, Config::barColor);
	} else if (scriptSelection1 == 4) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 80, 145, Config::barColor);
	} else if (scriptSelection1 == 5) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 250, 145, Config::barColor);
	}
}

static void drawSelection2(void) {
	if (scriptSelection2 == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 80, 10, Config::barColor);
	}
}

void drawScriptsCreatorFunctions(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Script Creator"))/2, 0, 0.72f, WHITE, "Script Creator");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();

	// Functions.
	if (screenPage == 0) {
	Gui::sprite(sprites_mainMenuButton_idx, 0, 25);
	Draw_Text(8, 42, 0.65f, WHITE, "downloadRelease");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 25);
	Draw_Text(190, 42, 0.7f, WHITE, "downloadFile");

	Gui::sprite(sprites_mainMenuButton_idx, 0, 90);
	Draw_Text(45, 107, 0.65f, WHITE, "extract");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 90);
	Draw_Text(222, 104, 0.7f, WHITE, "install");

	Gui::sprite(sprites_mainMenuButton_idx, 0, 160);
	Draw_Text(45, 177, 0.7f, WHITE, "delete");

	Gui::sprite(sprites_mainMenuButton_idx, 170, 160);
	Draw_Text(225, 177, 0.7f, WHITE, "msg");

	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, WHITE, "1"); //Draw First Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 256, 2, RED);
	Draw_Text(280, 4, 0.50, BLACK, "2"); //Draw Second Page Number.
	drawSelection1();
	} else if (screenPage == 1) {

	// Second Page.
	Gui::sprite(sprites_mainMenuButton_idx, 0, 25);
	Draw_Text(50, 42, 0.65f, WHITE, "mkdir");

	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, BLACK, "1"); //Draw First Page Number.
	Draw_Text(280, 4, 0.50, WHITE, "2"); //Draw Second Page Number.
	Gui::Draw_ImageBlend(sprites_frame_idx, 276, 2, RED);
	drawSelection2();
}
}

static void scriptSelectionLogic1(u32 hDown) {
		if (screenPage == 0 && hDown & KEY_UP) {
			if(scriptSelection1 > 0)	scriptSelection1--;
		} else if (screenPage == 0 && hDown & KEY_DOWN) {
			if(scriptSelection1 < 5)	scriptSelection1++;
		} else if (screenPage == 0 && hDown & KEY_A) {
			switch(scriptSelection1) {
				case 0: {
				std::string Function = "downloadRelease	";
				std::string param1 = Input::getLine();
				std::string param2 = Input::getLine();
				std::string param3 = Input::getLine();
				scpt << Function << param1 << "	" << param2 << "	" << param3 << std::endl;
					break;
				} case 1: {
				std::string Function = "downloadFile	";
				std::string param1 = Input::getLine();
				std::string param2 = Input::getLine();
				scpt << Function << param1 << "	" << param2 << std::endl;
					break;
				  } case 2: {
				std::string Function = "extract	";
				std::string param1 = Input::getLine();
				std::string param2 = Input::getLine();
				std::string param3 = Input::getLine();
				scpt << Function << param1 << "	" << param2 << "	" << param3 << std::endl;
					break;
				} case 3: {
				std::string Function = "install	";
				std::string param1 = Input::getLine();
				scpt << Function << param1 << std::endl;
					break;
				} case 4: {
				std::string Function = "delete	";
				std::string param1 = Input::getLine();
				scpt << Function << param1 << std::endl;
					break;
				} case 5: {
				std::string Function = "msg	";
				std::string param1 = Input::getLine();
				scpt << Function << param1 << std::endl;
					break;
			}
		}
		}
}

static void scriptSelectionLogic2(u32 hDown) {
		if (screenPage == 1 && hDown & KEY_UP) {
			if(scriptSelection2 > 0)	scriptSelection2--;
		} else if (screenPage == 1 && hDown & KEY_DOWN) {
			if(scriptSelection1 < 0)	scriptSelection2++;
		} else if (screenPage == 1 && hDown & KEY_A) {
			switch(scriptSelection2) {
				case 0: {
				std::string Function = "mkdir	";
				std::string param1 = Input::getLine();
				scpt << Function << param1 << std::endl;
					break;
				}
			}
		}
}

void scriptCreatorFunctionsLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	scriptSelectionLogic1(hDown);
	scriptSelectionLogic2(hDown);
	if (screenPage == 0 && hHeld & KEY_SELECT) {
	helperBox("downloadRelease : Download a File from Github's Release.\n\ndownloadFile : Download a File from a URL.\n\nextract : Extract an Archive.\n\ninstall : Install a CIA File from the SD Card.\n\ndelete : Delete a FILE from the SD Card.\n\nmsg : Displays a Message on the Top Screen.");
	} else if (screenPage == 1 && hHeld & KEY_SELECT) {
	helperBox("mkdir : Creates a Folder.");
	} else if (hDown & KEY_START) {
		if(confirmPopup("Do you want to save this Script?")) {
			scpt.close();
			screenTransition(scriptMainScreen);
	}
	} else if (screenPage == 1 && hDown & KEY_L) {
		screenPage = 0;
	} else if (screenPage == 0 && hDown & KEY_R) {
		screenPage = 1;
	} else if (screenPage == 0 && hDown & KEY_TOUCH) {
	if (touching(touch, scriptCreatorFunctionButtonPos[0])) {

		std::string Function = "downloadRelease	";
		std::string param1 = Input::getLine();
		std::string param2 = Input::getLine();
		std::string param3 = Input::getLine();
		scpt << Function << param1 << "	" << param2 << "	" << param3 << std::endl;


	} else if (touching(touch, scriptCreatorFunctionButtonPos[1])) {

		std::string Function = "downloadFile	";
		std::string param1 = Input::getLine();
		std::string param2 = Input::getLine();
		scpt << Function << param1 << "	" << param2 << std::endl;


	} else if (touching(touch, scriptCreatorFunctionButtonPos[2])) {

		std::string Function = "extract	";
		std::string param1 = Input::getLine();
		std::string param2 = Input::getLine();
		std::string param3 = Input::getLine();
		scpt << Function << param1 << "	" << param2 << "	" << param3 << std::endl;


	} else if (touching(touch, scriptCreatorFunctionButtonPos[3])) {
		std::string Function = "install	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;


	} 	else if (touching(touch, scriptCreatorFunctionButtonPos[4])) {
		std::string Function = "delete	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;


	} else if (touching(touch, scriptCreatorFunctionButtonPos[5])) {
		std::string Function = "msg	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;
	}

		// Page 2.
	} else if (screenPage == 1 && hDown & KEY_TOUCH) {
		std::string Function = "mkdir	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;
	}
}

