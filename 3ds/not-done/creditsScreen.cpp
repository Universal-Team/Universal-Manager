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
#include <citro2d.h>
#include "settings.hpp"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

int dialog = 5; 

// Color Stuff.
#define VOLTZCOLORBAR RGBA8(0, 108, 255, 255)
#define VOLTZCOLORBG RGBA8(0, 205, 199, 255)

#define FLAMECOLORBAR RGBA8(50, 50, 50, 255)
#define FLAMECOLORBG RGBA8(66, 233, 66, 255)

#define PK11COLORBAR RGBA8(100, 100, 255, 255)
#define PK11COLORBG RGBA8(255, 100, 150, 255)

#define ROCKETROBZCOLORBAR RGBA8(127, 127, 127, 255)
#define ROCKETROBZCOLORBG RGBA8(0, 0, 31, 255)

#define TNGCOLORBAR RGBA8(127, 127, 127, 255)
#define TNGCOLORBG RGBA8(0, 0, 31, 255)

std::vector<std::string> names = {
	"VoltZ",
	"FlameKat53",
	"Pk11",
	"RocketRobz",
	"TotallyNotGuy",
	"MainMenu"
};

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};
extern bool touching(touchPosition touch, ButtonPos button);

ButtonPos creditsButtonPos[] = {
    {0, 25, 149, 52, -1},
    {170, 25, 149, 52, -1},
	{0, 90, 149, 52, -1},
	{170, 90, 149, 52, -1},
	{0, 150, 149, 52, -1},
    {170, 150, 149, 52, -1},
};

void drawVoltZ(void) {
	// Top BG Stuff.
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, VOLTZCOLORBG);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, VOLTZCOLORBAR);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Draw_Text(50, 0, 0.72f, WHITE, "Welcome to Universal-Manager!");


	Gui::Credits(credits_textbox_idx, 180, 30);
	Gui::Credits(credits_voltZ_idx, -10, 40);
	Draw_Text(250, 50, 0.40, BLACK, "Hello There!");
	Draw_Text(190, 65, 0.40, BLACK, "Thank you for using Universal-Manager!\nI'm the Main Developer, VoltZ.\nBefore Universal-Manager, it was\nUniversal-Updater. Because it was\nOnly an Updater.. I decided to\nDo an Universal-Manager with\nMuch Functions!\nVisit our Site on : \nhttps://universal-team.github.io/ !\nI like it to work on Universal-Manager! ;)");

	// BG Bottom Stuff.
	set_screen(bottom);
	Draw_Rect(0, 0, 400, 240, VOLTZCOLORBG);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, VOLTZCOLORBAR);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, VOLTZCOLORBAR);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
	Draw_Text((320-Draw_GetTextWidth(0.7f, "VoltZ"))/2, 0, 0.7f, WHITE, "VoltZ");
}

void drawFlame(void) {
	// Top BG Stuff.
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, FLAMECOLORBG);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, FLAMECOLORBAR);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Draw_Text(50, 0, 0.72f, WHITE, "Welcome to Universal-Manager!");


	Gui::Credits(credits_textbox_idx, 180, 30);
	Gui::Credits(credits_flame_idx, 0, 40);
	Draw_Text(240, 50, 0.40, BLACK, "Hey, I’m Flame!");
	Draw_Text(190, 65, 0.40, BLACK, "I’m the main developer of Relaunch!\nI have helped VoltZ with a small bit\n       of code in Universal-Manager.");

	// BG Bottom Stuff.
	set_screen(bottom);
	Draw_Rect(0, 0, 400, 240, FLAMECOLORBG);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, FLAMECOLORBAR);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, FLAMECOLORBAR);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
	Draw_Text((320-Draw_GetTextWidth(0.7f, "Flame"))/2, 0, 0.7f, WHITE, "Flame");
}

void drawPk11(void) {
	// Top BG Stuff.
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, PK11COLORBG);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, PK11COLORBAR);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Draw_Text(50, 0, 0.72f, WHITE, "Welcome to Universal-Manager!");


	Gui::Credits(credits_textbox_idx, 180, 30);
	Gui::Credits(credits_pk11_idx, -20, 40);
	Draw_Text(270, 50, 0.40, BLACK, "Hi .o/");
	Draw_Text(190, 65, 0.40, BLACK, "I'm Pk11, I've helped out with this app\na bit and am the main dev on pkmn-chest,\nanother Universal-Team project which\nyou can install using Universal Manager!");

	// BG Bottom Stuff.
	set_screen(bottom);
	Draw_Rect(0, 0, 400, 240, PK11COLORBG);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, PK11COLORBAR);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, PK11COLORBAR);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
	Draw_Text((320-Draw_GetTextWidth(0.7f, "Pk11"))/2, 0, 0.7f, WHITE, "Pk11");
}

