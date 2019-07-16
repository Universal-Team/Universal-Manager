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
#define creditsScreen			1
//###############################
#define musicMainScreen			2
//###############################
#define musicListScreen			3
#define musicPlayerScreen		4
#define musicPlaylistAddScreen	5
#define musicPlaylistPlayScreen	6
#define musicPlaylistEditScreen	7
#define themeSelectorScreen 	8
//###############################
#define SettingsScreen			9
//###############################
#define ImageSelectorScreen		10
#define showImageScreen			11
//###############################
#define ftpScreen				12
//###############################
#define updaterSubMenu			13
#define OtherScreen				14
#define TWLScreen				15
#define CFWScreen				16
#define UniversalScreen			17
//###############################
#define fileManager				18
#define scriptMainScreen		19
#define TextEditorScreen		20
#define buttonTesterScreen		21
#define scriptCreatorFunctions	22
#define calendarScreen			23
#define textFileBrowse			24
#define gameSubMenuScreen		25
#define pongScreen				26
#define tictactoeScreen			27

struct Playlist {
	std::string name;
	int position;
};

extern int screenMode;

// Main Menu Screen.
void drawMainMenu(void);
void MainMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Settings Screen.
void drawSettingsScreen(void);
void SettingsLogic(u32 hDown, u32 hHeld, touchPosition touch);

 //Music Player Screen.
void drawMusicMain(void);
void musicMainLogic(u32 hDown, u32 hHeld, touchPosition touch);
void musicListLogic(u32 hDown, u32 hHeld);
void drawMusicList(void);
void musicPlayerLogic(u32 hDown, u32 hHeld, touchPosition touch);
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
void updaterSubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);
// TWL Screen.
void drawUpdaterTWL(void);
void updaterTWLLogic(u32 hDown, u32 hHeld, touchPosition touch);
// Other Screen
void drawUpdaterOther(void);
void updaterOtherLogic(u32 hDown, u32 hHeld, touchPosition touch);
// CFW Screen
void drawUpdaterCFW(void);
void updaterCFWLogic(u32 hDown, u32 hHeld, touchPosition touch);
// Universal Screen.
void drawUniversalScreen(void);
void UniversalLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Image Viewer!
void drawImageSelectorScreen(void);
void ImageSelectorLogic(u32 hDown, u32 hHeld);
void showImage(void);
void showImageLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Miscs.
//void saveMsg(void);
bool confirmPopup(std::string msg);
bool confirmPopup(std::string msg1, std::string msg2, std::string yes, std::string no, int ynXPos);
void helperBox(std::string msg1);
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
void drawScriptsCreatorFunctions(void);
void scriptCreatorFunctionsLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Text Editor Stuff.
void drawTextEditorScreen(void);
void TextEditorLogic(u32 hDown, u32 hHeld);
void drawTextFileBrowse(void);
void textFileBrowseLogic(u32 hDown, u32 hHeld);

// Button Tester Screen stuff.
void drawButtonTesterScreen(void);
void buttonTesterLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Calendar Stuff.
void drawCalendarScreen(void);
void calendarLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Game Sub Menu.
void drawGamesSubMenuScreen(void);
void gamesSubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Pong Screen.
void drawPongScreen(void);
void pongLogic(u32 hDown, u32 hHeld);

// Tic-Tac-Toe Screen.
void drawTicTacToeScreen(void);
void ticTacToeLogic(u32 hDown, u32 hHeld, touchPosition touch);