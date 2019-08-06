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
#include <vector>
#include "buttonTester.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void ButtonTester::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Button Tester"))/2, 0, 0.72f, WHITE, "Button Tester");
	Draw_Text(80, 218, 0.72f, WHITE, "Currently Pressed Button : ");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void ButtonTester::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE000");

	} else if (hDown & KEY_B) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE001");

	} else if (hDown & KEY_X) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE002");

	} else if (hDown & KEY_Y) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE003");

	} else if (hDown & KEY_UP) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE079");

		} else if (hDown & KEY_DOWN) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE07A");

		} else if (hDown & KEY_LEFT) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE07B");

		} else if (hDown & KEY_RIGHT) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE07C");

		} else if (hDown & KEY_L) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE052");

		} else if (hDown & KEY_R) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE053");

		} else if (hDown & KEY_ZL) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE054");

		} else if (hDown & KEY_ZR) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE055");

		} else if (hDown & KEY_START) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE045");

		} else if (hDown & KEY_SELECT) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		set_screen(top);
		Draw_Text(330, 218, 0.72f, WHITE, "\uE046");

	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, buttonTesterButtonPos[0])) {
			Gui::screenBack();
			return;
	}
}
}