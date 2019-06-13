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

#include "gui.hpp"
#include "screenCommon.hpp"
#include "settings.hpp"
#include <assert.h>
#include <stdarg.h>


C3D_RenderTarget* g_renderTargetTop;
C3D_RenderTarget* g_renderTargetBottom;

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet animation;
static C2D_TextBuf dynamicBuf;
static C2D_TextBuf staticBuf;
static C2D_TextBuf sizeBuf;
static std::unordered_map<std::string, C2D_Text> staticMap;

constexpr u32 magicNumber = 0xC7D84AB9;


static Tex3DS_SubTexture _select_box(const C2D_Image& image, int x, int y, int endX, int endY)
{
    Tex3DS_SubTexture tex = *image.subtex;
    if (x != endX)
    {
        int deltaX  = endX - x;
        float texRL = tex.left - tex.right;
        tex.left    = tex.left - (float)texRL / tex.width * x;
        tex.right   = tex.left - (float)texRL / tex.width * deltaX;
        tex.width   = deltaX;
    }
    if (y != endY)
    {
        float texTB = tex.top - tex.bottom;
        int deltaY  = endY - y;
        tex.top     = tex.top - (float)texTB / tex.height * y;
        tex.bottom  = tex.top - (float)texTB / tex.height * deltaY;
        tex.height  = deltaY;
    }
    return tex;
}

void Gui::clearTextBufs(void)
{
    C2D_TextBufClear(dynamicBuf);
}

static void _draw_mirror_scale(int key, int x, int y, int off, int rep)
{
    C2D_Image sprite = C2D_SpriteSheetGetImage(sprites, key);
    // Left side
    Tex3DS_SubTexture tex = _select_box(sprite, 0, 0, off, 0);
    C2D_DrawImageAt({sprite.tex, &tex}, x, y, 0.5f);
    // Right side
    C2D_DrawImageAt({sprite.tex, &tex}, x + off + rep, y, 0.5f, nullptr, -1.0f, 1.0f);
    // Center
    tex = _select_box(sprite, off, 0, sprite.subtex->width, 0);
    C2D_DrawImageAt({sprite.tex, &tex}, x + off, y, 0.5f, nullptr, rep, 1.0f);
}

static void _draw_repeat(int key, int x, int y, u8 rows, u8 cols)
{
    C2D_Image sprite = C2D_SpriteSheetGetImage(sprites, key);
    for (u8 row = 0; row < rows; row++)
    {
        for (u8 col = 0; col < cols; col++)
        {
            C2D_DrawImageAt(sprite, x + col * sprite.subtex->width, y + row * sprite.subtex->height, 0.5f);
        }
    }
}

bool Gui::Draw_ImageScale(C2D_Image image, float x, float y, float scaleX, float scaleY) {
	return C2D_DrawImageAt(image, x, y, 0.5f, NULL, scaleX, scaleY);
}

Result Gui::init(void)
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    g_renderTargetTop    = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    g_renderTargetBottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    dynamicBuf = C2D_TextBufNew(2048);
    staticBuf  = C2D_TextBufNew(4096);
    sizeBuf    = C2D_TextBufNew(4096);
    sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    animation = C2D_SpriteSheetLoad("romfs:/gfx/animation.t3x");
    return 0;
}

void Gui::exit(void)
{
    if (sprites)
    {
        C2D_SpriteSheetFree(sprites);
    }
    if (animation)
    {
        C2D_SpriteSheetFree(animation);
    }
    if (dynamicBuf)
    {
        C2D_TextBufDelete(dynamicBuf);
    }
    if (staticBuf)
    {
        C2D_TextBufDelete(staticBuf);
    }
    if (sizeBuf)
    {
        C2D_TextBufDelete(sizeBuf);
    }
    C2D_Fini();
    C3D_Fini();
}

void Gui::sprite(int key, int x, int y)
{
    if (key == sprites_res_null_idx)
    {
        return;
    }
    // standard case
    else
    {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f);
    }
}

void Gui::AnimationSprite(int key, int x, int y)
{
    if (key == sprites_res_null_idx)
    {
        return;
    }
    // standard case
    else
    {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(animation, key), x, y, 0.5f);
    }
}

// Text Stuff.
void Gui::dynamicText(const std::string& str, int x, int y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY)
{
    const float lineMod = ceilf(scaleY * fontGetInfo()->lineFeed);

    static std::vector<std::string> print;
    static std::vector<int> printX;

    size_t index = 0;
    while (index != std::string::npos)
    {
        print.push_back(str.substr(index, str.find('\n', index) - index));
        index = str.find('\n', index);
        if (index != std::string::npos)
        {
            index++;
        }
    }

    switch (positionX)
    {
        case TextPosX::LEFT:
            for (size_t i = 0; i < print.size(); i++)
            {
                printX.push_back(x);
            }
            break;
        case TextPosX::CENTER:
            for (size_t i = 0; i < print.size(); i++)
            {
                printX.push_back(x - (ceilf(StringUtils::textWidth(print[i], scaleX)) / 2));
            }
            break;
        case TextPosX::RIGHT:
            for (size_t i = 0; i < print.size(); i++)
            {
                printX.push_back(x - (ceilf(StringUtils::textWidth(print[i], scaleX))));
            }
            break;
    }

    switch (positionY)
    {
        case TextPosY::TOP:
            break;
        case TextPosY::CENTER:
            y -= ceilf(0.5f * lineMod * (float)print.size());
            break;
        case TextPosY::BOTTOM:
            y -= lineMod * (float)print.size();
            break;
    }

    for (size_t i = 0; i < print.size(); i++)
    {
        C2D_Text text;
        C2D_TextParse(&text, dynamicBuf, print[i].c_str());
        C2D_TextOptimize(&text);
        C2D_DrawText(&text, C2D_WithColor, printX[i], y + lineMod * i, 0.5f, scaleX, scaleY, color);
    }

    print.clear();
    printX.clear();
}

