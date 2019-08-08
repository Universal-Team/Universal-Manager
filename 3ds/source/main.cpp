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
#include "ptmu_x.h"
#include "mainMenuScreen.hpp"
#include "creditsScreen.hpp"
#include "settings.hpp"
#include "sound.h"

extern "C" {
	#include "music/error.h"
	#include "music/playback.h"
}

std::string currentSong;
std::vector<Playlist> nowPlayingList;
int locInPlaylist;
int musicRepeat;
bool musicShuffle;
bool firstSong;
bool dspfirmfound = false;
bool exiting = false;

touchPosition touch;

extern C3D_RenderTarget* top;
extern C3D_RenderTarget* bottom;

//Music and sound effects.
sound *sfx_scroll = NULL;
sound *sfx_pong = NULL;
sound *sfx_score = NULL;

void loadSoundEffects(void) {
	sfx_scroll = new sound("romfs:/sfx/scroll.wav", 2, false);
	sfx_pong = new sound("romfs:/sfx/pong.wav", 2, false);
	sfx_score = new sound("romfs:/sfx/score.wav", 2, false);
}


bool touching(touchPosition touch, Structs::ButtonPos button) {
	if (touch.px >= button.x && touch.px <= (button.x + button.w) && touch.py >= button.y && touch.py <= (button.y + button.h))
		return true;
	else
		return false;
}



int main()
{
	acInit();
	amInit();
	ptmuInit();	// For battery status
	ptmuxInit();	// For AC adapter status
	sdmcInit();
	Config::loadConfig();
	if (Config::Citra == 0) {
	mcuInit();
	} else if (Config::Citra == 1) {
	}
	romfsInit();
	cfguInit();
    gfxInitDefault();
	Gui::init();
	srand(time(NULL));

	if (Config::Credits == 0) { // Credits Screen if 1 and mainScreen if 0.
		Gui::setScreen(std::make_unique<MainMenu>());
	} else if (Config::Credits == 1) {
		Gui::setScreen(std::make_unique<Credits>());
	}

	osSetSpeedupEnable(true);	// Enable speed-up for New 3DS users


	// make folders if they don't exist
	mkdir("sdmc:/3ds", 0777);	// For DSP dump
	mkdir("sdmc:/Universal-Manager", 0777); // main Path.

 	if( access( "sdmc:/3ds/dspfirm.cdc", F_OK ) != -1 ) {
		ndspInit();
		dspfirmfound = true;
	 }

	 loadSoundEffects();

	// Loop as long as the status is not exit
    while (aptMainLoop() && !exiting)
    {
        hidScanInput();
        u32 hHeld = hidKeysHeld();
        u32 hDown = hidKeysDown();
		hidTouchRead(&touch);
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, BLACK);
        C2D_TargetClear(bottom, BLACK);
		Gui::clearTextBufs();
		Gui::mainLoop(hDown, hHeld, touch);

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
	delete sfx_scroll;
	delete sfx_pong;
	delete sfx_score;
	if (isPlaying()) {
	stopPlayback(); // This seems to do `ndspExit();` already. I hope the Crash is finally fixed?
	} else if (!isPlaying()) {
		ndspExit();
	}
	Gui::exit();
	gfxExit();
	cfguExit();
	romfsExit();
	sdmcExit();
	acExit();
	amExit();

    return 0;
}
