/*
*   This file is part of Universal-Updater
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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

#include <3ds.h>
#include <algorithm>
#include <dirent.h>
#include <malloc.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "datetime.h"
#include "download.hpp"
#include "dumpdsp.h"
#include "graphic.h"
#include "inifile.h"
#include "keyboard.h"
#include "music.hpp"
#include "settings.h"
#include "Textures.hpp"
#include "thread.hpp"
#include "Universal-Settings.hpp"
#include "voltlib/volt.h"

// UI STUFF

void loadBasicGraphic(void) {
	if((access("sdmc:/Universal-Updater/Graphic/Image.png", F_OK) == 0)) {
	volt_load_texture_png(SD, "sdmc:/Universal-Updater/Graphic/Image.png");
    volt_load_texture_png(button, "romfs:/graphics/Misc/Button.png");
	volt_load_texture_png(dot, "romfs:/graphics/Misc/Dot.png");
	volt_load_texture_png(Border, "romfs:/graphics/Misc/border_top.png");
	volt_load_texture_png(buttonRGB, "romfs:/graphics/Misc/buttonRGB.png");
	volt_load_texture_png(pageframe, "romfs:/graphics/Misc/Page_Number_Frame.png");
	volt_load_texture_png(settingsIcon, "romfs:/graphics/Misc/Settings_Icon.png");
	volt_load_texture_png(barTop, "romfs:/graphics/Misc/Bars_Top.png");
} else {
	volt_load_texture_png(button, "romfs:/graphics/Misc/Button.png");
	volt_load_texture_png(dot, "romfs:/graphics/Misc/Dot.png");
	volt_load_texture_png(Border, "romfs:/graphics/Misc/border_top.png");
	volt_load_texture_png(buttonRGB, "romfs:/graphics/Misc/buttonRGB.png");
	volt_load_texture_png(pageframe, "romfs:/graphics/Misc/Page_Number_Frame.png");
	volt_load_texture_png(settingsIcon, "romfs:/graphics/Misc/Settings_Icon.png");
	volt_load_texture_png(barTop, "romfs:/graphics/Misc/Bars_Top.png");
}
}

void draw_Bars_Bottom(void) {
	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
	volt_draw_rectangle(0, 0, 320, 19, settings.universal.bars);
	volt_draw_rectangle(0, 221, 320, 19, settings.universal.bars);
}

void draw_Background_Bottom(void) {
	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
    volt_draw_rectangle(0, 0, 320, 240, settings.universal.bg);
}

void draw_Background_Top(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
    volt_draw_rectangle(0, 0, 400, 240, settings.universal.bg);
}

void draw_Dialogbox_Color(void) {
	if (settings.universal.layout == 0) {
	volt_draw_rectangle(0, 21, 400, 199, settings.universal.bg);
} else if (settings.universal.layout == 1) {
	volt_draw_rectangle(24, 23, 352, 207, settings.universal.bg);
} else if (settings.universal.layout == 2) {
	volt_draw_rectangle(0, 21, 320, 199, settings.universal.bg);
}
}

void displayMsg(const char* text) {
	if (settings.universal.layout == 0) {
		volt_begin_draw(GFX_TOP, GFX_LEFT);
		draw_Dialogbox_Color();
		volt_draw_text(26, 32, 0.45f, 0.45f, settings.universal.text, text);
	} else if (settings.universal.layout == 1) {
		volt_begin_draw(GFX_TOP, GFX_LEFT);
		draw_Dialogbox_Color();
		volt_draw_text(30, 30, 0.40f, 0.40f, settings.universal.text, text);
	} else if (settings.universal.layout == 2) {
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		draw_Dialogbox_Color();
		volt_draw_text(30, 30, 0.40f, 0.40f, settings.universal.text, text);
	}
	volt_end_draw();
}

// Layouts!

void draw_Border(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	draw_Background_Top();
	//volt_draw_texture(Border, 0, 0);
	volt_draw_texture_blend(Border, 0, 0, settings.universal.bars);
	volt_draw_text(4, 3, 0.5f, 0.5f, settings.universal.text, getTime().c_str());
    volt_draw_text(350, 3, 0.5f, 0.5f, settings.universal.text, "v2.0.0");
    volt_draw_text(140, 3, 0.5f, 0.5f, settings.universal.text, "Universal-Updater");
	draw_Background_Bottom();
	draw_Bars_Bottom();
}

void draw_Bar(void) {
	draw_Background_Top();
	volt_draw_texture_blend(barTop, 0, 0, settings.universal.bars);
	volt_draw_text(4, 3, 0.5f, 0.5f, settings.universal.text, getTime().c_str());
    volt_draw_text(350, 3, 0.5f, 0.5f, settings.universal.text, "v2.0.0");
    volt_draw_text(140, 3, 0.5f, 0.5f, settings.universal.text, "Universal-Updater");
	draw_Background_Bottom();
	draw_Bars_Bottom();
}

void draw_SD(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	draw_Background_Top();
	if((access("sdmc:/Universal-Updater/Graphic/Image.png", F_OK) == 0)) {
	volt_draw_texture(SD, 0, 0);
	} else {
		volt_draw_texture_blend(barTop, 0, 0, settings.universal.bars);
	}
	draw_Background_Bottom();
	draw_Bars_Bottom();
	volt_draw_text(150, 225, 0.5f, 0.5f, settings.universal.text, "v2.0.0");
	volt_draw_text(0, 225, 0.5f, 0.5f, settings.universal.text, getTime().c_str());
}

void chooseLayout(void) {
	if (settings.universal.layout == 0) {
	draw_Bar();
} else if (settings.universal.layout == 1) {
	draw_Border();
} else if (settings.universal.layout == 2) {
	draw_SD();
}
}