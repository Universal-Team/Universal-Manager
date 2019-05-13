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
//#include "Colors.hpp"
#include "Textures.hpp"
#include "voltlib/volt.h"

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

// Misc Stuff.
size_t MainMenuButton = 1;
size_t BackIcon = 2;
// Main Menu Stuff.
size_t FileManagerIcon = 3;
size_t FTPIcon = 4;
size_t UpdaterIcon = 5;
size_t SettingsIcon = 6;

// FileManager Stuff.
size_t MusicIcon = 7;

// Music Player Stuff.
size_t PlayIcon = 8;
size_t PauseIcon = 9;

void graphicsInit(void) {
	
	// Misc Stuff.
	volt_load_texture_png(MainMenuButton, "romfs:/graphics/Button/Button.png");
	volt_load_texture_png(BackIcon, "romfs:/graphics/Misc/Back Icon.png");

	// Main Menu Stuff.
	volt_load_texture_png(FileManagerIcon, "romfs:/graphics/MainMenu/Filemanager Icon.png");
	volt_load_texture_png(FTPIcon, "romfs:/graphics/MainMenu/FTP Icon.png");
	volt_load_texture_png(UpdaterIcon, "romfs:/graphics/MainMenu/Updater Icon.png");
	volt_load_texture_png(SettingsIcon, "romfs:/graphics/MainMenu/Settings Icon.png");

	// FileManager Stuff.
	volt_load_texture_png(MusicIcon, "romfs:/graphics/FileManager/Music Icon.png");

	// Music Player Stuff.
	volt_load_texture_png(PlayIcon, "romfs:/graphics/MusicPlayer/Play Icon.png");
	volt_load_texture_png(PauseIcon, "romfs:/graphics/MusicPlayer/Pause Icon.png");
}
	