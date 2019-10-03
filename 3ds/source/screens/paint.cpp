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

#include "screens/paint.hpp"
#include "screens/screenCommon.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>


void Paint::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Universal-Manager Paint!"))/2, 0, 0.72f, WHITE, "Universal-Manager Paint!");

	set_screen(bottom);
	Draw_Rect(0, 0, 320, 240, WHITE);
	Draw_Rect(drawX, drawY, drawX2, drawY2, BLACK);
}

void Paint::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (hHeld & KEY_TOUCH) {
		drawX = touch.px;
		drawY = touch.py;
		drawX2 = touch.px+50;
		drawY2 = touch.py+50;
	} else if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}