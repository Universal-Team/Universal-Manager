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

#include "screens/screenCommon.hpp"
#include <unistd.h>
#include <vector>

#include "fileBrowse.h"

extern "C" {
	#include "music/playback.h"
}

uint selectedFile = 0;
int keyRepeatDelay = 3;
bool dirChanged = true;
std::vector<DirEntry> dirContents;
std::string scanDir;
std::string currentSong = "";
std::vector<std::string> nowPlayingList;

void drawMusicPlayerUI(void) {
	 // Theme Stuff.
	if (settings.universal.theme == 0) {
		drawBgTop();
		drawBarsTopLight();
	} else if (settings.universal.theme == 1) {
		drawBgTop();
		drawBarsTopDark();
	} else if (settings.universal.theme == 2) {
		drawBgTop();
		drawBarsTopRed();
	}
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Music Player Menu");

	if (dirChanged) {
		dirContents.clear();
		std::vector<DirEntry> dirContentsTemp;
		getDirectoryContents(dirContentsTemp);
		for(uint i=0;i<dirContentsTemp.size();i++) {
			if ((strcasecmp(dirContentsTemp[i].name.substr(dirContentsTemp[i].name.length()-3, 3).c_str(), "mp3") == 0 ||
				// strcasecmp(dirContentsTemp[i].name.substr(dirContentsTemp[i].name.length()-3, 3).c_str(), "m4a") == 0 ||
				strcasecmp(dirContentsTemp[i].name.substr(dirContentsTemp[i].name.length()-3, 3).c_str(), "wav") == 0 ||
				strcasecmp(dirContentsTemp[i].name.substr(dirContentsTemp[i].name.length()-3, 3).c_str(), "ogg") == 0 ||
				dirContentsTemp[i].isDirectory)) {
				dirContents.push_back(dirContentsTemp[i]);
			}
		}
		dirChanged = false;
	}
	const u32 hDown = hidKeysDown();
	const u32 hHeld = hidKeysHeld();
	if (keyRepeatDelay)	keyRepeatDelay--;
	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			if(dirContents[selectedFile].name != currentSong) {
				currentSong = dirContents[selectedFile].name;
				playbackInfo_t playbackInfo;
				changeFile(dirContents[selectedFile].name.c_str(), &playbackInfo);
			}
			screenMode = musicPlayScreen;
			aptSetSleepAllowed(false);
			togglePlayback(); // Since it would otherwise pause it in main.cpp
		}
	} else if (hDown & KEY_B) {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
	} else if (hDown & KEY_Y) {
		nowPlayingList.push_back(dirContents[selectedFile].name);
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			keyRepeatDelay = 3;
		}
	}
	std::string dirs;
	for (uint i=(selectedFile<12) ? 0 : selectedFile-12;i<dirContents.size()&&i<((selectedFile<12) ? 13 : selectedFile+1);i++) {
		if (i == selectedFile) {
			dirs += "> " + dirContents[i].name + "\n";
		} else {
			dirs += "  " + dirContents[i].name + "\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<13) ? 13-dirContents.size() : 0);i++) {
		dirs += "\n";
	}
	if (dirContents[selectedFile].isDirectory)	dirs += "\n\uE000 : Open Folder   \uE001 : Back   \uE002 : Exit";
	else if(dirContents[selectedFile].name == currentSong)	dirs += "\n\uE000 : Show Player   \uE001 : Back   \uE002 : Exit   \uE003 : Add to Now Playing";
	else	dirs += "\n\uE000 : Play   \uE001 : Back   \uE002 : Exit   \uE003 : Add to Now Playing";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, dirs.c_str());

		if (settings.universal.theme == 0) {
		drawBgBot();
		drawBarsBotLight();
	} else if (settings.universal.theme == 1) {
		drawBgBot();
		drawBarsBotDark();
	} else if (settings.universal.theme == 2) {
		drawBgBot();
		drawBarsBotRed();
	}
	volt_end_draw();

}

void drawMusicPlay(void) {
	 // Theme Stuff.
	if (settings.universal.theme == 0) {
		drawBgTop();
		drawBarsTopLight();
	} else if (settings.universal.theme == 1) {
		drawBgTop();
		drawBarsTopDark();
	} else if (settings.universal.theme == 2) {
		drawBgTop();
		drawBarsTopRed();
	} 
	if(!isPaused() && isPlaying()) {
		std::string nowPlayingText = "Currently Playing: " + currentSong;
		volt_draw_text(0, 4, 0.72f, 0.72f, WHITE, nowPlayingText.c_str());
		volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE000 : Pause   \uE001 : Back   \uE002 : Stop song");
	} else if(isPaused() && isPlaying()) {
		volt_draw_text(0, 4, 0.72f, 0.72f, WHITE, "Currently Paused.");
		volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE000 : Play   \uE001 : Back   \uE002 : Stop song");
	} else {
		volt_draw_text(0, 4, 0.72f, 0.72f, WHITE, "Currently Stopped.");
		volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE001 : Back");
	}

		if (settings.universal.theme == 0) {
		drawBgBot();
		drawBarsBotLight();
	} else if (settings.universal.theme == 1) {
		drawBgBot();
		drawBarsBotDark();
	} else if (settings.universal.theme == 2) {
		drawBgBot();
		drawBarsBotRed();
	}
	volt_draw_texture(!isPaused() ? PauseIcon : PlayIcon, 140, 100);
	volt_end_draw();
}