void drawRocketRobz(void) {
	// Top BG Stuff.
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, ROCKETROBZCOLORBG);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, ROCKETROBZCOLORBAR);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Draw_Text(50, 0, 0.72f, WHITE, "Welcome to Universal-Manager!");


	Gui::Credits(credits_textbox_idx, 180, 30);
	Gui::Credits(credits_rocketRobz_idx, 0, 40);
	Draw_Text(270, 50, 0.40, BLACK, "Hi!");
	Draw_Text(190, 65, 0.40, BLACK, "I'm RocketRobz, main developer of\n           TWiLight Menu++.\nSome code I made (not from TWLM++)\n       has made it into this app.\nEnjoy this universally good manager!");

	// BG Bottom Stuff.
	set_screen(bottom);
	Draw_Rect(0, 0, 400, 240, ROCKETROBZCOLORBG);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, ROCKETROBZCOLORBAR);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, ROCKETROBZCOLORBAR);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
	Draw_Text((320-Draw_GetTextWidth(0.7f, "RocketRobz"))/2, 0, 0.7f, WHITE, "RocketRobz");
}

void drawTotallyNotGuy(void) {
	// Top BG Stuff.
	set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, TNGCOLORBG);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, TNGCOLORBAR);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Draw_Text(50, 0, 0.72f, WHITE, "Welcome to Universal-Manager!");


	Gui::Credits(credits_textbox_idx, 180, 30);
	Gui::Credits(credits_totallyNotGuy_idx, 0, 40);
	Draw_Text(250, 50, 0.40, BLACK, "Hello!");
	Draw_Text(190, 65, 0.40, BLACK, "I'm TotallyNotGuy and I did \nmost of the graphics in \nUniversal-Manager and pkmn-chest. \nI hope you enjoy the graphics \nin both Apps!");

	// BG Bottom Stuff.
	set_screen(bottom);
	Draw_Rect(0, 0, 400, 240, TNGCOLORBG);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, TNGCOLORBAR);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, TNGCOLORBAR);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
	Draw_Text((320-Draw_GetTextWidth(0.7f, "TotallyNotGuy"))/2, 0, 0.7f, WHITE, "TotallyNotGuy");
}

void CREDITS::drawCredits(void) const
{
	set_screen(top);
	Gui::Credits(credits_universal_credits_idx, 0, 0);

	set_screen(bottom);
	Draw_Rect(0, 0, 400, 240, GRAY);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, BLUE);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
	Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
	C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, BLUE);
	Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
	Draw_Text(20, 0, 0.7f, WHITE, "Welcome to Universal-Manager!");
}

void CREDITS::drawCreditsDialogs(void) const
{
	if (dialog == 0) {
		drawVoltZ();
	} else if (dialog == 1) {
		drawFlame();
	} else if (dialog == 2) {
		drawPk11();
	} else if (dialog == 3) {
		drawRocketRobz();
	} else if (dialog == 4) {
		drawTotallyNotGuy();
	} else if (dialog == 5) {
		drawCredits();
	}
}

void drawButtons(void) {
	// Buttons.
	for(int i=0;i<3;i++) {
		Gui::sprite(sprites_mainMenuButton_idx, 0, 29+(i*65));
		Draw_Text(((320-Draw_GetTextWidth(0.65f, names[(2*i)].c_str()))/2)-85, 46+(i*65), 0.65f, WHITE, names[(2*i)].c_str());
		Gui::sprite(sprites_mainMenuButton_idx, 170, 29+(i*65));
		Draw_Text(((320-Draw_GetTextWidth(0.65f, names[(2*i)+1].c_str()))/2)+85, 46+(i*65), 0.65f, WHITE, names[(2*i)+1].c_str());
	}
}

void CREDITS::Draw(void) const
{
    C2D_TargetClear(top, GRAY);
    C2D_TargetClear(bottom, GRAY);
	set_screen(top);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
	Draw_Text(50, 0, 0.72f, WHITE, "Welcome to Universal-Manager!");
	drawCreditsDialogs();
	drawButtons();
}

void CREDITS::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_TOUCH) {
		if (touching(touch, creditsButtonPos[0])) {
			dialog = 0;
		} else if (touching(touch, creditsButtonPos[1])) {
			dialog = 1;
		} else if (touching(touch, creditsButtonPos[2])) {
			dialog = 2;
		} else if (touching(touch, creditsButtonPos[3])) {
			dialog = 3;
		} else if (touching(touch, creditsButtonPos[4])) {
			dialog = 4;
		} else if (touching(touch, creditsButtonPos[5])) {
			if (Config::Credits == 0) {
			} else if (Config::Credits == 1) {
				Config::Credits = 0;
				Config::setCredits();
			}
				Gui::screenBack();
				return;
		}
	}
}
			