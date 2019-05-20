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
#include <fstream>
#include <unistd.h>
#include <vector>

#include "fileBrowse.h"
#include "keyboard.h"

extern "C" {
	#include "music/playback.h"
}

struct ButtonPos {
    int x;
    int y;
    int w;
    int h;
	int link;
};

uint selectedFile = 0;
int keyRepeatDelay = 3;
bool dirChanged = true;
std::vector<DirEntry> dirContents;
std::string scanDir;
std::string currentSong = "";

int musicPlayerReturn = musicMainScreen;
uint selectedPlst = 0;
std::vector<DirEntry> plsts;
std::vector<std::string> nowPlayingList;

extern bool touching(touchPosition touch, ButtonPos button);

ButtonPos mainButtonPos[] = {
    {0, 40, 149, 52, musicListScreen},
    {170, 40, 149, 52, musicPlayerScreen},
    {0, 150, 149, 52, musicPlaylistPlayScreen},
};

ButtonPos playerButtonPos = {130, 90, 60, 60, -1};

void drawMusicMain() {
	// Theme Stuff.
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Music Player Menu");
	drawBgBot();
	drawBarsBot();

	volt_draw_texture(MainMenuButton, mainButtonPos[0].x, mainButtonPos[0].y);
	volt_draw_texture(MusicIcon, mainButtonPos[0].x+5, mainButtonPos[0].y+10);
	volt_draw_text(40, 57, 0.7f, 0.7f, BLACK, "Songs");

	volt_draw_texture(MainMenuButton, mainButtonPos[1].x, mainButtonPos[1].y);
	volt_draw_texture(PlayIconSmall, mainButtonPos[1].x+5, mainButtonPos[1].y+10);
	volt_draw_text(210, 57, 0.7f, 0.7f, BLACK, "Now playing");

	volt_draw_texture(MainMenuButton, mainButtonPos[2].x, mainButtonPos[2].y);
	volt_draw_texture(PlaylistIcon, mainButtonPos[2].x+1, mainButtonPos[2].y+6);
	volt_draw_text(40, 167, 0.7f, 0.7f, BLACK, "Playlists");

	volt_end_draw();
}

void musicMainLogic(u32 hDown, touchPosition touch) {
	if(hDown & KEY_B) {
		screenMode = fileScreen;
	} else if(hDown & KEY_TOUCH) {
		for(uint i=0;i<(sizeof(mainButtonPos)/sizeof(mainButtonPos[0]));i++) {
			if (touching(touch, mainButtonPos[i])) {
				screenMode = mainButtonPos[i].link;
				if(mainButtonPos[i].link == musicPlayerScreen) {
					musicPlayerReturn = musicMainScreen;
				} else if(mainButtonPos[i].link == musicPlaylistPlayScreen) {
					selectedPlst = 0;
					dirChanged = true;
				}
			}
		}
	}
}

void drawMusicList(void) {
	// Theme Stuff.
	drawBgTop();
	drawBarsTop();
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
	else	dirs += "\n\uE000 : Play   \uE001 : Back   \uE002 : Exit   \uE003 : Add to Playlist";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, dirs.c_str());

	drawBgBot();
	drawBarsBot();
	volt_end_draw();
}

void musicListLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;
	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			if(dirContents[selectedFile].name != currentSong) {
				nowPlayingList.clear();
				currentSong = dirContents[selectedFile].name;
				playbackInfo_t playbackInfo;
				changeFile(dirContents[selectedFile].name.c_str(), &playbackInfo);
			}
			screenMode = musicPlayerScreen;
			musicPlayerReturn = musicListScreen;
			aptSetSleepAllowed(false);
			togglePlayback(); // Since it would otherwise pause it in main.cpp
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0) {
			screenMode = musicMainScreen;
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
	} else if (hDown & KEY_Y && !dirContents[selectedFile].isDirectory) {
		dirChanged = true;
		screenMode = musicPlaylistAddScreen;
		keyRepeatDelay = 2;
	} else if (hDown & KEY_X) {
		screenMode = musicMainScreen;
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
}

void drawMusicPlayer(void) {
	drawBgTop();
	drawBarsTop();
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

	drawBgBot();
	drawBarsBot();
	volt_draw_texture(!isPaused() ? PauseIcon : PlayIcon, playerButtonPos.x, playerButtonPos.y);
	volt_end_draw();
}

void musicPlayerLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_A) {
		togglePlayback();
	} else if (hDown & KEY_X) {
		stopPlayback();
	} else if (hDown & KEY_B) {
		screenMode = musicPlayerReturn;
	} else if (hDown & KEY_TOUCH) {
		if(touching(touch, playerButtonPos)) {
			togglePlayback();
		}
	}
}

void drawMusicPlaylistAdd(void) {
	// Theme Stuff.
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Music Playlist Menu");
	mkdir("sdmc:/Universal-Manager/playlists/", 0777);
	
	if(dirChanged) {
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/playlists/");
		getDirectoryContents(plsts);
		chdir(startPath);
		DirEntry dirEntry;
		dirEntry.name = "Now Playing";
		dirEntry.isDirectory = false;
		plsts.insert(plsts.begin(), dirEntry);
		dirChanged = false;
	}

	std::string plstList;
	for (uint i=(selectedPlst<12) ? 0 : selectedPlst-12;i<plsts.size()&&i<((selectedPlst<12) ? 13 : selectedPlst+1);i++) {
		if (i == selectedPlst) {
			plstList += "> " + plsts[i].name + "\n";
		} else {
			plstList += "  " + plsts[i].name + "\n";
		}
	}
	for (uint i=0;i<((plsts.size()<13) ? 13-plsts.size() : 0);i++) {
		plstList += "\n";
	}
	plstList += "\n\uE000 : Add to "+plsts[selectedPlst].name+"   \uE001 : Back   \uE002 : Delete   \uE003 : New";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, plstList.c_str());

	drawBgBot();
	drawBarsBot();
	volt_end_draw();
}

void musicPlaylistAddLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	if(hDown & KEY_A) {
		if(selectedPlst == 0) {
		nowPlayingList.push_back(dirContents[selectedFile].name);
		} else {
			char path[PATH_MAX];
			getcwd(path, PATH_MAX);
			FILE* plst = fopen(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str(), "a");
			fputs((path+dirContents[selectedFile].name+"\n").c_str(), plst);
			fclose(plst);
		}
		screenMode = musicListScreen;
	} else if (hDown & KEY_B) {
		screenMode = musicListScreen;
	} else if (hDown & KEY_Y && !keyRepeatDelay) { // !keyRepeat delay so it doesn't instantly make a new plst
		std::string newPlaylist = keyboardInput("Enter new playlist name:");
		FILE* plst = fopen(("sdmc:/Universal-Manager/playlists/"+newPlaylist+".plst").c_str(), "w");
		fclose(plst);
		dirChanged = true;
	} else if (hDown & KEY_X) {
		if (selectedPlst != 0) {
		remove(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str());
		dirChanged = true;
		}
	} else if (hHeld & KEY_UP) {
		if (selectedPlst > 0 && !keyRepeatDelay) {
			selectedPlst--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlst < plsts.size()-1) {
			selectedPlst++;
			keyRepeatDelay = 3;
		}
	}
}

void drawMusicPlaylistPlay(void) {
	// Theme Stuff.
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Music Playlist Menu");
	mkdir("sdmc:/Universal-Manager/playlists/", 0777);
	
	if(dirChanged) {
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/playlists/");
		getDirectoryContents(plsts);
		chdir(startPath);
	}

	std::string plstList;
	for (uint i=(selectedPlst<12) ? 0 : selectedPlst-12;i<plsts.size()&&i<((selectedPlst<12) ? 13 : selectedPlst+1);i++) {
		if (i == selectedPlst) {
			plstList += "> " + plsts[i].name + "\n";
		} else {
			plstList += "  " + plsts[i].name + "\n";
		}
	}
	for (uint i=0;i<((plsts.size()<13) ? 13-plsts.size() : 0);i++) {
		plstList += "\n";
	}
	plstList += "\n\uE000 : Add "+plsts[selectedPlst].name+" to Now Playing   \uE001 : Back   \uE002 : Delete";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, plstList.c_str());

	drawBgBot();
	drawBarsBot();
	volt_end_draw();
}

void musicPlaylistPlayLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	if(hDown & KEY_A) {
		std::ifstream plst("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name);
		if(plst) {
			std::string song;
			while(std::getline(plst, song)) {
				nowPlayingList.push_back(song);
			}
		}
	} else if (hDown & KEY_B) {
		screenMode = musicMainScreen;
	} else if (hDown & KEY_X) {
		remove(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str());
		dirChanged = true;
	} else if (hHeld & KEY_UP) {
		if (selectedPlst > 0 && !keyRepeatDelay) {
			selectedPlst--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlst < plsts.size()-1) {
			selectedPlst++;
			keyRepeatDelay = 3;
		}
	}
}
