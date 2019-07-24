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

// Better Screen Modes. ;P
enum SCREEN_MODE {
	// MainScreen.
    mainScreen = 0,

	// Credits.
	creditsScreen = 1,

	// Music.
	musicMainScreen = 2,
	musicListScreen = 3,
	musicPlayerScreen = 4,
	musicPlaylistAddScreen = 5,
	musicPlaylistPlayScreen = 6,
	musicPlaylistEditScreen = 7,
	themeSelectorScreen = 8,

	// Settings.
	SettingsScreen = 9,

	// Image.
	ImageSelectorScreen = 10,
	showImageScreen = 11,

	// FTP.
	ftpScreen = 12,

	// Updater.
	updaterSubMenu = 13,
	TWLScreen = 14,
	OtherScreen = 15,
	CFWScreen = 16,
	UniversalScreen = 17,

	// FileManager.
	fileManager = 18,

	// Scripts.
	scriptMainScreen = 19,
	scriptCreatorFunctions = 20,

	// Text.
	TextEditorScreen = 21,
	textFileBrowse = 22,

	// Utils.
	utilsScreen = 23,
	buttonTesterScreen = 24,
	calendarScreen = 25,
	calculatorScreen = 26,

	// Games.
	gameSubMenuScreen = 27,
	pongScreen = 28,
	tictactoeScreen = 29
};

struct Playlist {
	std::string name;
	int position;
};

extern int SCREEN_MODE;

// Main Menu Screen.
void drawMainMenu(void);
void MainMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Credits Stuff.
void drawCreditsScreen(void);
void creditsLogic(u32 hDown, touchPosition touch);

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

// Utils Stuff.

void drawUtilsScreen(void);
void utilsLogic(u32 hDown, u32 hHeld, touchPosition touch);

// Calculator Stuff.
void drawCalculatorScreen(void);
void calculatorLogic(u32 hDown, u32 hHeld, touchPosition touch);