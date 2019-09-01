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

#include "screens/calculatorScreen.hpp"
#include "screens/screenCommon.hpp"

#include "utils/settings.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>


// To-Do.
void Calculator::drawCalculatorKeyboard(void) const
{
	for(uint i=0;i<(sizeof(calculatorKeys)/sizeof(calculatorKeys[0]));i++) {
		C2D_DrawRectSolid(calculatorKeys[i].x, calculatorKeys[i].y, 0.5f, 45, 40, Config::barColor & C2D_Color32(255, 255, 255, 200));
		char c[2] = {calculatorKeys[i].character[0]};
		Gui::DrawString(calculatorKeys[i].x+18, calculatorKeys[i].y+10, 0.72f, WHITE, c);
	}
}


void Calculator::Draw(void) const
{
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Calculator"))/2, 0, 0.72f, WHITE, "Calculator");

	// Display Box with input.
	Draw_Rect(40, 60, 320, 80, Config::barColor);
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, display.c_str()))/2-160+140, 90, 0.72f, WHITE, display.c_str());


	C2D_SceneBegin(bottom);
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();
	drawCalculatorKeyboard();
}

// To-Do -> Calculator Logic.
void Calculator::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}