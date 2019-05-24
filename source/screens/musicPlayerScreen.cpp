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
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

#include "fileBrowse.h"
#include "keyboard.h"

extern "C" {
	#include "music/playback.h"
	#include "music/mp3.h"
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
int musicRepeat = 0;
bool musicShuffle = 0;

int musicPlayerReturn = musicMainScreen;
bool firstSong = true;
int locInPlaylist = 0;
uint selectedPlst = 0;
std::vector<DirEntry> plsts;
std::vector<Playlist> nowPlayingList;
bool decForRepeat2 = false;

uint selectedPlstItem = 0;
int movingPlstItem = -1;
std::vector<std::string> plstContents;

extern bool touching(touchPosition touch, ButtonPos button);

ButtonPos mainButtonPos[] = {
    {0, 40, 149, 52, musicListScreen},
    {170, 40, 149, 52, musicPlayerScreen},
    {0, 150, 149, 52, musicPlaylistPlayScreen},
	// {170, 150, 149, 52, /*playListEditorScreen*/},
};

ButtonPos playerButtonPos[] = {
	{130, 90, 60, 60, -1},
	{80, 100, 40, 40, -1},
	{200, 100, 40, 40, -1},
	{240, 175, 30, 30, -1},
	{280, 175, 30, 30, -1},
};

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

	// volt_draw_texture(MainMenuButton, mainButtonPos[3].x, mainButtonPos[3].y);
	// volt_draw_texture(PlaylistEditor, mainButtonPos[3].x+1, mainButtonPos[3].y+6);
	// volt_draw_text(210, 167, 0.7f, 0.7f, BLACK, "Plst Editor");

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
	if (dirContents[selectedFile].isDirectory)	dirs += "\n\uE000 : Open Folder   \uE001 : Back   \uE002 : Exit   \uE003 : Add to Playlist";
	else if(dirContents[selectedFile].name == currentSong)	dirs += "\n\uE000 : Show Player   \uE001 : Back   \uE002 : Exit   \uE003 : Add to Playlist";
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
				char path[PATH_MAX];
				getcwd(path, PATH_MAX);
				currentSong = path + dirContents[selectedFile].name;
				Playlist song;
				song.name = currentSong;
				song.position = nowPlayingList.size() + 1;
				nowPlayingList.push_back(song);
				playbackInfo_t playbackInfo;
				changeFile(dirContents[selectedFile].name.c_str(), &playbackInfo);
			}
			screenMode = musicPlayerScreen;
			musicPlayerReturn = musicListScreen;
			togglePlayback(); // Since it would otherwise pause it in main.cpp
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			screenMode = musicMainScreen;
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
	} else if (hDown & KEY_Y) {
		dirChanged = true;
		screenMode = musicPlaylistAddScreen;
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

	if(isPlaying()) {
		std::string nowPlayingText = "Current Song: " + currentSong.substr(currentSong.find_last_of("/")+1);
		volt_draw_text(0, 4, 0.52f, 0.52f, WHITE, nowPlayingText.c_str());
		volt_draw_text(20, 177, 0.45f, 0.45f, WHITE, (secondsToString(Audio_GetPosition()/Audio_GetRate()) + " / " + secondsToString(Audio_GetLength()/Audio_GetRate())).c_str());
		volt_draw_rectangle(18, 192, 364, 20, BLACK);
		volt_draw_rectangle(20, 194, ((float)Audio_GetPosition()/Audio_GetLength())*360, 16, settings.universal.bars);
		volt_draw_texture(Cover, 40, 25);
	}
	
	if(!isPaused() && isPlaying()) {
		volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE000 : Pause   \uE001 : Back   \uE002 : Stop song");
	} else if(isPaused() && isPlaying()) {
		volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE000 : Play   \uE001 : Back   \uE002 : Stop song");
	} else {
		volt_draw_text(0, 4, 0.72f, 0.72f, WHITE, "No song selected.");
		volt_draw_text(26, 221, 0.45f, 0.45f, WHITE, "\uE001 : Back");
	}

	drawBgBot();
	drawBarsBot();
	volt_draw_texture(!isPaused() ? PauseIcon : PlayIcon, playerButtonPos[0].x, playerButtonPos[0].y);
	volt_draw_texture(LeftIcon, playerButtonPos[1].x, playerButtonPos[1].y);
	volt_draw_texture(RightIcon, playerButtonPos[2].x, playerButtonPos[2].y);
	volt_draw_texture_blend(ShuffleIcon, playerButtonPos[3].x, playerButtonPos[3].y, (musicShuffle ? WHITE : settings.universal.bars));
	volt_draw_texture_blend(RepeatIcon, playerButtonPos[4].x, playerButtonPos[4].y, (musicRepeat ? WHITE : settings.universal.bars));
	if (musicRepeat)	volt_draw_text(playerButtonPos[4].x+11, playerButtonPos[4].y+9, 0.5f, 0.5f, WHITE, (musicRepeat == 1 ? "A" : "S"));
	volt_end_draw();
}

