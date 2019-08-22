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

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <random>
#include <stack>
#include <string.h>
#include <unordered_map>
#include <wchar.h>
#include "common.hpp"
#include "screen.hpp"

// Spritesheets.
#include "sprites.h"
#include "animation.h"
#include "credits.h"
#include "button.h"

#include "colors.hpp"

// Battery Stuff.
#include "ptmu_x.h"

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
    void exit(void);

	void mainLoop(u32 hDown, u32 hHeld, touchPosition touch);
	void setScreen(std::unique_ptr<SCREEN> screen);
	void screenBack(void);

    C3D_RenderTarget* target(gfxScreen_t t);

    void clearTextBufs(void);
    
    void sprite(int key, int x, int y);
    void AnimationSprite(int key, int x, int y);
    void Credits(int key, int x, int y);
    void Button(int key, int x, int y);
    bool Draw_ImageScale(C2D_Image image, float x, float y, float scaleX, float scaleY);

    // Layouts!
    void DrawBGTop(void);
    void DrawBarsTop(void);
    void DrawBGBot(void);
    void DrawBarsBot(void);
    void DrawBarsBottomBack(void);

    void Draw_ImageBlend(int key, int x, int y, u32 color);
    void Draw_ImageBlend2(int key, int x, int y, u32 color);

    void drawFileSelector(float x, float y);
}

   // Text.
    void DisplayMsg(const char* text);
    void DisplayTime(void);

    void drawBatteryTop(void);
    void drawBatteryBot(void);

    void set_screen(C3D_RenderTarget * screen);

    // 3DShell -> Most likely for FTP.
    void Draw_Text(float x, float y, float size, u32 color, const char *text);
    void Draw_Textf(float x, float y, float size, u32 color, const char* text, ...);
    void Draw_GetTextSize(float size, float *width, float *height, const char *text);
    float Draw_GetTextWidth(float size, const char *text);
    float Draw_GetTextHeight(float size, const char *text);
    bool Draw_Rect(float x, float y, float w, float h, u32 color);

    void Draw_Text_Editor(float x, float y, float size, u32 color, const char *text);
    void Draw_GetTextSizeEditor(float size, float *width, float *height, const char *text);
    float Draw_GetTextWidthEditor(float size, const char *text);

#endif
