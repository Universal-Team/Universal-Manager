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
int paddle1 = 0, paddle2 = 0;
int ballX = 200, ballY = 100;
int multiPlayerMode = 1;


struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

extern bool touching(touchPosition touch, ButtonPos button);

ButtonPos pongButtonPos[] = {
	{293, 213, 27, 27, mainScreen},
};


static void drawPaddle(void) {
	C2D_DrawRectSolid(10, paddle1, 1.0f, 10, 60, Config::barColor);
	C2D_DrawRectSolid(380, paddle2, 1.0f, 10, 60, Config::barColor);
}


static void drawBall(void) {
	C2D_DrawCircleSolid(ballX, ballY, 1.0f, 5, Config::barColor);
}


void drawPongScreen(void) {
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 1.0f, 400, 240, Config::bgColor);

	drawBall();
	drawPaddle();

	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
	Draw_Text(0, 0, 0.72f, WHITE, "Score : 0");
	Draw_Text(220, 0, 0.72f, WHITE, "Score : 0");
	Gui::DrawBarsBottomBack();
}


static void player1Control(u32 hDown, u32 hHeld) {
	if (hDown & KEY_DOWN) {
		if(paddle1 < 181)	paddle1 += 4.0;
		if (paddle1 == 180)	paddle1 += 0.0;
	} else if (hDown & KEY_UP) {
		if(paddle1 > -1)	paddle1 -= 4.0;
		if (paddle1 == 0)	paddle1 -= 0.0;
	} else if (hHeld & KEY_DOWN) {
		if(paddle1 < 181)	paddle1 += 4.0;
		if (paddle1 == 180)	paddle1 += 0.0;
	} else if (hHeld & KEY_UP) {
		if(paddle1 > -1)	paddle1 -= 4.0;
		if (paddle1 == 0)	paddle1 -= 0.0;
}
}


static void player2Control(u32 hDown, u32 hHeld) {
		if (hDown & KEY_B) {
		if(paddle2 < 181)	paddle2 += 4.0;
		if (paddle2 == 180)	paddle2 += 0.0;
	} else if (hDown & KEY_X) {
		if(paddle2 > -1)	paddle2 -= 4.0;
		if (paddle2 == 0)	paddle2 -= 0.0;
	} else if (hHeld & KEY_B) {
		if(paddle2 < 181)	paddle2 += 4.0;
		if (paddle2 == 180)	paddle2 += 0.0;
	} else if (hHeld & KEY_X) {
		if(paddle2 > -1)	paddle2 -= 4.0;
		if (paddle2 == 0)	paddle2 -= 0.0;
}
}


// To-Do -> Do the whole Pong Logic inside it.
void pongLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (multiPlayerMode == 0) {
		player1Control(hDown, hHeld);
	}

	if (multiPlayerMode == 1) {
		player1Control(hDown, hHeld);
		player2Control(hDown, hHeld); 
	}


	if (hDown & KEY_TOUCH) {
	if (touching(touch, pongButtonPos[0])) {
			screenMode = pongButtonPos[0].link;
	}
}
}