/*
*   This file is part of Universal-Manager
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

#include "graphic.h"
//#include "colors.hpp"
#include "textures.hpp"
#include "voltlib/volt.h"
#include <unistd.h>

void volt_draw_texture_scale_blend(size_t id, int x, int y, float scaleX, float scaleY, u32 color)
{
	volt_texture_select(id, x, y);
	volt_texture_blend(color);
	volt_texture_scale(scaleX, scaleY);
	volt_texture_draw();		
}

void volt_draw_texture_part_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, u32 color)
{
	volt_texture_select_part(id, x, y, xbegin, ybegin, width, height);
	volt_texture_blend(color);
	volt_texture_draw();		
}

void volt_draw_texture_part_scale(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY)
{
	volt_texture_select_part(id, x, y, xbegin, ybegin, width, height);
	volt_texture_scale(scaleX, scaleY);
	volt_texture_draw();
}

void volt_draw_texture_part_scale_blend(size_t id, int x, int y, int xbegin, int ybegin, int width, int height, float scaleX, float scaleY, u32 color)
{
	volt_texture_select_part(id, x, y, xbegin, ybegin, width, height);
	volt_texture_blend(color);
	volt_texture_scale(scaleX, scaleY);
	volt_texture_draw();		
}

void volt_draw_texture_rotate_flip_blend(size_t id, int x, int y, float angle, flipType fliptype, u32 color)
{
	volt_texture_select(id, x, y);
	volt_texture_rotate(angle);
	volt_texture_flip(fliptype);
	volt_texture_blend(color);
	volt_texture_draw();
}

//void volt_free_texture(size_t id)
//{
//	if (id >= MAX_TEXTURES)
//		return;
//	
//	if (!textures[id].allocated)
//		return;
//	
//	C3D_TexDelete(&textures[id].tex);
//	textures[id].width = 0;
//	textures[id].height = 0;
//	textures[id].allocated = false;
//}

void drawBarsTop(void) {
	volt_draw_texture_blend(barTopTop, 0, 0, settings.universal.bars);
	volt_draw_texture_blend(barTopBot, 0, 215, settings.universal.bars);
}

void drawBgTop(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	volt_draw_rectangle(0, 25, 400, 190, settings.universal.bg);
	volt_draw_texture(bgTop, 0, 25);
}

void displayTime(void) {
	volt_draw_text(4, 3, 0.7f, 0.7f, WHITE, getTime().c_str());
}

void drawBarsBot(void) {
	volt_draw_texture_blend(barBotTop, 0, 0, settings.universal.bars);
	volt_draw_texture_blend(barBotBot, 0, 215, settings.universal.bars);
}

void drawBgBot(void) {
	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
	volt_draw_rectangle(0, 25, 320, 190, settings.universal.bg);
	volt_draw_texture(bgBot, 0, 25);
}

void displayMsg(const char* text) {
	volt_begin_draw(GFX_TOP, GFX_LEFT);
	drawBgTop();
	drawBarsTop();
	displayTime();
	volt_draw_text(26, 32, 0.45f, 0.45f, settings.universal.bars, text);
	drawBgBot();
	drawBarsBot();
	volt_end_draw();
}


void drawMusicPlayerImage(void) {
		volt_draw_on(GFX_TOP, GFX_LEFT);
		volt_draw_text(20, 100, 0.7f, 0.7f, WHITE, "You didn't loaded an Image Yet.\nPlease Load an Image under Themes.");
		volt_draw_texture(MusicPlayerImage, 0, 25);
	}

void drawCover(void) {
	drawBgTop();
	volt_draw_texture(Cover, 185, 30);
}

void MusicPlayerImageChoice(void) {
	if (settings.universal.music == 0) {
		drawBgTop();
	} else if(settings.universal.music ==1) {
		drawCover();
	} else if(settings.universal.music == 2) {
		drawMusicPlayerImage();
	}
}

// Misc Stuff.
size_t MainMenuButton = 1;
size_t UpdaterButton = 3;
size_t Dot = 4;

// Main Menu Stuff.
size_t FileManagerIcon = 5;
size_t FTPIcon = 6;
size_t UpdaterIcon = 7;
size_t SettingsIcon = 8;

// FileManager Stuff.
size_t MusicIcon = 9;
size_t ImageIcon = 10;

// Music Player Stuff.
size_t LeftIcon = 11;
size_t PauseIcon = 12;
size_t PlayIcon = 13;
size_t PlayIconSmall = 14;
size_t PlaylistIcon = 15;
size_t RepeatIcon = 16;
size_t RightIcon = 17;
size_t ShuffleIcon = 18;
size_t PlaylistEditor = 19;

// Image Viewer!
size_t SDImage = 20;

size_t CreditsImage = 21;

size_t Cover = 22;
size_t MusicPlayerImage = 23;
size_t bgTop = 24;
size_t bgBot = 25;
size_t barTopTop = 26;
size_t barTopBot = 27;
size_t barBotTop = 28;
size_t barBotBot  = 29;
size_t RGBButton = 30;

void graphicsInit(void) {
	
	// Misc Stuff.
	volt_load_texture_png(MainMenuButton, "romfs:/graphics/Button/MainMenuButton.png");
	volt_load_texture_png(UpdaterButton, "romfs:/graphics/Button/UpdaterButton.png");
	volt_load_texture_png(Dot, "romfs:/graphics/Misc/Dot.png");
	volt_load_texture_png(bgTop, "romfs:/graphics/Misc/universal-bg-top.png");
	volt_load_texture_png(bgBot, "romfs:/graphics/Misc/universal-bg-bottom.png");
	volt_load_texture_png(RGBButton, "romfs:/graphics/Button/RGBButton.png");

	// Bars
	volt_load_texture_png(barTopTop, "romfs:/graphics/Misc/top-screen-top.png");
	volt_load_texture_png(barTopBot, "romfs:/graphics/Misc/top-screen-bot.png");
	volt_load_texture_png(barBotTop, "romfs:/graphics/Misc/bottom-screen-top.png");
	volt_load_texture_png(barBotBot, "romfs:/graphics/Misc/bottom-screen-bot.png");

	// Main Menu Stuff.
	volt_load_texture_png(FileManagerIcon, "romfs:/graphics/MainMenu/Filemanager Icon.png");
	volt_load_texture_png(FTPIcon, "romfs:/graphics/MainMenu/FTP Icon.png");
	volt_load_texture_png(UpdaterIcon, "romfs:/graphics/MainMenu/Updater Icon.png");
	volt_load_texture_png(SettingsIcon, "romfs:/graphics/MainMenu/Settings Icon.png");

	// FileManager Stuff.
	volt_load_texture_png(MusicIcon, "romfs:/graphics/FileManager/Music Icon.png");
	volt_load_texture_png(ImageIcon, "romfs:/graphics/FileManager/Image Icon.png");
	
	// Music Player Stuff.
	volt_load_texture_png(LeftIcon, "romfs:/graphics/MusicPlayer/Left Icon.png");
	volt_load_texture_png(PauseIcon, "romfs:/graphics/MusicPlayer/Pause Icon.png");
	volt_load_texture_png(PlayIcon, "romfs:/graphics/MusicPlayer/Play Icon.png");
	volt_load_texture_png(PlayIconSmall, "romfs:/graphics/MusicPlayer/Play Icon Small.png");
	volt_load_texture_png(PlaylistIcon, "romfs:/graphics/MusicPlayer/Playlist Icon.png");
	volt_load_texture_png(RepeatIcon, "romfs:/graphics/MusicPlayer/Repeat Icon.png");
	volt_load_texture_png(RightIcon, "romfs:/graphics/MusicPlayer/Right Icon.png");
	volt_load_texture_png(ShuffleIcon, "romfs:/graphics/MusicPlayer/Shuffle Icon.png");
	volt_load_texture_png(PlaylistEditor, "romfs:/graphics/MusicPlayer/Playlist Editor.png");
	volt_load_texture_png(Cover, "romfs:/graphics/MusicPlayer/Unknown.png");

	// Credits Stuff.
	volt_load_texture_png(CreditsImage, "romfs:/graphics/Misc/universal-credits.png");


	if((access("sdmc:/Universal-Manager/Theme/Image.png", F_OK) == 0)) {
		volt_load_texture_png(MusicPlayerImage, "sdmc:/Universal-Manager/Theme/Image.png");
	} else {
}
}

std::string secondsToString(u64 seconds) {
    int h = 0, m = 0, s = 0;
    h = (seconds / 3600);
    m = (seconds - (3600 * h)) / 60;
    s = (seconds - (3600 * h) - (m * 60));

	char string[35];

    if (h > 0) snprintf(string, sizeof(string), "%02d:%02d:%02d", h, m, s);
    else	snprintf(string, sizeof(string), "%02d:%02d", m, s);
	
	return string;
}