bool playlistSortPredicate(const Playlist &lhs, const Playlist &rhs) {
	if(lhs.position < rhs.position)	return true;
	else	return false;
}

bool playlistShufflePredicate(const Playlist &lhs, const Playlist &rhs) {
	return rand() % 2;
}
void musicPlayerLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_A) {
		togglePlayback();
	} else if (hDown & KEY_X) {
		stopPlayback();
	} else if (hDown & KEY_B) {
		screenMode = musicPlayerReturn;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, playerButtonPos[0])) {
			togglePlayback();
		} else if (touching(touch, playerButtonPos[1])) {
			goto prevSong;
		} else if (touching(touch, playerButtonPos[2])) {
			goto nextSong;
		} else if (touching(touch, playerButtonPos[3])) {
			goto toggleShuffle;
		} else if (touching(touch, playerButtonPos[4])) {
			goto toggleRepeat;
		}
	} else if (hDown & KEY_LEFT) {
		prevSong:
		if(locInPlaylist > 0) {
			locInPlaylist--;
		} else if (nowPlayingList.size() > 0) {
			locInPlaylist = nowPlayingList.size() - 1;
		}
		currentSong = nowPlayingList[locInPlaylist].name;
		playbackInfo_t playbackInfo;
		changeFile(currentSong.c_str(), &playbackInfo);
	} else if (hDown & KEY_RIGHT) {
		nextSong:
		if(locInPlaylist < (int)nowPlayingList.size()-1) {
			locInPlaylist++;
		} else {
			locInPlaylist = 0;
		}
		currentSong = nowPlayingList[locInPlaylist].name;
		playbackInfo_t playbackInfo;
		changeFile(currentSong.c_str(), &playbackInfo);
	} else if (hDown & KEY_START) {
		toggleRepeat:
		if (musicRepeat < 2 ) {
			musicRepeat++;
		} else {
			musicRepeat = 0;
		}
	} else if (hDown & KEY_SELECT) {
		toggleShuffle:
		musicShuffle = !musicShuffle;
		std::sort(nowPlayingList.begin(), nowPlayingList.end(), musicShuffle ?  playlistShufflePredicate : playlistSortPredicate);
	}
	aptSetSleepAllowed(false);
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
			plstList += "> " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n";
		} else {
			plstList += "  " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n";
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
		std::vector<DirEntry> selectedFiles;
		if(dirContents[selectedFile].isDirectory) {
			char startPath[PATH_MAX];
			getcwd(startPath, PATH_MAX);
			chdir(dirContents[selectedFile].name.c_str());
			getDirectoryContents(selectedFiles);
			chdir(startPath);
		} else {
			DirEntry dirEntry;
			dirEntry.name = "";
			selectedFiles.push_back(dirEntry);
		}
		for(uint i=0;i<selectedFiles.size();i++) {
			if(selectedPlst == 0) {
				Playlist playlist;
				char path[PATH_MAX];
				getcwd(path, PATH_MAX);
				playlist.name = path + dirContents[selectedFile].name + "/" + selectedFiles[i].name;
				playlist.position = nowPlayingList.size() + 1;
				nowPlayingList.push_back(playlist);
			} else {
				char path[PATH_MAX];
				getcwd(path, PATH_MAX);
				FILE* plst = fopen(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str(), "a");
				fputs((path+dirContents[selectedFile].name+(dirContents[selectedFile].isDirectory?"/"+selectedFiles[i].name:"")+"\n").c_str(), plst);
				fclose(plst);
			}
		}
		screenMode = musicListScreen;
	} else if (hDown & KEY_B) {
		screenMode = musicListScreen;
	} else if (hDown & KEY_Y) {
		std::string newPlaylist = keyboardInput("Enter new playlist name:");
		if(newPlaylist != "") {
			FILE* plst = fopen(("sdmc:/Universal-Manager/playlists/"+newPlaylist+".plst").c_str(), "w");
			fclose(plst);
			dirChanged = true;
		}
	} else if (hDown & KEY_X) {
		if (selectedPlst != 0) {
			if(confirmPopup("Are you sure you want to delete this playlist?")) {
				remove(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str());
				dirChanged = true;
			}
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
			plstList += "> " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n";
		} else {
			plstList += "  " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n";
		}
	}
	for (uint i=0;i<((plsts.size()<13) ? 13-plsts.size() : 0);i++) {
		plstList += "\n";
	}
	plstList += "\n\uE000 : Play   \uE001 : Back   \uE002 : Delete   \uE003 : Edit";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, plstList.c_str());

	drawBgBot();
	drawBarsBot();
	volt_end_draw();
}

void musicPlaylistPlayLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	if(hDown & KEY_A) {
		if(confirmPopup("Would you like to add to these songs to",
						 "Now Playing or play them now?",
						 "Play Now",
						 "Add to Now Playing",
						 117)) {
			nowPlayingList.clear();
		}
		std::ifstream plst("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name);
		if(plst) {
			Playlist song;
			int i = nowPlayingList.size() + 1;
			while(std::getline(plst, song.name)) {
				song.position = i++;
				nowPlayingList.push_back(song);
			}
		}
	} else if (hDown & KEY_B) {
		screenMode = musicMainScreen;
	} else if (hDown & KEY_X) {
		if(confirmPopup("Are you sure you want to delete this playlist?")) {
			remove(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str());
			dirChanged = true;
		}
	} else if (hDown & KEY_Y) {
		std::string temp;
		std::ifstream file("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name);
		plstContents.clear();
		while(std::getline(file, temp)) {
			plstContents.push_back(temp);
		}
		selectedPlstItem = 0;
		screenMode = musicPlaylistEditScreen;
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

void drawMusicPlaylistEdit() {
	drawBgTop();
	drawBarsTop();
	volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Music Playlist Menu");

	std::string plstList;
	for (uint i=(selectedPlstItem<12) ? 0 : selectedPlstItem-12;i<plstContents.size()&&i<((selectedPlstItem<12) ? 13 : selectedPlstItem+1);i++) {
		if (i == selectedPlstItem) {
			plstList += "> " + plstContents[i].substr(plstContents[i].find_last_of("/")+1) + "\n";
		} else if ((int)i == movingPlstItem) {
			plstList += "x " + plstContents[i].substr(plstContents[i].find_last_of("/")+1) + "\n";
		} else {
			plstList += "  " + plstContents[i].substr(plstContents[i].find_last_of("/")+1) + "\n";
		}
	}
	for (uint i=0;i<((plstContents.size()<13) ? 13-plstContents.size() : 0);i++) {
		plstList += "\n";
	}
	plstList += "\n\uE000 : Save   \uE001 : Back   \uE002 : Delete   \uE003 : Move";
	volt_draw_text(26, 32, 0.45f, 0.45f, WHITE, plstList.c_str());

	drawBgBot();
	drawBarsBot();
	volt_end_draw();
}

void musicPlaylistEditLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	if(hDown & KEY_A) {
		FILE* plst = fopen(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str(), "w");
		for(uint i=0;i<plstContents.size();i++) {
			fputs((plstContents[i]+"\n").c_str(), plst);
		}
		fclose(plst);
		screenMode = musicPlaylistPlayScreen;
	} else if(hDown & KEY_B) {
		screenMode = musicPlaylistPlayScreen;
	} else if (hDown & KEY_X) {
		if(confirmPopup("Are you sure you want to remove this song?")) {
			plstContents.erase(plstContents.begin()+selectedPlstItem);
			dirChanged = true;
		}
	} else if (hDown & KEY_Y) {
		if(movingPlstItem == -1) {
			movingPlstItem = selectedPlstItem;
		} else {
			std::string item = plstContents[movingPlstItem];
			plstContents.erase(plstContents.begin()+movingPlstItem);
			plstContents.insert(plstContents.begin()+selectedPlstItem, item);
			movingPlstItem = -1;
		}
	} else if (hHeld & KEY_UP) {
		if (selectedPlstItem > 0 && !keyRepeatDelay) {
			selectedPlstItem--;
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlstItem < plstContents.size()-1) {
			selectedPlstItem++;
			keyRepeatDelay = 3;
		}
	}
}
