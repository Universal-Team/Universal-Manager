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
#include <citro3d.h>
#include <citro2d.h>
#include <3ds.h>
#include <algorithm>
#include <dirent.h>
#include <malloc.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "gui.hpp"
#include "screenCommon.hpp"
#include "settings.hpp"
#include "ptmu_x.h"

extern "C" {
	#include "music/error.h"
	#include "music/playback.h"
}

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

extern std::string currentSong;
extern std::vector<Playlist> nowPlayingList;
extern int locInPlaylist;
extern int musicRepeat;
extern bool musicShuffle;
extern bool firstSong;

// Music and sound effects.
//sound *sfx_scroll = NULL;

static touchPosition touch;
extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;	
int screenMode = 0;


ButtonPos mainScreenButtonPos[] = {
    {0, 40, 149, 52, fileScreen},
    {170, 40, 149, 52, ftpScreen},
	{0, 150, 149, 52, updaterSubMenu},
    {170, 150, 149, 52, settingsScreen},
};

ButtonPos fileScreenButtonPos[] = {
    {100, 40, 149, 52, musicMainScreen},
//	{100, 120, 149, 52, PNGScreen},
    {293, 213, 27, 27, mainScreen},
};

ButtonPos settingsScreenButtonPos[] = {
    {0, 40, 149, 52, creditsScreen},
    {293, 213, 27, 27, mainScreen},
	{170, 40, 149, 52, uiSettingsScreen},
};

ButtonPos uisettingsScreenButtonPos[] = {
    {293, 213, 27, 27, settingsScreen},
};

ButtonPos updaterScreenButtonPos[] = {
    {293, 213, 27, 27, mainScreen},
};

ButtonPos creditsScreenButtonPos[] = {
    {293, 213, 27, 27, settingsScreen},
};


void screenoff()
{
    gspLcdInit();\
    GSPLCD_PowerOffBacklight(GSPLCD_SCREEN_BOTH);\
    gspLcdExit();
}

void screenon()
{
    gspLcdInit();\
    GSPLCD_PowerOnBacklight(GSPLCD_SCREEN_BOTH);\
    gspLcdExit();
}


/* void loadSoundEffects(void) {
	if (dspfirmfound) {
		sfx_scroll = new sound("romfs:/sfx/scroll.wav", 2, false);
	}
}*/

bool touching(touchPosition touch, ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}


int main()
{
	aptInit();
	amInit();
	sdmcInit();
	romfsInit();
	srvInit();
	hidInit();
	acInit();
	LoadUniversalSettings();
    gfxInitDefault();
	Gui::init();
	ptmuInit();	// For battery status
	ptmuxInit();	// For AC adapter status
	//mcuInit(); // Comment this out, if you use Citra.

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users


	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Manager", 0777); // main Path.

	//loadSoundEffects();

	// Loop as long as the status is not exit
    while (aptMainLoop())
    {
        hidScanInput();
        u32 hHeld = hidKeysHeld();
        u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, BLUE2);
        C2D_TargetClear(bottom, BLUE2);

		// Draws a screen based on screenMode
		switch(screenMode) {
//#########################################################################################################
			case mainScreen:
				drawMainMenu();				// Draws the Main Menu screen
				break;
//#########################################################################################################
			case fileScreen:
				drawFileManagerSubMenu();	// Draws the File Manager screen
				break;
//#########################################################################################################
			case creditsScreen:
				drawCredits();				// Draws the Credits screen
				break;
//#########################################################################################################
			case musicMainScreen:
				drawMusicMain();			// Draws the Music Player song selection screen
				break;
			case musicListScreen:
				drawMusicList();			// Draws the Music Player song selection screen
				break;
			case musicPlayerScreen:
				drawMusicPlayer();			// Draws the Music Player playback screen
				break;
			case musicPlaylistAddScreen:
				drawMusicPlaylistAdd();		// Draws the Music Player playlist creation screen
				break;
			case musicPlaylistPlayScreen:
				drawMusicPlaylistPlay();		// Draws the Music Player playlist selection screen
				break;
			case musicPlaylistEditScreen:
				drawMusicPlaylistEdit();		// Draws the Music Player playlist selection screen
				break;
//			case themeSelectorScreen:
//				drawThemeSelector();
//				break;
//#########################################################################################################
			case settingsScreen:
				drawSettingsScreen();		// Draws the Settings screen
				break;
			case uiSettingsScreen:
				drawUISettingsScreen();
				break;
			case uiSettingsScreen2:
				drawUISettingsScreen2();
				break;
//#########################################################################################################
//			case PNGScreen:
//				drawPNGImageViewerUI();		// Draw the Image Viewer screen [PNG]
//				break;
//			case BMPScreen:
//				drawBMPImageViewerUI();		// Draw the Image Viewer screen [BMP]
//				break;
//			case showImageScreen:
//				showImage();
//				break;
//#########################################################################################################
			case ftpScreen:
				drawFTPScreen();
				break;
//#########################################################################################################
			case updaterSubMenu:
				drawUpdaterSubMenu();		// Draws the Updater screen
				break;
			case TWLScreen:
				drawUpdaterTWL();
				break;
			case OtherScreen:
				drawUpdaterOther();
				break;
			case CFWScreen:
				drawUpdaterCFW();
				break;
//#########################################################################################################
		}

		// Scans inputs for the current screen
		switch(screenMode) {
//#########################################################################################################
			case mainScreen:
				if (hDown & KEY_A) {
					screenMode = fileScreen;
				} else if (hDown & KEY_X) {
					screenMode = ftpScreen;
				} else if (hDown & KEY_Y) {
					screenMode = updaterSubMenu;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(mainScreenButtonPos)/sizeof(mainScreenButtonPos[0]));i++) {
						if (touching(touch, mainScreenButtonPos[i])) {
							screenMode = mainScreenButtonPos[i].link;
						}
					}
				}
				break;
