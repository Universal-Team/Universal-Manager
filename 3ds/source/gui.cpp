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
#include "screens/screenCommon.hpp"
#include "utils/settings.hpp"

#include <3ds.h>
#include <assert.h>
#include <stack>
#include <stdarg.h>
#include <unistd.h>

C3D_RenderTarget* top;
C3D_RenderTarget* bottom;

static C2D_SpriteSheet sprites;
static C2D_SpriteSheet animation;
static C2D_SpriteSheet credits;
static C2D_SpriteSheet button;

C2D_TextBuf dynamicBuf, sizeBuf;
C2D_Font systemFont, editorFont;
std::stack<std::unique_ptr<SCREEN>> screens;

void Gui::clearTextBufs(void)
{
    C2D_TextBufClear(dynamicBuf);
    C2D_TextBufClear(sizeBuf);
}

void Gui::Draw_ImageBlend(int key, int x, int y, u32 color)
{
    C2D_ImageTint tint;
    C2D_SetImageTint(&tint, C2D_TopLeft, color, 0.5);
    C2D_SetImageTint(&tint, C2D_TopRight, color, 0.5);
    C2D_SetImageTint(&tint, C2D_BotLeft, color, 0.5);
    C2D_SetImageTint(&tint, C2D_BotRight, color, 0.5);
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
    sizeBuf = C2D_TextBufNew(4096);
    sprites    = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    animation = C2D_SpriteSheetLoad("romfs:/gfx/animation.t3x");
    credits   = C2D_SpriteSheetLoad("romfs:/gfx/credits.t3x");
    button   = C2D_SpriteSheetLoad("romfs:/gfx/button.t3x");
    
    systemFont = C2D_FontLoadSystem(CFG_REGION_USA);
    editorFont = C2D_FontLoad("romfs:/gfx/TextEditorFont.bcfnt");
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
    if (credits)
    {
        C2D_SpriteSheetFree(credits);
    }
    if (button)
    {
        C2D_SpriteSheetFree(button);
    }
    C2D_TextBufDelete(dynamicBuf);
    C2D_TextBufDelete(sizeBuf);
    C2D_FontFree(editorFont);
    C2D_Fini();
    C3D_Fini();
}

void set_screen(C3D_RenderTarget * screen)
{
    C2D_SceneBegin(screen);
}

void Gui::sprite(int key, int x, int y)
{
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(sprites, key), x, y, 0.5f);
}

void Gui::AnimationSprite(int key, int x, int y)
{
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(animation, key), x, y, 0.5f);
}

void Gui::Credits(int key, int x, int y)
{
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(credits, key), x, y, 0.5f);
}

void Gui::Button(int key, int x, int y)
{
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(button, key), x, y, 0.5f);
}

// Basic GUI Stuff.

void Gui::DrawBarsBottomBack(void) {
    C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, Config::barColor);
    C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, Config::barColor);
    if (Config::layout == 0 || Config::layout == 1) {
        Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
        Gui::sprite(sprites_bottom_screen_bot_back_idx, 0, 210);
    }
}

void Gui::DrawBGTop(void) 
{
    set_screen(top);
	C2D_DrawRectSolid(0, 0, 0.5f, 400, 240, Config::bgColor);
    if (Config::layoutBG == 0) {
	Gui::sprite(sprites_universal_bg_top_idx, 0, 25);
    } else if (Config::layoutBG == 1) {
            for (int x = 0; x < 400; x += 14)
        {
            for (int y = 0; y < 240; y += 14)
            {
                Gui::sprite(sprites_stripes_idx, x, y);
            }
}
} else if (Config::layoutBG == 2) {
}
}

void Gui::DrawBarsTop(void) 
{
    C2D_DrawRectSolid(0, 215, 0.5f, 400, 25, Config::barColor);
    C2D_DrawRectSolid(0, 0, 0.5f, 400, 25, Config::barColor);
    if (Config::layout == 0) {
        Gui::sprite(sprites_top_screen_top_idx, 0, 0);
        Gui::sprite(sprites_top_screen_bot_idx, 0, 215);
    } else if (Config::layout == 1) {
    }
}

