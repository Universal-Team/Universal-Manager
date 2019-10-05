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
#include "logging.hpp"
#include "ptmu_x.h"

#include "lang/lang.h"

#include "screens/creditsScreen.hpp"
#include "screens/mainMenuScreen.hpp"
#include "screens/screenCommon.hpp"

#include "utils/settings.hpp"
#include "utils/sound.h"

#include <3ds.h>
#include <algorithm>
#include <dirent.h>
#include <unistd.h>

extern "C" {
	#include "music/error.h"
	#include "music/playback.h"
}

bool dspfirmfound = false;
bool exiting = false;
int fadealpha = 255;
bool fadein = true;

extern bool decForRepeat2;
extern uint selectedPlstItem;
extern int movingPlstItem;

extern bool firstSong;
extern int locInPlaylist;
extern int musicRepeat;
extern bool musicShuffle;
extern std::vector<Playlist> nowPlayingList;
extern std::string currentSong;

bool isCitra = false; // Set it to true, if it is used with Citra.


touchPosition touch;

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

bool is3dsx;
bool Is3dsxUpdated = false;
std::string path3dsx;

void getCurrentUsage(){
	u64 id;
	APT_GetProgramID(&id);

	if(id == 0x0004000004394100){
		is3dsx = false;
		return;
	}

	is3dsx = true;
}


// If an Error while startup appears, Return this!

// If an Error while startup appears, Return this!

static Result DisplayStartupError(std::string message, Result res)
{
		std::string errorMsg = std::to_string(res);
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, BLACK);
		C2D_TargetClear(bottom, BLACK);
		Gui::clearTextBufs();
		set_screen(top);
		Draw_Rect(0, 0, 400, 27, BARCOLOR);
		Draw_Rect(0, 27, 400, 186, GRAY);
		Draw_Rect(0, 213, 400, 27, BARCOLOR);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Oh no, an error occured!"))/2, 2, 0.8f, WHITE, "Oh no, an error occured!", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Description: "+ message))/2, 155, 0.8f, WHITE, "Description: "+message, 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Press Start to exit."))/2, 213, 0.8f, WHITE, "Press Start to exit.", 400);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, "Error during Startup: "+errorMsg))/2, 80, 0.8f, WHITE, "Error during Startup: "+errorMsg, 400);
		set_screen(bottom);
		Draw_Rect(0, 0, 320, 27, BARCOLOR);
		Draw_Rect(0, 27, 320, 186, GRAY);
		Draw_Rect(0, 213, 320, 27, BARCOLOR);
		C3D_FrameEnd(0);

		// For the Log.
		std::string error = message;
		error += ", ";
		error += std::to_string(res);
		Logging::writeToLog(error.c_str());

		gspWaitForVBlank();
		while (aptMainLoop() && !(hidKeysDown() & KEY_START))
		{
			hidScanInput();
		}
	return res;
}

void loadMessage(std::string Message) {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, BLACK);
		C2D_TargetClear(bottom, BLACK);
		Gui::clearTextBufs();
		set_screen(top);
		Draw_Rect(0, 0, 400, 27, BARCOLOR);
		Draw_Rect(0, 27, 400, 186, GRAY);
		Draw_Rect(0, 213, 400, 27, BARCOLOR);
		Gui::DrawString((400-Gui::GetStringWidth(0.8f, Message))/2, 2, 0.8f, WHITE, Message, 400);
		set_screen(bottom);
		Draw_Rect(0, 0, 320, 27, BARCOLOR);
		Draw_Rect(0, 27, 320, 186, GRAY);
		Draw_Rect(0, 213, 320, 27, BARCOLOR);
		C3D_FrameEnd(0);
}

int main()
{
	gfxInitDefault();
	Result res;

	Gui::init();
	loadMessage("Initialize everything.. please wait.");

	if (R_FAILED(res = romfsInit())) {
		return DisplayStartupError("romfsInit failed.", res);
	}

	if (R_FAILED(res = sdmcInit())) {
		return DisplayStartupError("sdmcInit failed.", res);
	}

	Gui::loadSheetsAndFont();

	Logging::createLogFile(); // Create Log File, if it doesn't exists already.

	if (R_FAILED(res = acInit())) {
		return DisplayStartupError("acInit failed.", res);
	}

	if (R_FAILED(res = amInit())) {
		return DisplayStartupError("amInit failed.", res);
	}

	// For battery status
	if (R_FAILED(res = ptmuInit())) {
		return DisplayStartupError("ptmuInit failed.", res);
	}

	// For AC adapter status
	if (R_FAILED(res = ptmuxInit())) {
		return DisplayStartupError("ptmuxInit failed.", res);
	}

	Config::loadConfig();
	Lang::loadLangStrings(1);

	if (isCitra == false) {
		mcuInit();
	} else if (isCitra == true) {
	}

	if (R_FAILED(res = cfguInit())) {
		return DisplayStartupError("cfguInit failed.", res);
	}

	srand(time(NULL));
	getCurrentUsage();
	
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	if (is3dsx == true) {
			path3dsx = path;
			if (path3dsx == "sdmc:/") {
				path3dsx = path3dsx.substr(5, path3dsx.size());
			} else {
		}
	}


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

	// We write a successfull Message, because it launched Successfully. Lol.
	Logging::writeToLog("Universal-Manager launched successfully!");

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
		displayBatteryNearlyToDead();
		C3D_FrameEnd(0);

		if (fadein == true) {
			fadealpha -= 3;
			if (fadealpha < 0) {
				fadealpha = 0;
				fadein = false;
			}
		}

		if (Is3dsxUpdated == true) {
			break;
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

	delete sfx_scroll;
	delete sfx_pong;
	delete sfx_score;
	stopPlayback();
	if (dspfirmfound == true) {
		ndspExit();
	}
	Gui::exit();
	gfxExit();
	cfguExit();
	romfsExit();
	acExit();
	amExit();
	Logging::writeToLog("Universal-Manager closing successfully!");
	sdmcExit();
	return 0;
}