//#########################################################################################################
			case fileScreen:
				if (hDown & KEY_B) {
					screenMode = mainScreen;
				} else if (hDown & KEY_A) {
					screenMode = musicMainScreen;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(fileScreenButtonPos)/sizeof(fileScreenButtonPos[0]));i++) {
						if (touching(touch, fileScreenButtonPos[i])) {
							screenMode = fileScreenButtonPos[i].link;
						}
					}
				}
				break;
//#########################################################################################################
			case creditsScreen:
				if (hDown & KEY_B) {
					screenMode = settingsScreen;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(creditsScreenButtonPos)/sizeof(creditsScreenButtonPos[0]));i++) {
						if (touching(touch, creditsScreenButtonPos[i])) {
							screenMode = creditsScreenButtonPos[i].link;
						}
					}
				}
				break;
//#########################################################################################################
			case musicMainScreen:
				musicMainLogic(hDown, touch);
				break;
			case musicListScreen:
				musicListLogic(hDown, hHeld);
				break;
			case musicPlayerScreen:
				musicPlayerLogic(hDown, touch);
				break;
			case musicPlaylistAddScreen:
				musicPlaylistAddLogic(hDown, hHeld);
				break;
			case musicPlaylistPlayScreen:
				musicPlaylistPlayLogic(hDown, hHeld);
				break;
			case musicPlaylistEditScreen:
				musicPlaylistEditLogic(hDown, hHeld);
				break;
//			case themeSelectorScreen:
//				themeSelectorLogic(hDown, hHeld);
//				break;
//#########################################################################################################
			case settingsScreen:
			if (hDown & KEY_B) {
				screenMode = mainScreen;
			} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(settingsScreenButtonPos)/sizeof(settingsScreenButtonPos[0]));i++) {
						if (touching(touch, settingsScreenButtonPos[i])) {
							screenMode = settingsScreenButtonPos[i].link;
						}
					}
				}
				break;
			case uiSettingsScreen:
				uiSettingsLogic(hDown, touch);
				break;
			case uiSettingsScreen2:
				uiSettingsLogic2(hDown, touch);
				break;
//#########################################################################################################
//			case PNGScreen:
//			PNGSelectorLogic(hDown, hHeld);
//				break;
//			case BMPScreen:
//			BMPSelectorLogic(hDown, hHeld);
//				break;
//			case showImageScreen:
//			showImageLogic(hDown, touch);
//				break;
//#########################################################################################################
			case ftpScreen:
			ftpLogic(hDown, touch);
				break;
//#########################################################################################################
			case updaterSubMenu:
				updaterSubMenuLogic(hDown, touch);
				break;
			case TWLScreen:
				updaterTWLLogic(hDown, touch);
				break;
			case OtherScreen:
				updaterOtherLogic(hDown, touch);
				break;
			case CFWScreen:
				updaterCFWLogic(hDown, touch);
				break;
			}
//#########################################################################################################
		if (!isPlaying() && ((int)nowPlayingList.size()-1 > locInPlaylist || ((int)nowPlayingList.size() > 0 && musicRepeat))) {
			if (locInPlaylist > (int)nowPlayingList.size()-2 && musicRepeat != 2)	locInPlaylist = -1;
			if (musicRepeat != 2 && !firstSong) {
				locInPlaylist++;
			}
			firstSong = false;
			currentSong = nowPlayingList[locInPlaylist].name;
			playbackInfo_t playbackInfo;
			changeFile(currentSong.c_str(), &playbackInfo);
		} else if (isPlaying() && currentSong == "") {
			stopPlayback();
		} else if (!isPlaying() && currentSong != "") {
			currentSong = "";
		}
		if (hDown & KEY_START && screenMode == mainScreen) 
		{
			break;
		}
		
        C3D_FrameEnd(0);
        Gui::clearTextBufs();
    }

	SaveUniversalSettings();
	Gui::exit();
	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}
