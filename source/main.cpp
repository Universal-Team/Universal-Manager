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

#include "dumpdsp.hpp"
#include "graphic.h"
#include "screens/screenCommon.hpp"
#include "settings.h"
#include "sfx.hpp"
#include "textures.hpp"
#include "voltlib/volt.h"
#include "keyboard.h"
#include "download/download.hpp"

extern "C" {
	#include "music/error.h"
	#include "music/playback.h"
}

#define CONFIG_3D_SLIDERSTATE (*(float *)0x1FF81080)

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

// Music and sound effects.
sound *sfx_example = NULL;

// 3D offsets. (0 == Left, 1 == Right)
Offset3D offset3D[2] = {0.0f, 0.0f};	

int screenMode = 0;
bool dspfirmfound = false;
static touchPosition touch;

extern std::string currentSong;
extern std::vector<Playlist> nowPlayingList;
extern int locInPlaylist;
extern int musicRepeat;
extern bool musicShuffle;
extern bool firstSong;

ButtonPos mainScreenButtonPos[] = {
    {0, 40, 149, 52, fileScreen},
    {170, 40, 149, 52, ftpScreen},
    {0, 150, 149, 52, updaterSubMenu},
    {170, 150, 149, 52, settingsScreen},
};

ButtonPos fileScreenButtonPos[] = {
    {100, 40, 149, 52, musicMainScreen},
	{100, 120, 149, 52, imageScreen},
    {288, 208, 32, 32, mainScreen},
};

ButtonPos settingsScreenButtonPos[] = {
    {0, 40, 149, 52, creditsScreen},
    {288, 208, 32, 32, mainScreen},
	{170, 40, 149, 52, uiSettingsScreen},
};

ButtonPos uisettingsScreenButtonPos[] = {
    {288, 208, 32, 32, settingsScreen},
};

ButtonPos updaterScreenButtonPos[] = {
    {288, 208, 32, 32, mainScreen},
};

ButtonPos creditsScreenButtonPos[] = {
    {288, 208, 32, 32, settingsScreen},
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


void loadSoundEffects(void) {
	if (dspfirmfound) {
		sfx_example = new sound("romfs:/sfx/example.wav", 2, false);
	}
}

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

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users

	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Manager", 0777); // main Path.

	volt_init();

	volt_set_3D(1);
	LoadUniversalSettings();

	graphicsInit();

	if (access("sdmc:/3ds/dspfirm.cdc", F_OK) != -1) {
		ndspInit();
		dspfirmfound = true;
	} else {
		volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
		volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "Dumping DSP firm...");
		volt_end_draw();
		dumpDsp();
		if (access("sdmc:/3ds/dspfirm.cdc", F_OK) != -1) {
			ndspInit();
			dspfirmfound = true;
		} else {
			for (int i = 0; i < 90; i++) {
				volt_begin_draw(GFX_BOTTOM, GFX_LEFT);
				volt_draw_text(12, 16, 0.5f, 0.5f, WHITE, "DSP firm dumping failed.\n"
						"Running without sound.");
				volt_end_draw();
			}	
		}
	}

	loadSoundEffects();

	int fadealpha = 255;

	//displayMsg("Checking for Updates.. please wait.");
	//			checkForUpdates();							// Maybe for later? 
	
	// Loop as long as the status is not exit
	while(aptMainLoop()) {
		offset3D[0].topbg = CONFIG_3D_SLIDERSTATE * -7.0f;
		offset3D[1].topbg = CONFIG_3D_SLIDERSTATE * 7.0f;

		// Scan hid shared memory for input events
		gspWaitForVBlank();
		hidScanInput();
		const u32 hDown = hidKeysDown();
		const u32 hHeld = hidKeysHeld();
		hidTouchRead(&touch);

		for (int topfb = GFX_LEFT; topfb <= GFX_RIGHT; topfb++) {
			if (topfb == GFX_LEFT) volt_begin_draw(GFX_TOP, (gfx3dSide_t)topfb);
			else volt_draw_on(GFX_TOP, (gfx3dSide_t)topfb);
			if (fadealpha > 0) volt_draw_rectangle(0, 0, 400, 240, RGBA8(0, 0, 0, fadealpha)); // Fade in/out effect
		}

		// Draws a screen based on screenMode
		switch(screenMode) {
			case mainScreen:
				drawMainMenu();				// Draws the Main Menu screen
				break;
			case fileScreen:
				drawFileManagerSubMenu();	// Draws the File Manager screen
				break;
			case creditsScreen:
				drawCredits();				// Draws the Credits screen
				break;
			case updaterSubMenu:
				drawUpdaterSubMenu();		// Draws the Updater screen
				break;
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
			case settingsScreen:
				drawSettingsScreen();		// Draws the Settings screen
				break;
			case imageScreen:
				drawImageViewerUI();		// Draw the Image Viewer screen
				break;
			case uiSettingsScreen:
				drawUISettingsScreen();
				break;
			case ftpScreen:
				drawFTPScreen();
				break;
			case fileManagerScreen:
				drawFileManagerScreen();
				break;
			case TWLScreen:
				drawUpdaterTWL();
				break;
			case OtherScreen:
				drawUpdaterOther();
				break;
		}

		// Scans inputs for the current screen
		switch(screenMode) {
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
			case fileScreen:
				if (hDown & KEY_B) {
					screenMode = mainScreen;
				} else if (hDown & KEY_A) {
					screenMode = musicListScreen;
				} else if (hDown & KEY_X) {
					screenMode = fileManagerScreen;
				} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(fileScreenButtonPos)/sizeof(fileScreenButtonPos[0]));i++) {
						if (touching(touch, fileScreenButtonPos[i])) {
							screenMode = fileScreenButtonPos[i].link;
						}
					}
				}
				break;
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
			case updaterSubMenu:
				updaterSubMenuLogic(hDown, touch);
				break;
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
			case settingsScreen:
			if (hDown & KEY_B) {
				screenMode = mainScreen;
			} else if (hDown & KEY_X) {
				screenMode = uiSettingsScreen;
			} else if (hDown & KEY_TOUCH) {
					for(uint i=0;i<(sizeof(settingsScreenButtonPos)/sizeof(settingsScreenButtonPos[0]));i++) {
						if (touching(touch, settingsScreenButtonPos[i])) {
							screenMode = settingsScreenButtonPos[i].link;
						}
					}
				}
				break;
			case imageScreen:
			if (hDown & KEY_B) {
				screenMode = fileScreen;
				break;
			case uiSettingsScreen:
				uiSettingsLogic(hDown, touch);
				break;
			case ftpScreen:
			if (hDown & KEY_B) {  // Later : "ftpLogic".
				screenMode = mainScreen;
			}
				break;
			case fileManagerScreen:
			if(hDown & KEY_B) {
				screenMode = fileScreen;
			}
				break;
			case TWLScreen:
				updaterTWLLogic(hDown, touch);
				break;
			case OtherScreen:
				updaterOtherLogic(hDown, touch);
				break;
			}
		}

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
	}
	
	delete sfx_example;
	if (dspfirmfound) {
		ndspExit();
	}
	SaveUniversalSettings();
	hidExit();
	srvExit();
	romfsExit();
	sdmcExit();
	aptExit();

    return 0;
}
