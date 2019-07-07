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
#include "animation.hpp"
#include "sound.h"

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

#define mainScreen				0
//###############################
#define mainScreen2				1
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
#define uiSettingsScreen		10
#define uiSettingsScreen2		11
//###############################
#define ImageSelectorScreen		12
#define showImageScreen			13
//###############################
#define ftpScreen				14
//###############################
#define updaterSubMenu			15
#define OtherScreen				16
#define TWLScreen				17
#define CFWScreen				18
#define UniversalScreen			19
//###############################
#define fileManager				20
#define scriptMainScreen		21
#define TextEditorScreen		22

struct Playlist {
	std::string name;
	int position;
};

extern int screenMode;

// Main Menu Screen.
void drawMainMenu(void);
void MainMenu1Logic(u32 hDown, touchPosition touch);
void drawMainMenu2(void);
void MainMenu2Logic(u32 hDown, touchPosition touch);

// Settings Screen.
void drawUISettingsScreen(void);
void uiSettingsLogic(u32 hDown, touchPosition touch);
void drawUISettingsScreen2(void);
void uiSettingsLogic2(u32 hDown, touchPosition touch);

 //Music Player Screen.
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
// Universal Screen.
void drawUniversalScreen(void);
void UniversalLogic(u32 hDown, touchPosition touch);

// Image Viewer!
void drawImageSelectorScreen(void);
void ImageSelectorLogic(u32 hDown, u32 hHeld);
void showImage(void);
void showImageLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Miscs.
//void saveMsg(void);
bool confirmPopup(std::string msg);
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos);
bool helperBox(std::string msg1);
void drawCredits(void);

// FTP Stuff.
void drawFTPScreen(void);

// File Manager.
void drawFileBrowse(void);
void fileManagerLogic(u32 hDown, u32 hHeld, touchPosition touch);
bool displayActionBox(void);

// Script Screen Stuff.
void drawScriptMainScreen(void);
void scriptMainScreenLogic(u32 hDown, u32 hHeld);

// Text Editor Stuff.
void drawTextEditorScreen(void);
void TextEditorLogic(u32 hDown, u32 hHeld);