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

#include "screens/buttonTester.hpp"
#include "screens/screenCommon.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void ButtonTester::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Button Tester"))/2, 0, 0.72f, WHITE, "Button Tester");
	std::string pressedBtn = "Currently Pressed Button: ";
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, pressedBtn.c_str()))/2, 218, 0.72f, WHITE, pressedBtn.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();
}


void ButtonTester::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hHeld & KEY_A) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE000");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_B) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE001");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_X) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE002");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_Y) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE003");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_UP) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE079");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_DOWN) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE07A");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_LEFT) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE07B");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_RIGHT) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE07C");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_L) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE052");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_R) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE053");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_ZL) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE054");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_ZR) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE055");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_START) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE045");
		C3D_FrameEnd(0);

	} else if (hHeld & KEY_SELECT) {
		Gui::clearTextBufs();
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Gui::DrawString(330, 218, 0.72f, WHITE, "\uE046");
		C3D_FrameEnd(0);

	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, buttonTesterButtonPos[0])) {
			Gui::screenBack();
			return;
	}
}
}