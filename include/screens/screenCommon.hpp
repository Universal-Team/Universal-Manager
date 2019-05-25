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
//###############################
#define fileScreen				1
//###############################
#define creditsScreen			2
//###############################
#define musicMainScreen			3
//###############################
#define musicListScreen			4
#define musicPlayerScreen		5
#define musicPlaylistAddScreen	6
#define musicPlaylistPlayScreen	7
#define musicPlaylistEditScreen	8
#define themeSelectorScreen 	9
//###############################
#define settingsScreen			10
#define uiSettingsScreen		11
//###############################
#define PNGScreen				12
#define BMPScreen				13
#define showImageScreen			14
//###############################
#define ftpScreen				15
//###############################
#define fileManagerScreen		16
//###############################
#define updaterSubMenu			17
#define OtherScreen				18
#define TWLScreen				19
#define CFWScreen				20
//###############################

struct Playlist {
	std::string name;
	int position;
};

extern int screenMode;

// Main Menu Screen.
void drawMainMenu(void);

// File Manager Sub Menu Screen.
void drawFileManagerSubMenu(void);
void drawFileManagerScreen(void);

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
void drawMusicPlaylistEdit(void);
void musicPlaylistEditLogic(u32 hDown, u32 hHeld);
void drawThemeSelector(void);
void themeSelectorLogic(u32 hDown, u32 hHeld);

// Updater Screens
// Sub Menu.
void drawUpdaterSubMenu(void);
void updaterSubMenuLogic(u32 hDown, touchPosition touch);
// TWL Screen.
void drawUpdaterTWL(void);
void updaterTWLLogic(u32 hDown, touchPosition touch);
// Other Screen
void drawUpdaterOther(void);
void updaterOtherLogic(u32 hDown, touchPosition touch);
// CFW Screen
void drawUpdaterCFW(void);
void updaterCFWLogic(u32 hDown, touchPosition touch);

// Image Viewer!
void drawPNGImageViewerUI(void);
void PNGSelectorLogic(u32 hDown, u32 hHeld);
void drawBMPImageViewerUI(void);
void BMPSelectorLogic(u32 hDown, u32 hHeld);
void showImage(void);
void showImageLogic(u32 hDown, touchPosition touch);

// Miscs.
void saveMsg(void);
bool confirmPopup(std::string msg);
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos);

// FTP Stuff.
void drawFTPScreen(void);