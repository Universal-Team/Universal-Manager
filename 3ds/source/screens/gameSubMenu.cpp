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

#include "screens/gameSubMenu.hpp"
#include "screens/pongScreen.hpp"
#include "screens/screenCommon.hpp"
#include "screens/ticTacToeScreen.hpp"
#include "utils/settings.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>


extern bool touching(touchPosition touch, Structs::ButtonPos button);

void GameSub::drawSelection(void) const
{
	if (Selection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 12, 20, Config::barColor);
	} else if (Selection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 182, 20, Config::barColor);
	}
}


void GameSub::Draw(void) const
{
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Universal-Manager"))/2, 0, 0.72f, WHITE, "Universal-Manager");
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Games Tab"))/2, 220, 0.72f, WHITE, "Games Tab");

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();

	// Buttons.
	Gui::sprite(sprites_mainMenuButton_idx, gamesSubMenuButtonPos[0].x, gamesSubMenuButtonPos[0].y);
	Draw_Text(40, 57, 0.7f, WHITE, "Pong");

	Gui::sprite(sprites_mainMenuButton_idx, gamesSubMenuButtonPos[1].x, gamesSubMenuButtonPos[1].y);
	Draw_Text(210, 57, 0.65f, WHITE, "Tic-Tac-Toe");

	drawSelection();
}

void GameSub::SelectionLogic(u32 hDown) {
		if (hDown & KEY_UP) {
			if(Selection > 0)	Selection--;
		} else if (hDown & KEY_DOWN) {
			if(Selection < 1)	Selection++;
		} else if (hDown & KEY_A) {
			switch(Selection) {
				case 0: {
					Gui::setScreen(std::make_unique<Pong>());
					break;
				} case 1:
					Gui::setScreen(std::make_unique<TicTacToe>());
					break;
		}
		}
}

void GameSub::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	SelectionLogic(hDown);
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, gamesSubMenuButtonPos[0])) {
			Gui::setScreen(std::make_unique<Pong>());
		} else if (touching(touch, gamesSubMenuButtonPos[1])) {
			Gui::setScreen(std::make_unique<TicTacToe>());
		} else if (touching(touch, gamesSubMenuButtonPos[2])) {
			Gui::screenBack();
			return;
		}
	}
}