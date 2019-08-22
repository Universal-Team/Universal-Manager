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
#include "scriptScreen.hpp"
#include "sound.h"
#include "settings.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Script::Draw(void) const
{
	if (ScriptMode == 0) {
		DrawScriptBrowse();
	} else if (ScriptMode == 1) {
		DrawScriptCreator();
	}
}

void Script::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (ScriptMode == 0) {
		ScriptBrowseLogic(hDown, hHeld, touch);
	} else if (ScriptMode == 1) {
		ScriptCreatorLogic(hDown, hHeld, touch);
	}
}

void Script::DrawScriptBrowse(void) const
{
		Gui::DrawBGTop();
		animatedBGTop();
		Gui::DrawBarsTop();
		DisplayTime();
		drawBatteryTop();
		Draw_Text((400-Draw_GetTextWidth(0.72f, "Script Main Screen"))/2, 0, 0.72f, WHITE, "Script Main Screen");
		mkdir("sdmc:/Universal-Manager/scripts/", 0777);

		std::string scptList;
		for (uint i=(selectedScpt<5) ? 0 : selectedScpt-5;i<scpts.size()&&i<((selectedScpt<5) ? 6 : selectedScpt+1);i++) {
			if (selectedScpt == 0) {
				Gui::drawFileSelector(0, 28);
				scptList += scpts[i].name + "\n\n";

			} else if (selectedScpt == 1) {
				Gui::drawFileSelector(0, 58);
				scptList += scpts[i].name + "\n\n";

			} else if (selectedScpt == 2) {
				Gui::drawFileSelector(0, 91);
				scptList += scpts[i].name + "\n\n";

			} else if (selectedScpt == 3) {
				Gui::drawFileSelector(0, 125);
				scptList += scpts[i].name + "\n\n";

			} else if (selectedScpt == 4) {
				Gui::drawFileSelector(0, 156);
				scptList += scpts[i].name + "\n\n";

			} else if (selectedScpt == 5) {
				Gui::drawFileSelector(0, 188);
				scptList += scpts[i].name + "\n\n";
			} else {
				Gui::drawFileSelector(0, 188);
				scptList += scpts[i].name + "\n\n";
			}
		}
		for (uint i=0;i<((scpts.size()<6) ? 6-scpts.size() : 0);i++) {
			scptList += "\n";
		}
		Draw_Text(26, 32, 0.53f, BLACK, scptList.c_str());

		Gui::DrawBGBot();
		animatedBGBot();
		Gui::DrawBarsBot();
}