C2D_Text Gui::cacheStaticText(const std::string& strKey)
{
    C2D_Text text;
    std::unordered_map<std::string, C2D_Text>::const_iterator index = staticMap.find(strKey);
    if (index == staticMap.end())
    {
        C2D_TextParse(&text, staticBuf, strKey.c_str());
        C2D_TextOptimize(&text);
        staticMap.emplace(strKey, text);
    }
    else
    {
        return index->second;
    }

    return text;
}

void Gui::clearStaticText()
{
    C2D_TextBufClear(staticBuf);
    staticMap.clear();
}

void Gui::staticText(const std::string& strKey, int x, int y, float scaleX, float scaleY, u32 color, TextPosX positionX, TextPosY positionY)
{
    const float lineMod = ceilf(scaleY * fontGetInfo()->lineFeed);

    static std::vector<std::string> print;
    static std::vector<int> printX;

    size_t index = 0;
    while (index != std::string::npos)
    {
        print.push_back(strKey.substr(index, strKey.find('\n', index) - index));
        index = strKey.find('\n', index);
        if (index != std::string::npos)
        {
            index++;
        }
    }

    switch (positionX)
    {
        case TextPosX::LEFT:
            for (size_t i = 0; i < print.size(); i++)
            {
                printX.push_back(x);
            }
            break;
        case TextPosX::CENTER:
            for (size_t i = 0; i < print.size(); i++)
            {
                printX.push_back(x - (ceilf(StringUtils::textWidth(print[i], scaleX)) / 2));
            }
            break;
        case TextPosX::RIGHT:
            for (size_t i = 0; i < print.size(); i++)
            {
                printX.push_back(x - (ceilf(StringUtils::textWidth(print[i], scaleX))));
            }
            break;
    }

    switch (positionY)
    {
        case TextPosY::TOP:
            break;
        case TextPosY::CENTER:
            y -= ceilf(0.5f * lineMod * (float)print.size());
            break;
        case TextPosY::BOTTOM:
            y -= lineMod * (float)print.size();
            break;
    }

    for (size_t i = 0; i < print.size(); i++)
    {
        C2D_Text text = cacheStaticText(print[i].c_str());
        C2D_DrawText(&text, C2D_WithColor, printX[i], y + lineMod * i, 0.5f, scaleX, scaleY, color);
    }

    print.clear();
    printX.clear();
}

// Basic GUI Stuff.

void Gui::DrawBGTop(void) 
{
	C2D_SceneBegin(g_renderTargetTop);
	C2D_DrawRectSolid(0, 25, 400, 0.5f, 190, BLUE);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
}

void Gui::DrawBarsTop(void) 
{
    // Blending stuff.
    C2D_ImageTint bars;
    C2D_SetImageTint(&bars, C2D_TopLeft, RED, 1);
    C2D_SetImageTint(&bars, C2D_TopRight, GREEN, 1);
    C2D_SetImageTint(&bars, C2D_BotLeft, BLUE, 1);
    C2D_SetImageTint(&bars, C2D_BotRight, WHITE, 1);

    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_top_screen_top_idx), 0, 0, 0.5f, &bars);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_top_screen_bot_idx), 0, 215, 0.5f, &bars);
}

void Gui::DrawBGBot(void)
{
	C2D_SceneBegin(g_renderTargetBottom);
	C2D_DrawRectSolid(0, 25, 0.5f, 320, 190, BLUE);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
}

void Gui::DrawBarsBot(void)
{
        // Blending stuff.
    C2D_ImageTint bars;
    C2D_SetImageTint(&bars, C2D_TopLeft, RED, 1);
    C2D_SetImageTint(&bars, C2D_TopRight, GREEN, 1);
    C2D_SetImageTint(&bars, C2D_BotLeft, BLUE, 1);
    C2D_SetImageTint(&bars, C2D_BotRight, WHITE, 1);

    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_bottom_screen_top_idx), 0, 0, 0.5f, &bars);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, sprites_bottom_screen_bot_idx), 0, 215, 0.5f, &bars);
}

// Text.

void DisplayMsg(const char* text) {
    Gui::clearStaticText();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(g_renderTargetTop, BLUE2);
    C2D_TargetClear(g_renderTargetBottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	Gui::staticText(text, 200, 32, 0.45f, 0.45f, BLACK, TextPosX::CENTER, TextPosY::TOP);
	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	C3D_FrameEnd(0);
}
