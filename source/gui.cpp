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
#include <unistd.h>


C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet animation;
C2D_TextBuf dynamicBuf, sizeBuf;
static C2D_TextBuf widthBuf;
C2D_Font defaultFont;
C2D_Font customFont;

void Gui::clearTextBufs(void)
{
    C2D_TextBufClear(dynamicBuf);
    C2D_TextBufClear(sizeBuf);
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
    dynamicBuf = C2D_TextBufNew(4096);
    widthBuf = C2D_TextBufNew(4096);
    sizeBuf = C2D_TextBufNew(4096);
    sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    animation = C2D_SpriteSheetLoad("romfs:/gfx/animation.t3x");
    defaultFont = C2D_FontLoad("romfs:/gfx/Font.bcfnt");
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
    C2D_TextBufDelete(sizeBuf);
    C2D_FontFree(defaultFont);
    C2D_FontFree(customFont);
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
    if (settings.universal.bgl == 0) {
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    } else if (settings.universal.bgl == 1) {
            for (int x = 0; x < 400; x += 14)
        {
            for (int y = 0; y < 240; y += 14)
            {
                Gui::sprite(sprites_stripes_idx, x, y);
            }
}
} else if (settings.universal.bgl == 2) {
}
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
    if (settings.universal.bgl == 0) {
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
    } else if (settings.universal.bgl == 1) {
       {
        for (int x = 0; x < 320; x += 14)
        {
            for (int y = 0; y < 240; y += 14)
            {
                Gui::sprite(sprites_stripes2_idx, x, y);
            }
        }
    }
} else if (settings.universal.bgl == 2) {
}
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
    Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::chooseLayoutTop();
    Gui::sprite(sprites_textbox_idx, 10, 25);
	Draw_Text(35, 42, 0.45f, BLACK, text);
	Gui::DrawBGBot();
	Gui::chooseLayoutBot();
	C3D_FrameEnd(0);
}

void DisplayTime(void) {
    C2D_Text timeText;
    if (settings.universal.font == 0) {
    C2D_TextFontParse(&timeText, defaultFont, sizeBuf, DateTime::timeStr().c_str());
    } else if (settings.universal.font == 1) {
    C2D_TextFontParse(&timeText, customFont, sizeBuf, DateTime::timeStr().c_str());
    }
    C2D_TextOptimize(&timeText);
    C2D_DrawText(&timeText, C2D_WithColor, 1.0f, 1.0f, 0.5f, 0.65f, 0.65f, WHITE);
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
	} else if(batteryPercent > 50 && batteryPercent <= 99) {
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
		Draw_Text(310, 0, 0.65f, WHITE, "100%");
	} else {
		snprintf(percent, 5, "%d%%", batteryPercent);
        C2D_Text percentText;
        if (settings.universal.font == 0) {
        C2D_TextFontParse(&percentText, defaultFont, sizeBuf, percent);
        } else if (settings.universal.font == 1) {
        C2D_TextFontParse(&percentText, customFont, sizeBuf, percent);
        }
		C2D_TextOptimize(&percentText);
        C2D_DrawText(&percentText, C2D_WithColor, 310.0f, 0.0f, 0.5f, 0.65f, 0.65f, WHITE);
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
	} else if(batteryPercent > 50 && batteryPercent <= 99) {
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
		Draw_Text(230, 0, 0.65f, WHITE, "100%");
	} else {
		snprintf(percent, 5, "%d%%", batteryPercent);
        C2D_Text percentText;
        if (settings.universal.font == 0) {
        C2D_TextFontParse(&percentText, defaultFont, sizeBuf, percent);
        } else if (settings.universal.font == 1) {
        C2D_TextFontParse(&percentText, customFont, sizeBuf, percent);
        }
		C2D_TextOptimize(&percentText);
        C2D_DrawText(&percentText, C2D_WithColor, 230.0f, 0.0f, 0.5f, 0.65f, 0.65f, WHITE);
}
}
}

void Draw_EndFrame(void) {
	C2D_TextBufClear(dynamicBuf);
	C2D_TextBufClear(sizeBuf);
	C3D_FrameEnd(0);
}

void start_frame(void)
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
}

void Draw_Text(float x, float y, float size, u32 color, const char *text) {
	C2D_Text c2d_text;
    if (settings.universal.font == 0) {
        C2D_TextFontParse(&c2d_text, defaultFont, sizeBuf, text);
    } else if (settings.universal.font == 1) {
	    C2D_TextFontParse(&c2d_text, customFont, sizeBuf, text);
    }
	C2D_TextOptimize(&c2d_text);
	C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, color);
}

void Draw_Textf(float x, float y, float size, u32 color, const char* text, ...) {
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	Draw_Text(x, y, size, color, buffer);
	va_end(args);
}

void Draw_GetTextSize(float size, float *width, float *height, const char *text) {
	C2D_Text c2d_text;
    if (settings.universal.font == 0) {
        C2D_TextFontParse(&c2d_text, defaultFont, sizeBuf, text);
    } else if (settings.universal.font == 1) {
	    C2D_TextFontParse(&c2d_text, customFont, sizeBuf, text);
    }
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}

float Draw_GetTextWidth(float size, const char *text) {
	float width = 0;
	Draw_GetTextSize(size, &width, NULL, text);
	return width;
}

float Draw_GetTextHeight(float size, const char *text) {
	float height = 0;
	Draw_GetTextSize(size, NULL, &height, text);
	return height;
}

bool Draw_Rect(float x, float y, float w, float h, u32 color) {
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, color);
}

std::string DateTime::timeStr(void)
{
    time_t unixTime       = time(NULL);
    struct tm* timeStruct = gmtime((const time_t*)&unixTime);
    return StringUtils::format("%02i:%02i:%02i", timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
}