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

#include <algorithm>
#include <fstream>
#include <unistd.h>



// NOTE: This'll get the app stuck in a loop while its running, so background
// processes like the clock won't update while the message bubble is up
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLUE2);
	C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	C2D_DrawRectSolid(0, 60, 0.5f, 400, 120, WHITE);
	Gui::DrawString(200-(Gui::GetStringWidth(0.45f, msg1.c_str())/2), 90, 0.45f, BLACK, msg1.c_str());
	Gui::DrawString(200-(Gui::GetStringWidth(0.45f, msg1.c_str())/2), 110, 0.45f, BLACK, msg2.c_str());
	Gui::DrawString(ynXPos, 160, 0.45f, BLACK, ("B : "+no+"   A : "+yes).c_str());
	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	C3D_FrameEnd(0);
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

void helperBox(std::string msg1) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	set_screen(top);
	Draw_Rect(0, 0, 400, 240, DIM);
	Gui::sprite(sprites_textbox_idx, 10, 25);
	Gui::DrawString(35, 42, 0.45f, BLACK, msg1.c_str());
	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

bool confirmPopup(std::string msg) {
	return confirmPopup(msg, "", "Yes", "No", 100);
}