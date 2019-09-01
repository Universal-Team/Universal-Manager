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

#include "screens/pongScreen.hpp"
#include "screens/screenCommon.hpp"

#include "utils/keyboard.hpp"
#include "utils/settings.hpp"
#include "utils/sound.h"

#include <3ds.h>
#include <algorithm>
#include <citro2d.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>


// Score stuff for Player 1.
void Pong::drawScoreP1(void) const
{
	Gui::DrawString(0, 0, 0.72f, WHITE, ("Score : "+std::to_string(scoreP1)).c_str());
}


// Score stuff for Player 2.
void Pong::drawScoreP2(void) const
{
	Gui::DrawString(320-Gui::GetStringWidth(0.72f, ("Score : "+std::to_string(scoreP2)).c_str()), 0, 0.72f, WHITE, ("Score : "+std::to_string(scoreP2)).c_str());
}


// Draws the Paddles for both Players.
void Pong::drawPaddle(void) const
{
	C2D_DrawRectSolid(10, paddle1, 1.0f, 10, 60, Config::barColor);
	C2D_DrawRectSolid(380, paddle2, 1.0f, 10, 60, Config::barColor);
}


// Draws the ball.
void Pong::drawBall(void) const
{
	C2D_DrawCircleSolid(ballX, ballY, 1.0f, 5, Config::barColor);
}


// This is for the Sub Menu.
//#######################################################################################
void Pong::drawSelection(void) const
{
	if (Selection == 0) {
		Gui::drawGUISelector(button_selector_idx, 94, 29, .020f);
	} else if (Selection == 1) {
		Gui::drawGUISelector(button_selector_idx, 94, 94, .020f);
	} else if (Selection == 2) {
		Gui::drawGUISelector(button_selector_idx, 94, 164, .020f);
	}
}


void Pong::drawSubMenu(void) const
{
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	Gui::DrawString(180, 0, 0.72f, WHITE, "Pong");
	Gui::DrawString(70, 218, 0.72f, WHITE, "Universal-Manager Edition");

	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
	Gui::DrawBarsBot();

	// Draw all of the 3 Buttons.
	Gui::sprite(3, button_button_2_idx, 90, 25); 
	Gui::DrawString(110, 42, 0.65f, WHITE, "1 Player Mode");

	Gui::sprite(3, button_button_2_idx, 90, 90); 
	Gui::DrawString(110, 107, 0.65f, WHITE, "2 Player Mode");

	Gui::sprite(3, button_button_2_idx, 90, 160); 
	Gui::DrawString(130, 177, 0.7f, WHITE, "Exit Pong");
	drawSelection();
}

void Pong::selectionLogicPong(u32 hDown, u32 hHeld) {
	if (hDown & KEY_UP) {
		playPongSfx();
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		playPongSfx();
		if(Selection < 2)	Selection++;
	} else if (hDown & KEY_A) {
		playScoreSfx();
		switch(Selection) {
			case 0: {
				if(confirmPopup("Would you like to set the speed?")) {
					speed1 = Input::getUint(9, "Please Type in the Speed Value.");
					speed2 = - + speed1;
				} else {
					speed1 = 5;
					speed2 = -5;
				}
				multiPlayerMode = 0;
				subMenu = 0;
				Selection = 0;
				break;
			} case 1:
				if(confirmPopup("Would you like to set the speed?")) {
					speed1 = Input::getUint(9, "Please Type in the Speed Value.");
					speed2 = - + speed1;
				} else {
					speed1 = 5;
					speed2 = -5;
				}

				multiPlayerMode = 1;
				subMenu = 0;
				Selection = 0;
				break;
			case 2: {
				Gui::screenBack();
				return;
				Selection = 0;
				break;
			}
		}
	}
}
//#######################################################################################

// This is the actual Screen.
//#######################################################################################
void Pong::drawScreen(void) const
{
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 1.0f, 400, 240, Config::bgColor);

	drawBall();
	drawPaddle();

	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
	Gui::DrawBarsBot();

	drawScoreP1();
	drawScoreP2();

	Gui::DrawString(80, 80, 0.72f, WHITE, "How to Play :");
	Gui::DrawString(80, 100, 0.72f, WHITE, "Player 1 : Up / Down");
	Gui::DrawString(80, 120, 0.72f, WHITE, "Player 2 : X / B");
	Gui::DrawString(80, 140, 0.72f, WHITE, "Start : Quit");
}

//#######################################################################################


// Screen Selection.
void Pong::Draw(void) const
{
	if (subMenu == 0) {
		drawScreen();
	} else if (subMenu == 1) {
		drawSubMenu();
	}
}


// Player 1 Control.
void Pong::player1Control(u32 hDown, u32 hHeld) {
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
void Pong::player2Control(u32 hDown, u32 hHeld) {
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

void Pong::ballLogic(void) {
		ballX += ballXSpd;
		ballY += ballYSpd;

		if ((ballX <  20 && ballX >  10 && ballY > paddle1 && ballY < paddle1+60) ||
			(ballX > 380 && ballX < 390 && ballY > paddle2 && ballY < paddle2+60)) {
				playPongSfx();

				if(ballX < 20 && ballX > 10)	ballXSpd = speed1; // First speed to player 2.

				if(ballX < 390 && ballX > 380)	ballXSpd = speed2; // The second speed to player 1.

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

void Pong::stopLogic(void) {
			paddle1 = 90;
			paddle2 = 90;
}

void Pong::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (subMenu == 1) {
		selectionLogicPong(hDown, hHeld);
	}

	if (subMenu == 0) {
	if (scoreP1 == 10) {
		stopLogic();
		confirmPopup("Player 1 wins!\n\nPress A or B to continue to the subMenu.");
		subMenu = 1;
		scoreP1 = 0;
		scoreP2 = 0;
		return;
	} else if (scoreP2 == 10) {
		stopLogic();
		confirmPopup("Player 2 wins!\n\nPress A or B to continue to the subMenu.");
		subMenu = 1;
		scoreP1 = 0;
		scoreP2 = 0;
		return;
	} else if (scoreP1 < 10 || scoreP2 < 10) {
		ballLogic();
	}
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