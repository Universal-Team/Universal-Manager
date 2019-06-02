/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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
#include "ftp/ftp.h"

void drawCredits(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	volt_draw_texture(CreditsImage, 0, 0);
	
	drawBgBot();
	drawBarsBot();
	volt_draw_texture(BackIcon, 293, 213);
	displayTime();
	volt_end_draw();
}

void saveMsg(void) {
	displayMsg("Settings Saved!\n");
	for (int i = 0; i < 60*2; i++) {
		gspWaitForVBlank();
	}
}

//void ftpLogic(void) { // To-Do


void drawFTPScreen(void) {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "FTP Mode");
	drawBgBot();
	drawBarsBot();
	displayTime();
	volt_end_draw();
}

bool confirmPopup(std::string msg) {
	return confirmPopup(msg, "", "Yes", "No", 245);
}

// NOTE: This'll get the app stuck in a loop while its running, so background
// processes like the clock won't update while the message bubble is up
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos) {
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	volt_draw_rectangle(50, 60, 300, 120, settings.universal.bars);
	volt_draw_text_center(GFX_TOP, 90, 0.45f, 0.45f, WHITE, msg1.c_str());
	volt_draw_text_center(GFX_TOP, 110, 0.45f, 0.45f, WHITE, msg2.c_str());
	volt_draw_text(ynXPos, 160, 0.45f, 0.45f, WHITE, ("\uE001 : "+no+"   \uE000 : "+yes).c_str());
	volt_end_draw();
	while(1) {
		gspWaitForVBlank();
		hidScanInput();
		if(keysDown() & KEY_A) {
			return true;
		} else if(keysDown() & KEY_B) {
			return false;
		}
	}
}
