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

void drawMusicPlayerUI(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	volt_draw_rectangle(0, 0, 400, 240, GRAY);
	volt_draw_rectangle(0, 0, 400, 25, DARK_BLUE);
	volt_draw_rectangle(0, 215, 400, 25, DARK_BLUE);
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Music Player Menu");

	if (dirChanged) {
		getDirectoryContents(dirContents);
		for(uint i=0;i<dirContents.size();i++) {
			if (!(strcasecmp(dirContents[i].name.substr(dirContents[i].name.length()-3, 3).c_str(), "mp3") == 0 ||
				strcasecmp(dirContents[i].name.substr(dirContents[i].name.length()-3, 3).c_str(), "m4a") == 0 || // I dunno, if it can play this file yet?
				strcasecmp(dirContents[i].name.substr(dirContents[i].name.length()-3, 3).c_str(), "wav") == 0 ||
				strcasecmp(dirContents[i].name.substr(dirContents[i].name.length()-3, 3).c_str(), "ogg") == 0 ||
				dirContents[i].isDirectory)) {
				dirContents.erase(dirContents.begin()+i);
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
		} else if (isPlaying()) {
			currentSong = "";
			stopPlayback();
		} else {
			currentSong = dirContents[selectedFile].name;
			playbackInfo_t playbackInfo;
			changeFile(dirContents[selectedFile].name.c_str(), &playbackInfo);
			screenMode = musicPlayScreen;
		}
	} else if (hDown & KEY_B) {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
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
	if (dirContents[selectedFile].isDirectory)	dirs += "\nA: Open Folder   B: Back   X: Exit";
	else if (isPlaying())	dirs += "\nA: Stop Playing   B: Back   X: Exit";
	else	dirs += "\nA: Play   B: Back   X: Exit";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, dirs.c_str());

	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
	volt_draw_rectangle(0, 0, 320, 240, GRAY);
	volt_draw_rectangle(0, 0, 320, 25, DARK_BLUE);
	volt_draw_rectangle(0, 215, 320, 25, DARK_BLUE);
	volt_end_draw();

}

void drawMusicPlay(void) {
	volt_draw_on(GFX_TOP, GFX_LEFT);
	volt_draw_rectangle(0, 0, 400, 240, GRAY);
	volt_draw_rectangle(0, 0, 400, 25, BLACK);
	volt_draw_rectangle(0, 215, 400, 25, BLACK);
	if(!isPaused()) {
		std::string nowPlayingText = "Currently Playing: " + currentSong;
		volt_draw_text(0, 4, 0.72f, 0.72f, WHITE, nowPlayingText.c_str());
	} else {
		volt_draw_text(0, 4, 0.72f, 0.72f, WHITE, "Currently Paused.");
	}

	volt_draw_on(GFX_BOTTOM, GFX_LEFT);
	volt_draw_rectangle(0, 0, 320, 240, GRAY);
	volt_draw_rectangle(0, 0, 320, 25, BLACK);
	volt_draw_rectangle(0, 215, 320, 25, BLACK);
	volt_draw_texture(!isPaused() ? PauseIcon : PlayIcon, 140, 100);
	volt_end_draw();
}
