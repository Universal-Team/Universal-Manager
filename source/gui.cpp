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


C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet animation;
C2D_TextBuf staticBuf, dynamicBuf;
static C2D_TextBuf widthBuf;
static std::unordered_map<std::string, C2D_Text> staticMap;


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

void Gui::Draw_ImageBlend(int key, int x, int y, u32 color)
{
    C2D_ImageTint tint;
    C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
    C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
    C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
    C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f, &tint);
}

void Gui::Draw_ImageBlend2(int key, int x, int y, u32 color)
{
    C2D_ImageTint tint;
    C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
    C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
    C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
    C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(animation, key), x, y, 0.5f, &tint);
}

Result Gui::init(void)
{
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    staticBuf = C2D_TextBufNew(4096);
    dynamicBuf = C2D_TextBufNew(4096);
    widthBuf = C2D_TextBufNew(4096);
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
    C2D_TextBufDelete(widthBuf);
    C2D_TextBufDelete(dynamicBuf);
    C2D_TextBufDelete(staticBuf);
    C2D_Fini();
    C3D_Fini();
}

void set_screen(C3D_RenderTarget * screen)
{
    C2D_SceneBegin(screen);
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

// Basic GUI Stuff.

void Gui::DrawBarsBottomBack(void) {
    C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, settings.universal.bars);
    C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, settings.universal.bars);
    Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
    Gui::sprite(sprites_bottom_screen_bot_back_idx, 0, 210);
}

void Gui::DrawBGTop(void) 
{
    set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, settings.universal.bg);
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
}

void Gui::DrawBarsTop(void) 
{
    C2D_DrawRectSolid(0, 215, 0.5f, 400, 25, settings.universal.bars);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, settings.universal.bars);
    Gui::sprite(sprites_top_screen_top_idx, 0, 0);
    Gui::sprite(sprites_top_screen_bot_idx, 0, 215);
}

void Gui::DrawBGBot(void)
{
	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, settings.universal.bg);
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
}

void Gui::DrawBarsBot(void)
{
    C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, settings.universal.bars);
    C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, settings.universal.bars);
    Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
    Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
}

void Gui::DrawOverlayTop(void)
{
    Gui::Draw_ImageBlend(sprites_overlay_top_idx, 0, 0, settings.universal.bars);
	Gui::sprite(sprites_overlay_top_2_idx, 0, 0);
}

void Gui::DrawOverlayBot(void)
{
	Gui::Draw_ImageBlend(sprites_overlay_bot_idx, 0, 0, settings.universal.bars);
	Gui::sprite(sprites_overlay_bot_2_idx, 0, 0);
}

void Gui::DrawOverlayBotBack(void)
{
	Gui::Draw_ImageBlend(sprites_overlay_bot_idx, 0, 0, settings.universal.bars);
	Gui::sprite(sprites_overlay_bot_2_idx, 0, 0);
    Gui::sprite(sprites_back_idx, 293, 213);
}

void Gui::chooseLayoutTop(void) {
    if (settings.universal.layout == 0) {
        Gui::DrawBarsTop();
    } else if (settings.universal.layout == 1) {
        Gui::DrawOverlayTop();
    }
}

void Gui::chooseLayoutBot(void) {
    if (settings.universal.layout == 0) {
        Gui::DrawBarsBot();
    } else if (settings.universal.layout == 1) {
        Gui::DrawOverlayBot();
    }
}

void Gui::chooseLayoutBotBack(void) {
    if (settings.universal.layout == 0) {
        Gui::DrawBarsBottomBack();
    } else if (settings.universal.layout == 1) {
        Gui::DrawOverlayBotBack();
    }
}

// Text.

void DisplayMsg(const char* text) {
    //Gui::clearStaticText();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
	draw_text(26, 32, 0.45f, 0.45f, BLACK, text);
	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	C3D_FrameEnd(0);
}

void DisplayTime(void) {
    C2D_Text timeText;
    C2D_TextParse(&timeText, dynamicBuf, DateTime::timeStr().c_str());
    C2D_TextOptimize(&timeText);
    C2D_DrawText(&timeText, C2D_WithColor, 1.0f, 1.0f, 0.5f, 0.7f, 0.7f, WHITE);
}

// Battery stuff.

