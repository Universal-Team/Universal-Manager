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

#ifndef GUI_HPP
#define GUI_HPP

#include "colors.hpp"
#include "common.hpp"
#include "ptmu_x.h"
#include "spriteID.h"

#include "screens/screen.hpp"

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <random>
#include <stack>
#include <string.h>
#include <unordered_map>
#include <wchar.h>

#define FONT_SIZE_18 0.72f
#define FONT_SIZE_17 0.7f
#define FONT_SIZE_15 0.6f
#define FONT_SIZE_14 0.56f
#define FONT_SIZE_12 0.50f
#define FONT_SIZE_11 0.46f
#define FONT_SIZE_9 0.37f

namespace DateTime {
	std::string timeStr(void);
}

namespace Gui
{
	Result init(void);
	Result loadSheetsAndFont(void);
	void exit(void);

	void mainLoop(u32 hDown, u32 hHeld, touchPosition touch);
	void setScreen(std::unique_ptr<SCREEN> screen);
	void screenBack(void);

	C3D_RenderTarget* target(gfxScreen_t t);

	// Clear Text.
	void clearTextBufs(void);

	// Draw Sprites from the Sheets.
	void sprite(int sheet, int key, int x, int y);
	void Draw_ImageBlend(int sheet, int key, int x, int y, u32 color);

	// Layouts!
	void DrawBGTop(void);
	void DrawBarsTop(void);
	void DrawBGBot(void);
	void DrawBarsBot(void);
	void DrawBarsBottomBack(void);

	// The animated Selectors.
	void drawFileSelector(float x, float y);
	void drawGUISelector(int key, float x, float y, float speed);

	// New Text / String Functions.
	void DrawStringCentered(float x, float y, float size, u32 color, std::string Text, int maxWidth = 400);
	void DrawString(float x, float y, float size, u32 color, std::string Text, int maxWidth = 400);
	void GetStringSize(float size, float *width, float *height, std::string Text);
	float GetStringWidth(float size, std::string Text);
	float GetStringHeight(float size, std::string Text);

	void DrawFileBrowseBG(u32 color1 = C2D_Color32(127, 127, 127, 255), u32 color2 = C2D_Color32(195, 195, 195, 255), bool topDraw = true);
}

	// Other Display stuff.
	void DisplayMsg(std::string text);
	void DisplayTime(void);

	// Battery Draw (Top/Bottom Screen.)
	void drawBatteryTop(void);
	void drawBatteryBot(void);

	void displayBatteryNearlyToDead(void);

	void set_screen(C3D_RenderTarget * screen);

	// Misc.
	bool Draw_Rect(float x, float y, float w, float h, u32 color);

	// Editor Draw.
	void Draw_Text_Editor(float x, float y, float size, u32 color, const char *text);
	void Draw_GetTextSizeEditor(float size, float *width, float *height, const char *text);
	float Draw_GetTextWidthEditor(float size, const char *text);

#endif