void Script::ScriptBrowseLogic(u32 hDown, u32 hHeld, touchPosition touch) {
		if(keyRepeatDelay)	keyRepeatDelay--;
		gspWaitForVBlank();
		if(dirChanged) {
			char startPath[PATH_MAX];
			getcwd(startPath, PATH_MAX);
			chdir("sdmc:/Universal-Manager/scripts/");
			getDirectoryContents(scpts);
			chdir(startPath);
		}

		if(hDown & KEY_A) {
			if(confirmPopup("Do you want to run this Script : \n\n "+scpts[selectedScpt].name+"")) {
				runScript("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name);
			} 
		} else if (hDown & KEY_B) {
			Gui::screenBack();
			return;
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
		} else if (hDown & KEY_START) {
			if(confirmPopup("Do you want to edit this Script : \n\n "+scpts[selectedScpt].name+"")) {
			scpt.open(("sdmc:/Universal-Manager/scripts/"+scpts[selectedScpt].name).c_str(), std::ofstream::app);
			Selection = 0;
			ScriptPage = 1;
			ScriptMode = 1;
			}
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press A to start the selected Script. \n \n Press B to return to the Main Menu Screen. \n \n Press X to Delete the selected scpt File. \n \n Press Y to create scpt Files.");
	}
}

void Script::DrawScriptCreator(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Script Creator"))/2, 0, 0.72f, WHITE, "Script Creator");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
	DrawBottom();
	Draw_Text(150, 0, 0.50f, WHITE, "Current Page:");
	Draw_Text(260, 4, 0.50, BLACK, "1");
	Draw_Text(280, 4, 0.50, BLACK, "2"); 
	DrawSelection();
	DrawCurrentPage();
}

void Script::DrawBottom(void) const
{
	if (ScriptPage == 1) {
		Gui::Button(button_button_tl_idx, 1, 27);
		Draw_Text(8, 42, 0.65f, WHITE, "downloadRelease");

		Gui::Button(button_button_2_idx, 162, 27);
		Draw_Text(190, 42, 0.7f, WHITE, "downloadFile");

		Gui::Button(button_button_3_idx, 1, 91);
		Draw_Text(45, 107, 0.65f, WHITE, "extract");

		Gui::Button(button_button_4_idx, 162, 91);
		Draw_Text(222, 104, 0.7f, WHITE, "install");

		Gui::Button(button_button_5_idx, 1, 154);
		Draw_Text(45, 177, 0.7f, WHITE, "delete");

		Gui::Button(button_button_br_idx, 162, 151);
		Draw_Text(225, 177, 0.7f, WHITE, "msg");


	} else if (ScriptPage == 2) {
		Gui::Button(button_button_tl_idx, 1, 27);
		Draw_Text(50, 42, 0.65f, WHITE, "mkdir");
	}
}

void Script::DrawSelection(void) const
{
		if (ScriptPage == 1) {
			if (Selection == 0) {
				Gui::drawGUISelector(button_tl_selector_idx, 0, 26);
			} else if (Selection == 1) {
				Gui::drawGUISelector(button_selector_idx, 166, 31);
			} else if (Selection == 2) {
				Gui::drawGUISelector(button_selector_idx, 3, 95);
			} else if (Selection == 3) {
				Gui::drawGUISelector(button_selector_idx, 166, 95);
			} else if (Selection == 4) {
				Gui::drawGUISelector(button_selector_idx, 3, 158);
			} else if (Selection == 5) {
				Gui::drawGUISelector(button_br_selector_idx, 161, 153);
			}
		} else if (ScriptPage == 2) {
			if (Selection == 0) {
				Gui::drawGUISelector(button_tl_selector_idx, 0, 26);
			}
		}
}

void Script::DrawCurrentPage(void) const
{
	if (ScriptPage == 1) {
		Draw_Text(260, 4, 0.50, WHITE, "1");
	} else if (ScriptPage == 2) {
		Draw_Text(280, 4, 0.50, WHITE, "2");
	}
}

void Script::ScriptCreatorSelectionLogic(u32 hDown, u32 hHeld) {
		if (hHeld & KEY_SELECT) {
			if (ScriptPage == 1) {
				helperBox("downloadRelease : Download a File from Github's Release.\n\ndownloadFile : Download a File from a URL.\n\nextract : Extract an Archive.\n\ninstall : Install a CIA File from the SD Card.\n\ndelete : Delete a FILE from the SD Card.\n\nmsg : Displays a Message on the Top Screen.");
			} else if (ScriptPage == 2) {
				helperBox("mkdir : Creates a Folder.");
			}
		} else if (hDown & KEY_UP) {
			if(Selection > 0)	Selection--;
		} else if (hDown & KEY_DOWN) {
			if (ScriptPage == 1) {
			if(Selection < 5)	Selection++;
			} else if (ScriptPage == 2) {
				if(Selection < 0)	Selection++;
			}
		} else if (hDown & KEY_A) {
			if (ScriptPage == 1) {
			switch(Selection) {
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
		} else if (ScriptPage == 2) {
			switch(Selection) {
				case 0: {
				std::string Function = "mkdir	";
				std::string param1 = Input::getLine();
				scpt << Function << param1 << std::endl;
					break;
				}
			}
		}
	}
}

void Script::ScriptCreatorLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	ScriptCreatorSelectionLogic(hDown, hHeld);
	if (hDown & KEY_START) {
		if(confirmPopup("Do you want to save this Script?")) {
			scpt.close();
			Selection = 0;
			ScriptPage = 1;
			ScriptMode = 0;
		}
	} else if (ScriptPage == 2 && hDown & KEY_L) {
		Selection = 0;
		ScriptPage = 1;
	} else if (ScriptPage == 1 && hDown & KEY_R) {
		Selection = 0;
		ScriptPage = 2;
	} else if (ScriptPage == 1 && hDown & KEY_TOUCH) {
	if (touching(touch, Functions[0])) {

		std::string Function = "downloadRelease	";
		std::string param1 = Input::getLine();
		std::string param2 = Input::getLine();
		std::string param3 = Input::getLine();
		scpt << Function << param1 << "	" << param2 << "	" << param3 << std::endl;


	} else if (touching(touch, Functions[1])) {

		std::string Function = "downloadFile	";
		std::string param1 = Input::getLine();
		std::string param2 = Input::getLine();
		scpt << Function << param1 << "	" << param2 << std::endl;


	} else if (touching(touch, Functions[2])) {

		std::string Function = "extract	";
		std::string param1 = Input::getLine();
		std::string param2 = Input::getLine();
		std::string param3 = Input::getLine();
		scpt << Function << param1 << "	" << param2 << "	" << param3 << std::endl;


	} else if (touching(touch, Functions[3])) {
		std::string Function = "install	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;


	} 	else if (touching(touch, Functions[4])) {
		std::string Function = "delete	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;


	} else if (touching(touch, Functions[5])) {
		std::string Function = "msg	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;
	}

		// Page 2.
	} else if (ScriptPage == 2 && hDown & KEY_TOUCH) {
		if (touching(touch, Functions[6])) {
		std::string Function = "mkdir	";
		std::string param1 = Input::getLine();
		scpt << Function << param1 << std::endl;
		}
	}
}