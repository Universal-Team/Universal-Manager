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
#include "fileBrowse.h"

extern "C" {
#include "C2D_helper.h"
}

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

ButtonPos FunctionPos[] = {
    {59, 70, 93, 35, -1},
    {165, 70, 93, 35, -1},
    {59, 110, 93, 35, -1},
	{165, 110, 93, 35, -1},
	{59, 150, 93, 35, -1},
	{165, 150, 93, 35, -1},
};



bool displayActionBox(void) {
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    set_screen(bottom);
	//Gui::Draw_ImageBlend(sprites_actionBox_idx, 54, 30, settings.universal.bars);
	C2D_DrawRectSolid(54, 30, 0.5f, 211, 180, settings.universal.bars);
	Gui::sprite(sprites_actionBox_idx, 54, 30);

	// Buttons.
	Gui::sprite(sprites_FileManagerButton_idx, FunctionPos[0].x, FunctionPos[0].y);
	draw_text(FunctionPos[0].x+12, FunctionPos[0].y+10, 0.6f, 0.6f, WHITE, "Rename");
	Gui::sprite(sprites_FileManagerButton_idx, FunctionPos[1].x, FunctionPos[1].y);
	draw_text(FunctionPos[1].x+12, FunctionPos[1].y+10, 0.6f, 0.6f, WHITE, "Delete");
	Gui::sprite(sprites_FileManagerButton_idx, FunctionPos[2].x, FunctionPos[2].y);
	//draw_text(FunctionPos[2].x+12, FunctionPos[2].y+10, 0.6f, 0.6f, WHITE, "");
	Gui::sprite(sprites_FileManagerButton_idx, FunctionPos[3].x, FunctionPos[3].y);
	//draw_text(FunctionPos[3].x+12, FunctionPos[3].y+10, 0.6f, 0.6f, WHITE, "");
	Gui::sprite(sprites_FileManagerButton_idx, FunctionPos[4].x, FunctionPos[4].y);
	//draw_text(FunctionPos[4].x+12, FunctionPos[4].y+10, 0.6f, 0.6f, WHITE, "");
	Gui::sprite(sprites_FileManagerButton_idx, FunctionPos[5].x, FunctionPos[5].y);
	//draw_text(FunctionPos[5].x+12, FunctionPos[5].y+10, 0.6f, 0.6f, WHITE, "");

    Draw_EndFrame();
    while(1) {
		gspWaitForVBlank();
		hidScanInput();
		if(keysDown() & KEY_A) {
			return true;
		} else if(keysDown() & KEY_B) {
			return false;
		}
    }
}