void Gui::DrawBGBot(void)
{
	set_screen(bottom);
	C2D_DrawRectSolid(0, 0, 0.5f, 320, 240, Config::bgColor);
    if (Config::layoutBG == 0) {
	Gui::sprite(sprites_universal_bg_bottom_idx, 0, 25);
    } else if (Config::layoutBG == 1) {
       {
        for (int x = 0; x < 320; x += 14)
        {
            for (int y = 0; y < 240; y += 14)
            {
                Gui::sprite(sprites_stripes2_idx, x, y);
            }
        }
    }
} else if (Config::layoutBG == 2) {
}
}

void Gui::DrawBarsBot(void)
{
    C2D_DrawRectSolid(0, 215, 0.5f, 320, 25, Config::barColor);
    C2D_DrawRectSolid(0, 0, 0.5f, 320, 25, Config::barColor);
    if (Config::layout == 0) {
        Gui::sprite(sprites_bottom_screen_top_idx, 0, 0);
        Gui::sprite(sprites_bottom_screen_bot_idx, 0, 215);
    } else if (Config::layout == 1) {
    }
}

// Text.

void DisplayMsg(std::string text) {
    Gui::clearTextBufs();
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, BLUE2);
    C2D_TargetClear(bottom, BLUE2);
	Gui::DrawBGTop();
	Gui::DrawBarsTop();
    Gui::sprite(sprites_textbox_idx, 10, 25);
	Gui::DrawString(35, 42, 0.45f, BLACK, text);
	Gui::DrawBGBot();
	Gui::DrawBarsBot();
	C3D_FrameEnd(0);
}

void DisplayTime(void) {
    C2D_Text timeText;
    C2D_TextFontParse(&timeText, systemFont, sizeBuf, DateTime::timeStr().c_str());
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
	} else if(batteryPercent > 50 && batteryPercent <= 75) {
    Gui::sprite(sprites_battery75_idx, 361, 0);
	} else if(batteryPercent > 75 || batteryPercent == 100) {
	Gui::sprite(sprites_battery100_idx, 361, 0);
	}

    if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryChargeState)) && batteryChargeState) {
		Gui::sprite(sprites_batteryCharge_idx, 361, 0);
    }

        if (Config::percentDisplay == 0) {
        } else if (Config::percentDisplay == 1) {
	    if(batteryPercent == 100) {
		Gui::DrawString(315, 2, 0.65f, WHITE, "100%");
	    } else {
		snprintf(percent, 5, "%d%%", batteryPercent);
        C2D_Text percentText;
        C2D_TextFontParse(&percentText, systemFont, sizeBuf, percent);
		C2D_TextOptimize(&percentText);
        C2D_DrawText(&percentText, C2D_WithColor, 315.0f, 2.0f, 0.5f, 0.65f, 0.65f, WHITE);
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
	} else if(batteryPercent > 75 || batteryPercent == 100) {
	Gui::sprite(sprites_battery100_idx, 281, 0);
	}

    if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryChargeState)) && batteryChargeState) {
		Gui::sprite(sprites_batteryCharge_idx, 281, 0);
	}

        if (Config::percentDisplay == 0) {
        } else if (Config::percentDisplay == 1) {
    	if(batteryPercent == 100) {
		Gui::DrawString(230, 0, 0.65f, WHITE, "100%");
	    } else {
		snprintf(percent, 5, "%d%%", batteryPercent);
        C2D_Text percentText;
        C2D_TextFontParse(&percentText, systemFont, sizeBuf, percent);
		C2D_TextOptimize(&percentText);
        C2D_DrawText(&percentText, C2D_WithColor, 230.0f, 0.0f, 0.5f, 0.65f, 0.65f, WHITE);
}
}
}

// This is mainly, to replace `\\n` from the Ini file with `\n`.
void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
 
	// Repeat till end is reached
	while( pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos =data.find(toSearch, pos + replaceStr.size());
	}
}


