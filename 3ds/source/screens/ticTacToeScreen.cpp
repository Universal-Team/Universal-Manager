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
#include <3ds.h>
#include <citro2d.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "sound.h"
//#include "keyboard.hpp" // Maybe for the Future?

#define COLOR1 C2D_Color32(137, 207, 240, 255) // Color Player 1.
#define COLOR2 C2D_Color32(3, 192, 60, 255) // Color Player 2.

// Modes.
inline bool multiPlayerMode = 0;

// Sub Menu.
inline int subMenu = 1;
inline int selection = 0;

// Scores.
inline int scoreP1 = 0;
inline int scoreP2 = 0;

bool currentPlayer = 0;

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
};

ButtonPos gameBoardPos[] = {
	{80,  40, 50, 50}, {135,  40, 50, 50}, {190,  40, 50, 50},
	{80,  95, 50, 50}, {135,  95, 50, 50}, {190,  95, 50, 50},
	{80, 150, 50, 50}, {135, 150, 50, 50}, {190, 150, 50, 50},
};

int gameBoard[] = {
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
};

struct tri {
	int one;
	int two;
	int three;
} winIndexes[] = {
	{0, 1, 2}, {3, 4, 5}, {6, 7, 8},
	{0, 3, 6}, {1, 4, 7}, {2, 5, 8},
	{0, 4, 8}, {2, 4, 6},
};

extern bool touching(touchPosition touch, ButtonPos button);

void resetBoard() {
	for(uint i=0;i<(sizeof(gameBoard)/sizeof(gameBoard[0]));i++) {
		gameBoard[i] = 0;
		currentPlayer = 0;
	}
}

// Score stuff for Player 1.
static void drawScoreP1(void) {
	Draw_Text(0, 0, 0.72f, WHITE, ("Score : "+std::to_string(scoreP1)).c_str());
}

// Score stuff for Player 2.
static void drawScoreP2(void) {
	Draw_Text(320-Draw_GetTextWidth(0.72f, ("Score : "+std::to_string(scoreP2)).c_str()), 0, 0.72f, WHITE, ("Score : "+std::to_string(scoreP2)).c_str());
}

// Draws the Circles for Player 1.
static void drawPlayer1() {
	for(uint i=0;i<(sizeof(gameBoardPos)/sizeof(gameBoardPos[0]));i++) {
		if(gameBoard[i] == 1) {
			C2D_DrawCircle(gameBoardPos[i].x+25, gameBoardPos[i].y+25, 1.0f, 15, COLOR1, COLOR1, COLOR1, COLOR1);
		}
	}
}

// Draws the Circles for Player 2.
static void drawPlayer2() {
	for(uint i=0;i<(sizeof(gameBoardPos)/sizeof(gameBoardPos[0]));i++) {
		if(gameBoard[i] == 2) {
			C2D_DrawCircle(gameBoardPos[i].x+25, gameBoardPos[i].y+25, 1.0f, 15, COLOR2, COLOR2, COLOR2, COLOR2);
		}
	}
}

// Draw the Field for it.
static void drawField(void) {
	for(uint i=0;i<(sizeof(gameBoardPos)/sizeof(gameBoardPos[0]));i++) {
		C2D_DrawRectSolid(gameBoardPos[i].x, gameBoardPos[i].y, 1.0f, 50, 50, BLACK);
	}
}

// The actual Screen.
void drawScreen(void) {
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	drawScoreP1();
	drawScoreP2();

	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	drawField();
	drawPlayer1();
	drawPlayer2();
}

// Sub Menu Stuff.
//##############################################################################
static void drawSelection(void) {
	if (selection == 0) {
		C2D_DrawCircleSolid(60, 50, 1.0f, 10, Config::barColor);
	} else if (selection == 1) {
		C2D_DrawCircleSolid(60, 115, 1.0f, 10, Config::barColor);
	} else if (selection == 2) {
		C2D_DrawCircleSolid(60, 185, 1.0f, 10, Config::barColor);
	}
}


