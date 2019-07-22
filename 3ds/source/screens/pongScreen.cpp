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

// Paddle and Ball Positions.
int paddle1 = 90, paddle2 = 90;
double	ballX = 200, ballY = 100,
		ballXSpd = -3, ballYSpd = 0;

// Modes.
int multiPlayerMode = 0;

// Sub Menu.
int subMenu = 1;
int selection = 0;

// Scores.
int scoreP1 = 0;
int scoreP2 = 0;


// Score stuff for Player 1.
static void drawScoreP1(void) {
	Draw_Text(0, 0, 0.72f, WHITE, ("Score : "+std::to_string(scoreP1)).c_str());
}


// Score stuff for Player 2.
static void drawScoreP2(void) {
	Draw_Text(320-Draw_GetTextWidth(0.72f, ("Score : "+std::to_string(scoreP2)).c_str()), 0, 0.72f, WHITE, ("Score : "+std::to_string(scoreP2)).c_str());
}


// Draws the Paddles for both Players.
static void drawPaddle(void) {
	C2D_DrawRectSolid(10, paddle1, 1.0f, 10, 60, Config::barColor);
	C2D_DrawRectSolid(380, paddle2, 1.0f, 10, 60, Config::barColor);
}


// Draws the ball.
static void drawBall(void) {
	C2D_DrawCircleSolid(ballX, ballY, 1.0f, 5, Config::barColor);
}


// This is for the Sub Menu.
//#######################################################################################
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
	Draw_Text(180, 0, 0.72f, WHITE, "Pong");
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
	Draw_Text(130, 177, 0.7f, WHITE, "Exit Pong");
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
//#######################################################################################


static void drawEndScreen(void) {
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 1.0f, 400, 240, Config::bgColor);
	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
	Gui::DrawBarsBot();
	drawScoreP1();
	drawScoreP2();
	Draw_Text(80, 140, 0.72f, WHITE, "Select : Restart");
}

// This is the actual Screen.
//#######################################################################################
static void drawScreen(void) {
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 1.0f, 400, 240, Config::bgColor);

	drawBall();
	drawPaddle();

	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
	Gui::DrawBarsBot();

	drawScoreP1();
	drawScoreP2();

	Draw_Text(80, 80, 0.72f, WHITE, "How to Play :");
	Draw_Text(80, 100, 0.72f, WHITE, "Player 1 : Up / Down");
	Draw_Text(80, 120, 0.72f, WHITE, "Player 2 : X / B");
	Draw_Text(80, 140, 0.72f, WHITE, "Start : Quit");
}

//#######################################################################################


// Screen Selection.
void drawPongScreen(void) {
	if (subMenu == 0) {
		drawScreen();
	} else if (subMenu == 1) {
		drawSubMenu();
	} else if (subMenu == 2) {
		drawEndScreen();
	}
}


// Player 1 Control.
static void player1Control(u32 hDown, u32 hHeld) {
	if (hDown & KEY_DOWN) {
		if(paddle1 < 181)	paddle1 += 6.0;
		if (paddle1 == 180)	paddle1 += 0.0;
	} else if (hDown & KEY_UP) {
		if(paddle1 > -1)	paddle1 -= 6.0;
		if (paddle1 == 0)	paddle1 -= 0.0;
	} else if (hHeld & KEY_DOWN) {
		if(paddle1 < 181)	paddle1 += 6.0;
		if (paddle1 == 180)	paddle1 += 0.0;
	} else if (hHeld & KEY_UP) {
		if(paddle1 > -1)	paddle1 -= 6.0;
		if (paddle1 == 0)	paddle1 -= 0.0;
	}
}


// Player 2 Control.
static void player2Control(u32 hDown, u32 hHeld) {
		if (hDown & KEY_B) {
		if(paddle2 < 181)	paddle2 += 6.0;
		if (paddle2 == 180)	paddle2 += 0.0;
	} else if (hDown & KEY_X) {
		if(paddle2 > -1)	paddle2 -= 6.0;
		if (paddle2 == 0)	paddle2 -= 0.0;
	} else if (hHeld & KEY_B) {
		if(paddle2 < 181)	paddle2 += 6.0;
		if (paddle2 == 180)	paddle2 += 0.0;
	} else if (hHeld & KEY_X) {
		if(paddle2 > -1)	paddle2 -= 6.0;
		if (paddle2 == 0)	paddle2 -= 0.0;
	}
}

static void ballLogic(void) {
		ballX += ballXSpd;
		ballY += ballYSpd;

		if ((ballX <  20 && ballX >  10 && ballY > paddle1 && ballY < paddle1+60) ||
			(ballX > 380 && ballX < 390 && ballY > paddle2 && ballY < paddle2+60)) {
			playPongSfx();
			ballXSpd = ballXSpd * -1.1;
			ballYSpd += -(((ballX < 200 ? paddle1 : paddle2)+30)-ballY)/20;
		}

		if (ballY < 0 || ballY > 230) {
			playPongSfx();
			ballYSpd = -ballYSpd;
		}

		if (ballX < 0) {
			playScoreSfx();
			scoreP2++;
			ballX = 200;
			ballY = 100;
			ballXSpd = 3;
			ballYSpd = 0;
			paddle1 = 90;
			paddle2 = 90;
		} else if (ballX > 400) {
			playScoreSfx();
			scoreP1++;
			ballX = 200;
			ballY = 100;
			ballXSpd = -3;
			ballYSpd = 0;
			paddle1 = 90;
			paddle2 = 90;
		}
}

static void stopLogic(void) {
			paddle1 = 90;
			paddle2 = 90;
}

void pongLogic(u32 hDown, u32 hHeld) {
	if (subMenu == 1) {
		selectionLogic(hDown, hHeld);
	}

	if (subMenu == 0) {
	if (scoreP1 == 10 || scoreP2 == 10) {
		stopLogic();
		subMenu = 2;
	} else if (scoreP1 < 10 || scoreP2 < 10) {
		ballLogic();
	}
	}

	if (subMenu == 2 && hDown & KEY_SELECT) {
		subMenu = 0;
		paddle1 = 90;
		paddle2 = 90;
		scoreP1 = 0;
		scoreP2 =0;
	}

	if (subMenu == 0) {
		player1Control(hDown, hHeld);
		if(multiPlayerMode == 0) {
			if(ballY < paddle2+30 && paddle2 > 0)	paddle2--;
			else if(paddle2 < 180)	paddle2++;
		} else if (multiPlayerMode == 1) {
			player2Control(hDown, hHeld);
		}

	if (subMenu == 0 && hDown & KEY_START) {
		if(confirmPopup("Do you want to return to the Sub Menu?")) {
			subMenu = 1;
			paddle1 = 90;
			paddle2 = 90;
			scoreP1 = 0;
			scoreP2 =0;
		}
	}
}
}