// Draw String or Text.
void Gui::DrawString(float x, float y, float size, u32 color, std::string Text)
{
    findAndReplaceAll(Text, "\\n", "\n");
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
	C2D_TextOptimize(&c2d_text);
	C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, color);
}


// Get String or Text Width.
float Gui::GetStringWidth(float size, std::string Text) {
    findAndReplaceAll(Text, "\\n", "\n");
	float width = 0;
	GetStringSize(size, &width, NULL, Text);
	return width;
}

// Get String or Text Size.
void Gui::GetStringSize(float size, float *width, float *height, std::string Text) {
    findAndReplaceAll(Text, "\\n", "\n");
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, systemFont, sizeBuf, Text.c_str());
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}


// Get String or Text Height.
float Gui::GetStringHeight(float size, std::string Text) {
    findAndReplaceAll(Text, "\\n", "\n");
	float height = 0;
	GetStringSize(size, NULL, &height, Text.c_str());
	return height;
}

void Draw_Text_Editor(float x, float y, float size, u32 color, const char *text) {
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, editorFont, sizeBuf, text);
	C2D_TextOptimize(&c2d_text);
	C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, color);
}

void Draw_GetTextSizeEditor(float size, float *width, float *height, const char *text) {
	C2D_Text c2d_text;
    C2D_TextFontParse(&c2d_text, editorFont, sizeBuf, text);
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}

float Draw_GetTextWidthEditor(float size, const char *text) {
	float width = 0;
	Draw_GetTextSizeEditor(size, &width, NULL, text);
	return width;
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

void Gui::mainLoop(u32 hDown, u32 hHeld, touchPosition touch) {
	screens.top()->Draw();
	screens.top()->Logic(hDown, hHeld, touch);
}

void Gui::setScreen(std::unique_ptr<SCREEN> screen)
{
    screens.push(std::move(screen));
}

void Gui::screenBack()
{
    screens.pop();
}

void Gui::drawFileSelector(float x, float y)
{
    static constexpr int w     = 2;
    static float timer         = 0.0f;
    float highlight_multiplier = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
    u8 r                       = Config::barColor & 0xFF;
    u8 g                       = (Config::barColor >> 8) & 0xFF;
    u8 b                       = (Config::barColor >> 16) & 0xFF;
    u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);

    if (Config::selector == 0) {
        Draw_Rect(x, y, 400, 25, C2D_Color32(0, 0, 0, 255));
    } else if (Config::selector == 1) {
        Draw_Rect(x, y, 400, 25, C2D_Color32(255, 255, 255, 20));
    } else if (Config::selector == 2) {
        Draw_Rect(x, y, 400, 25, C2D_Color32(255, 255, 255, 255));
    }
    
    Draw_Rect(x, y, 400, w, color);                      // top
    Draw_Rect(x, y + w, w, 25 - 2 * w, color);          // left
    Draw_Rect(x + 400 - w, y + w, w, 25 - 2 * w, color); // right
    Draw_Rect(x, y + 25 - w, 400, w, color);             // bottom

    timer += .005f;
}

void Gui::drawGUISelector(int key, float x, float y, float speed)
{
    static float timer         = 0.0f;
    float highlight_multiplier = fmax(0.0, fabs(fmod(timer, 1.0) - 0.5) / 0.5);
    u8 r                       = Config::barColor & 0xFF;
    u8 g                       = (Config::barColor >> 8) & 0xFF;
    u8 b                       = (Config::barColor >> 16) & 0xFF;
    u32 color = C2D_Color32(r + (255 - r) * highlight_multiplier, g + (255 - g) * highlight_multiplier, b + (255 - b) * highlight_multiplier, 255);
    C2D_ImageTint tint;
    C2D_SetImageTint(&tint, C2D_TopLeft, color, 1);
    C2D_SetImageTint(&tint, C2D_TopRight, color, 1);
    C2D_SetImageTint(&tint, C2D_BotLeft, color, 1);
    C2D_SetImageTint(&tint, C2D_BotRight, color, 1);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(button, key), x, y, 0.5f, &tint);

    timer += speed;
}