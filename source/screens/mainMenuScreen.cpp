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


// Version numbers.
char universal_manager_vertext[13];

void drawMainMenu(void) {
	// Initialize the Version Number.
	snprintf(universal_manager_vertext, 13, "v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

	C2D_SceneBegin(g_renderTargetTop);
	C2D_DrawRectSolid(0, 25, 400, 0.5f, 190, BLUE);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
	Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Gui::sprite(sprites_top_screen_bot_idx, 0, 215);
	Gui::staticText("Universal-Manager", 200, 3, FONT_SIZE_18, FONT_SIZE_18, WHITE, TextPosX::CENTER, TextPosY::TOP);
	Gui::staticText(universal_manager_vertext, 340, 218, FONT_SIZE_18, FONT_SIZE_18, BLACK, TextPosX::CENTER, TextPosY::TOP);
	//Gui::volt_draw_wtext(100, 3, 0.72f, WHITE, TR(STR_MAINMENU_TITLE));

	C2D_SceneBegin(g_renderTargetBottom);
	C2D_DrawRectSolid(0, 25, 0.5f, 320, 190, BLUE);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);

}