void drawBatteryTop(void) {
	  u8 batteryChargeState = 0;
      char percent[5];

	u8 batteryPercent;
	mcuGetBatteryLevel(&batteryPercent);
	if(batteryPercent == 0) {
	Gui::sprite(sprites_battery0_idx, 361, 0);
	} else if (batteryPercent > 0 && batteryPercent <= 25) {
	Gui::sprite(sprites_battery25_idx, 361, 0);
	} else if(batteryPercent > 25 && batteryPercent <= 50) {
	Gui::sprite(sprites_battery50_idx, 361, 0);
	} else if(batteryPercent > 50 && batteryPercent <= 75) {
    Gui::sprite(sprites_battery75_idx, 361, 0);
	} else if(batteryPercent == 100) {
	Gui::sprite(sprites_battery100_idx, 361, 0);
	}

    if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryChargeState)) && batteryChargeState) {
		Gui::sprite(sprites_batteryCharge_idx, 361, 0);
    }

        if (settings.universal.battery == 0) {
        } else if (settings.universal.battery == 1) {
	if(batteryPercent == 100) {
		draw_text(310, 4, 0.7f, 0.7f, WHITE, "100%%");
	} else {
		snprintf(percent, 5, "%d%%", batteryPercent);
        C2D_Text percentText;
        C2D_TextParse(&percentText, dynamicBuf, percent);
		C2D_TextOptimize(&percentText);
        C2D_DrawText(&percentText, C2D_WithColor, 310.0f, 3.0f, 0.5f, 0.7f, 0.7f, WHITE);
}
}
}

void drawBatteryBot(void) {
	 u8 batteryChargeState = 0;
     char percent[5];

	u8 batteryPercent;
	mcuGetBatteryLevel(&batteryPercent);
	if(batteryPercent == 0) {
	Gui::sprite(sprites_battery0_idx, 281, 0);
	} else if (batteryPercent > 0 && batteryPercent <= 25) {
	Gui::sprite(sprites_battery25_idx, 281, 0);
	} else if(batteryPercent > 25 && batteryPercent <= 50) {
	Gui::sprite(sprites_battery50_idx, 281, 0);
	} else if(batteryPercent > 50 && batteryPercent <= 75) {
    Gui::sprite(sprites_battery75_idx, 281, 0);
	} else if(batteryPercent == 100) {
	Gui::sprite(sprites_battery100_idx, 281, 0);
	}

    if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryChargeState)) && batteryChargeState) {
		Gui::sprite(sprites_batteryCharge_idx, 281, 0);
	}

            if (settings.universal.battery == 0) {
        } else if (settings.universal.battery == 1) {
    	if(batteryPercent == 100) {
		draw_text(230, 4, 0.7f, 0.7f, WHITE, "100%%");
	} else {
		snprintf(percent, 5, "%d%%", batteryPercent);
        C2D_Text percentText;
        C2D_TextParse(&percentText, dynamicBuf, percent);
		C2D_TextOptimize(&percentText);
        C2D_DrawText(&percentText, C2D_WithColor, 230.0f, 3.0f, 0.5f, 0.7f, 0.7f, WHITE);
}
}
}

static void get_text_dimensions(const char * text, float scaleX, float scaleY, float * width, float * height)
{
    C2D_Text c2d_text;
    C2D_TextParse(&c2d_text, widthBuf, text);
    C2D_TextGetDimensions(&c2d_text, scaleX, scaleY, width, height);
}

static void draw_c2d_text(float x, float y, float z, float scaleX, float scaleY, u32 color, C2D_Text * text)
{
    C2D_DrawText(text, C2D_WithColor, x, y, z, scaleX, scaleY, color);
}

void draw_text(float x, float y, float scaleX, float scaleY, u32 color, const char * text)
{
    C2D_Text c2d_text;
    C2D_TextParse(&c2d_text, dynamicBuf, text);
    C2D_TextOptimize(&c2d_text);
    C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5, scaleX, scaleY, color);
}



static void draw_c2d_text_center(gfxScreen_t target, float y, float z, float scaleX, float scaleY, u32 color, C2D_Text * text)
{
    float width = 0;
    C2D_TextGetDimensions(text, scaleX, scaleY, &width, NULL);
    float offset = (target == GFX_TOP ? 400 : 320)/2 - width/2;

    C2D_DrawText(text, C2D_WithColor, offset, y, z, scaleX, scaleY, color);
}

void draw_text_center(gfxScreen_t target, float y, float z, float scaleX, float scaleY, u32 color, const char * text)
{
    C2D_Text text_arr[MAX_LINES];
    float offsets_arr[MAX_LINES];
    int actual_lines = 0;
    const char * end = text - 1;

    do {
        end = C2D_TextParseLine(&text_arr[actual_lines], dynamicBuf, end + 1, actual_lines);
        actual_lines++;
    } while(*end == '\n');

    for(int i = 0; i < actual_lines; i++)
    {
        C2D_TextOptimize(&text_arr[i]);
        float width = 0;
        C2D_TextGetDimensions(&text_arr[i], scaleX, scaleY, &width, NULL);
        offsets_arr[i] = (target == GFX_TOP ? 400 : 320)/2 - width/2;
    }

    for(int i = 0; i < actual_lines; i++)
    {
        C2D_DrawText(&text_arr[i], C2D_WithColor, offsets_arr[i], y, z, scaleX, scaleY, color);
    }
}

void end_frame(void)
{
    C2D_TextBufClear(dynamicBuf);
    C2D_TextBufClear(widthBuf);
    C3D_FrameEnd(0);
}

void start_frame(void)
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
}