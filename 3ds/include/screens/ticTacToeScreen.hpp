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

#include "screen.hpp"
#include <vector>
#include "structs.hpp"


class TicTacToe : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	bool touching(touchPosition touch, Structs::TTTPos button);
	void drawField(void) const;

	void drawSubMenu(void) const;
	void drawScreen(void) const;
	void drawSelection(void) const;
	void resetBoard();

	void selectionLogic(u32 hDown, u32 hHeld);
	void drawPlayer1() const;
	void drawPlayer2() const;

	// Modes.
	mutable bool multiPlayerMode = 0;

	// Sub Menu.
	int subMenu = 1;
	int selection = 0;

	mutable bool currentPlayer = 0;

	std::vector<Structs::TTTPos> gameBoardPos = {
		{80,  40, 50, 50}, {135,  40, 50, 50}, {190,  40, 50, 50},
		{80,  95, 50, 50}, {135,  95, 50, 50}, {190,  95, 50, 50},
		{80, 150, 50, 50}, {135, 150, 50, 50}, {190, 150, 50, 50},
	};

	std::vector<int> gameBoard = {
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
	};

	struct tri {
		int one;
		int two;
		int three;
	};

	std::vector<tri> winIndexes = {
		{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
		{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
		{0, 4, 8}, {2, 4, 6},
	}; 
};