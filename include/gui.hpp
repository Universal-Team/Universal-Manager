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

#include "sprites.h"
#include "colors.hpp"
#include "TextPos.hpp"
#include "3dsutils.hpp"

// emulated
#define sprites_res_null_idx 500

#define FONT_SIZE_18 0.72f
#define FONT_SIZE_17 0.7f
#define FONT_SIZE_15 0.6f
#define FONT_SIZE_14 0.56f
#define FONT_SIZE_12 0.50f
#define FONT_SIZE_11 0.46f
#define FONT_SIZE_9 0.37f

namespace Gui
{
    Result init(void);
    void exit(void);

    C3D_RenderTarget* target(gfxScreen_t t);

    void clearTextBufs(void);

    void dynamicText(const std::string& str, int x, int y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY);
    C2D_Text cacheStaticText(const std::string& strKey);
    void clearStaticText(void);
    void staticText(const std::string& strKey, int x, int y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY);
    
    void sprite(int key, int x, int y);
    bool Draw_ImageScale(C2D_Image image, float x, float y, float scaleX, float scaleY);

    // Basic GUI.
    void DrawBGTop();
    void DrawBarsTop();
    void DrawBGBot();
    void DrawBarsBot();
}

#endif