static void drawSubMenu(void) {
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	Draw_Text(120, 0, 0.72f, WHITE, "Tic-Tac-Toe");
	Draw_Text(70, 218, 0.72f, WHITE, "Universal-Manager Edition");
	

	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
	Gui::DrawBarsBot();

	// Draw all of the 3 Buttons.
	Gui::sprite(sprites_mainMenuButton_idx, 90, 25);
	Draw_Text(110, 42, 0.65f, WHITE, "1 Player Mode");

	Gui::sprite(sprites_mainMenuButton_idx, 90, 90);
	Draw_Text(110, 107, 0.65f, WHITE, "2 Player Mode");

	Gui::sprite(sprites_mainMenuButton_idx, 90, 160);
	Draw_Text(100, 177, 0.7f, WHITE, "Exit Tic-Tac-Toe");
	drawSelection();
}

static void selectionLogic(u32 hDown, u32 hHeld) {
	if (hDown & KEY_UP) {
		playPongSfx();
		if(selection > 0)	selection--;
	} else if (hDown & KEY_DOWN) {
		playPongSfx();
		if(selection < 2)	selection++;
	} else if (hDown & KEY_A) {
		playScoreSfx();
		switch(selection) {
			case 0: {
				multiPlayerMode = 0;
				subMenu = 0;
				selection = 0;
				break;
			} case 1:
				multiPlayerMode = 1;
				subMenu = 0;
				selection = 0;
				break;
			case 2: {
				screenTransition(gameSubMenuScreen);
				selection = 0;
				break;
			}
		}
	}
}
//##############################################################################

// Screen Selection stuff.
void drawTicTacToeScreen(void) {
	if (subMenu == 0) {
		drawScreen();
	} else if (subMenu == 1) {
		drawSubMenu();
	}
}

void ticTacToeLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (subMenu == 1) {
		selectionLogic(hDown, hHeld);
	}

	if(subMenu == 0) {
		for(uint i=0;i<(sizeof(winIndexes)/sizeof(winIndexes[0]));i++) {
			if(gameBoard[winIndexes[i].one] != 0 && gameBoard[winIndexes[i].one] == gameBoard[winIndexes[i].two] && gameBoard[winIndexes[i].two] == gameBoard[winIndexes[i].three]) {
				confirmPopup("Player "+std::to_string(gameBoard[winIndexes[i].one])+" wins!");
				resetBoard();
				subMenu = 1;
				return;
			}
		}

		bool canContinue = false;
		for(uint i=0;i<(sizeof(gameBoard)/sizeof(gameBoard[0]));i++) {
			if(gameBoard[i] == 0) {
				canContinue = true;
			}
		}
		if(!canContinue) {
			confirmPopup("Game over...");
			resetBoard();
			subMenu = 1;
			return;
		}

		if(hDown & KEY_TOUCH) {
			for(uint i=0;i<(sizeof(gameBoardPos)/sizeof(gameBoardPos[0]));i++) {
				if(touching(touch, gameBoardPos[i]) && gameBoard[i] == 0) {
					gameBoard[i] = currentPlayer+1;
					if(multiPlayerMode)	currentPlayer = !currentPlayer;
					else {
						for(uint i=0;i<(sizeof(gameBoard)/sizeof(gameBoard[0]));i++) {
							if(gameBoard[i] == 0) { // Make sure there's an empty spot
								while(1) {
									int pos = rand() % (sizeof(gameBoard)/sizeof(gameBoard[0]));
									if(gameBoard[pos] == 0) {
										gameBoard[pos] = 2;
										break;
									}
								}
								break;
							}
						}
					}
				}
			}
		}

		if(hDown & KEY_START) {
			if(confirmPopup("Do you want to return to the Sub Menu?")) {
				subMenu = 1;
			}
		}
	}
}