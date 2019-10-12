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


// Display a Message, which can be skipped with A.
void DisplayWaitMsg(std::string waitMsg, ...)
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLUE2);
	C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.6f, "Press A to continue."))/2, 217, 0.6f, WHITE, "Press A to continue.");
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, waitMsg.c_str()))/2, 100, 0.72f, WHITE, waitMsg.c_str());
	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	C3D_FrameEnd(0);

	while(1)
	{
		hidScanInput();
		if(hidKeysDown() & KEY_A)
			break;
	}
}

// Display the Helper Box with dimmed Background effect.
void helperBox(std::string Msg) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	set_screen(top);
	Draw_Rect(0, 0, 400, 240, DIM);

	Draw_Rect(0, 25, 400, 190, C2D_Color32(128, 128, 128, 255));
	Draw_Rect(5, 30, 390, 180, C2D_Color32(0, 0, 0, 190));

	Gui::DrawString(10, 40, 0.45f, WHITE, Msg, 380);
	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, DIM);
	C3D_FrameEnd(0);
}

// NOTE: This'll get the app stuck in a loop while its running, so background
// processes like the clock won't update while the message bubble is up
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLUE2);
	C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	C2D_DrawRectSolid(0, 60, 0.5f, 400, 120, WHITE);
	Gui::DrawString((400-Gui::GetStringWidth(0.45f, msg1.c_str()))/2, 90, 0.45f, BLACK, msg1.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.45f, msg2.c_str()))/2, 110, 0.45f, BLACK, msg2.c_str());
	Gui::DrawString((400-Gui::GetStringWidth(0.45f, ("A: "+yes+"   B: "+no).c_str()))/2, 160, 0.45f, BLACK, ("A: "+yes+"   B: "+no).c_str());
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

// For single confirm Popups.
bool confirmPopup(std::string msg) {
	return confirmPopup(msg, "", "Yes", "No");
}

// Display a Message for 3 seconds.
void DisplayTimeMessage(std::string Text, int Time)
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, BLUE2);
	C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, Text.c_str()))/2, 2, 0.72f, WHITE, Text.c_str());
	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	C3D_FrameEnd(0);
	for (int i = 0; i < 60*Time; i++) {
		gspWaitForVBlank();
	}
}