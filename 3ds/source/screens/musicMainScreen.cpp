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

#include "screenCommon.hpp"
#include "musicMainScreen.hpp"
#include "settings.hpp"

#include "musicListScreen.hpp"
#include "musicPlayerScreen.hpp"
#include "playlistPlayScreen.hpp"
#include "themeSelectorScreen.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void MusicMain::Draw(void) const
{
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Music Player Menu"))/2, 0, 0.72f, WHITE, "Music Player Menu");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();

	Gui::sprite(sprites_mainMenuButton_idx, mainButtonPos[0].x, mainButtonPos[0].y);
	Gui::sprite(sprites_music_icon_idx, mainButtonPos[0].x+5, mainButtonPos[0].y+10);
	Draw_Text(40, 57, 0.7f, WHITE, "Songs");

	Gui::sprite(sprites_mainMenuButton_idx, mainButtonPos[1].x, mainButtonPos[1].y);
	Gui::Draw_ImageBlend(sprites_play_icon_small_glow_idx, mainButtonPos[1].x+5, mainButtonPos[1].y+10, Config::barColor);
	Gui::sprite(sprites_play_icon_small_normal_idx, mainButtonPos[1].x+5, mainButtonPos[1].y+10);
	Draw_Text(210, 57, 0.65f, WHITE, "Now Playing");

	Gui::sprite(sprites_mainMenuButton_idx, mainButtonPos[2].x, mainButtonPos[2].y);
	Gui::sprite(sprites_playlist_icon_idx, mainButtonPos[2].x+1, mainButtonPos[2].y+6);
	Draw_Text(37, 167, 0.65f, WHITE, "Playlists");

	 Gui::sprite(sprites_mainMenuButton_idx, mainButtonPos[3].x, mainButtonPos[3].y);
	 Gui::sprite(sprites_themes_idx, mainButtonPos[3].x+5, mainButtonPos[3].y+10);
	 Draw_Text(220, 167, 0.7f, WHITE, "Themes");

	if (selection == 0) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 12, 20, Config::barColor);
	} else if (selection == 1) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 182, 20, Config::barColor);
	} else if (selection == 2) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 12, 130, Config::barColor);
	} else if (selection == 3) {
		Gui::Draw_ImageBlend(sprites_arrow_idx, 182, 130, Config::barColor);
	}
}

void MusicMain::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(keysDown() & KEY_UP) {
		if(selection > 0)	selection--;
	} else if(keysDown() & KEY_DOWN) {
		if(selection < 3)	selection++;
		} else if(keysDown() & KEY_A) {
			switch(selection) {
				case 0: {
					Gui::setScreen(std::make_unique<MusicList>());
					break;
				} case 1:
					Gui::setScreen(std::make_unique<MusicPlayer>());
					break;
				  case 2: {
					Gui::setScreen(std::make_unique<PlaylistPlay>());
					break;
				} case 3: {
					Gui::setScreen(std::make_unique<ThemeSelector>());
					break;
				}
			}
	} else if(hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if(hDown & KEY_TOUCH) {
		for(uint i=0;i<(sizeof(mainButtonPos)/sizeof(mainButtonPos[0]));i++) {
			if (touching(touch, mainButtonPos[0])) {
				Gui::setScreen(std::make_unique<MusicList>());
			} else if (touching(touch, mainButtonPos[1])) {
				Gui::setScreen(std::make_unique<MusicPlayer>());
			} else if (touching(touch, mainButtonPos[2])) {
				Gui::setScreen(std::make_unique<PlaylistPlay>());
				selectedPlst = 0;
				dirChanged = true;
			} else if (touching(touch, mainButtonPos[3])) {
				Gui::setScreen(std::make_unique<ThemeSelector>());
			}
		}
	}
}