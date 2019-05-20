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
#include "textures.hpp"
#include "universal-Settings.hpp"

#define mainScreen				0
#define fileScreen				1
#define creditsScreen			2
#define updaterScreen			3
#define musicMainScreen			4
#define musicListScreen			5
#define musicPlayerScreen		6
#define musicPlaylistAddScreen	7
#define musicPlaylistPlayScreen	8
#define settingsScreen			9
#define imageScreen				10
#define uiSettingsScreen        11
#define ftpScreen               12

struct Playlist {
    std::string name;
    int position;
};

extern int screenMode;

// Main Menu Screen.
void drawMainMenu(void);

// File Manager Sub Menu Screen.
void drawFileManagerSubMenu(void);

// Settings Screen.
void drawSettingsScreen(void);
void drawCredits(void);
void drawUISettingsScreen(void);
void uiSettingsLogic(u32 hDown, touchPosition touch);

// Music Player Screen.
void drawMusicMain(void);
void musicMainLogic(u32 hDown, touchPosition touch);
void musicListLogic(u32 hDown, u32 hHeld);
void drawMusicList(void);
void musicPlayerLogic(u32 hDown, touchPosition touch);
void drawMusicPlayer(void);
void drawMusicPlaylistAdd(void);
void musicPlaylistAddLogic(u32 hDown, u32 hHeld);
void drawMusicPlaylistPlay(void);
void musicPlaylistPlayLogic(u32 hDown, u32 hHeld);

// Updater Screen.
void drawUpdaterScreen(void);
void updaterLogic(u32 hDown, touchPosition touch);

// Image Viewer!
void drawImageViewerUI(void);

// Miscs.

void saveMsg(void);

// FTP Stuff.
void drawFTPScreen(void);