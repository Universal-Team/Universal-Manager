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

#include "screens/musicScreen.hpp"
#include "screens/screenCommon.hpp"
#include "utils/keyboard.hpp"
#include "utils/settings.hpp"
#include "utils/sound.h"

#include <algorithm>
#include <fstream>
#include <unistd.h>

extern "C" {
	#include "C2D_helper.h"
	#include "music/error.h"
	#include "music/playback.h"
}

bool decForRepeat2 = false;
uint selectedPlstItem = 0;
int movingPlstItem = -1;

bool firstSong = true;
int locInPlaylist = 0;
int musicRepeat = 0;
bool musicShuffle = 0;
std::vector<Playlist> nowPlayingList;
std::string currentSong = "";
extern bool MusicPlays;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Music::Draw(void) const
{
	if (MusicMode == 0) {
		DrawMusicMain();
	} else if (MusicMode == 1) {
		DrawMusicList();
	} else if (MusicMode == 2) {
		DrawPlayer();
	} else if (MusicMode == 3) {
		DrawPlaylistPlay();
	} else if (MusicMode == 4) {
		DrawThemeSelector();
	} else if (MusicMode == 5) {
		DrawPlaylistAdd();
	} else if (MusicMode == 6) {
		DrawPlaylistEdit();
	}
}

void Music::Logic(u32 hDown, u32 hHeld, touchPosition touch)
{
	if (MusicMode == 0) {
		MusicMainLogic(hDown, hHeld, touch);
	} else if (MusicMode == 1) {
		MusicListLogic(hDown, hHeld);
	} else if (MusicMode == 2) {
		PlayerLogic(hDown, touch);
	} else if (MusicMode == 3) {
		PlaylistPlayLogic(hDown, hHeld);
	} else if (MusicMode == 4) {
		ThemeSelectorLogic(hDown, hHeld);
	} else if (MusicMode == 5) {
		PlaylistAddLogic(hDown, hHeld, touch);
	} else if (MusicMode == 6) {
		PlaylistEditLogic(hDown, hHeld);
	}
}



void Music::DrawMusicMain(void) const
{
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Music Player Menu"))/2, 0, 0.72f, WHITE, "Music Player Menu");
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();

	Gui::sprite(3, button_button_tl_idx, Functions[0].x, Functions[0].y);
	Gui::sprite(0, sprites_music_icon_idx, Functions[0].x+5, Functions[0].y+15);
	Gui::DrawString(40, 60, 0.65f, WHITE, "Songs");

	Gui::sprite(3, button_button_2_idx, Functions[1].x, Functions[1].y);
	Gui::Draw_ImageBlend(0, sprites_play_icon_small_glow_idx, Functions[1].x+5, Functions[1].y+15, Config::barColor);
	Gui::sprite(0, sprites_play_icon_small_normal_idx, Functions[1].x+5, Functions[1].y+15);
	Gui::DrawString(210, 60, 0.65f, WHITE, "Now Playing");

	Gui::sprite(3, button_button_3_idx, Functions[2].x, Functions[2].y);
	Gui::sprite(0, sprites_playlist_icon_idx, Functions[2].x+1, Functions[2].y+12);
	Gui::DrawString(37, 173, 0.65f, WHITE, "Playlists");

	Gui::sprite(3, button_button_br_idx, Functions[3].x, Functions[3].y);
	Gui::sprite(0, sprites_themes_idx, Functions[3].x+5, Functions[3].y+15);
	Gui::DrawString(220, 173, 0.65f, WHITE, "Themes");

	if (selection == 0) {
		Gui::drawGUISelector(button_tl_selector_idx, 0, 39, .020f);
	} else if (selection == 1) {
		Gui::drawGUISelector(button_selector_idx, 166, 44, .020f);
	} else if (selection == 2) {
		Gui::drawGUISelector(button_selector_idx, 3, 154, .020f);
	} else if (selection == 3) {
		Gui::drawGUISelector(button_br_selector_idx, 161, 149, .020f);
	}
}


void Music::MusicMainLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(hDown & KEY_UP) {
		if(selection > 1)	selection -= 2;
	} else if(hDown & KEY_DOWN) {
		if(selection < 3 && selection != 2 && selection != 3)	selection += 2;
	} else if (hDown & KEY_LEFT) {
		if (selection%2) selection--;
	} else if (hDown & KEY_RIGHT) {
		if (!(selection%2)) selection++;
	} else if(hDown & KEY_A) {
		switch(selection) {
			case 0:
				MusicMode = 1;
				break;
			case 1:
				if (MusicPlays == true) {
					MusicMode = 2;
				}
				break;
			case 2:
				MusicMode = 3;
				selectedPlst = 0;
				dirChanged = true;
				break;
			case 3:
				MusicMode = 4;
				break;
		}
	} else if(hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if(hDown & KEY_TOUCH) {
			if (touching(touch, Functions[0])) {
				MusicMode = 1;
			} else if (touching(touch, Functions[1])) {
				if (MusicPlays == true) {
					MusicMode = 2;
				}
			} else if (touching(touch, Functions[2])) {
				MusicMode = 3;
				selectedPlst = 0;
				dirChanged = true;
			} else if (touching(touch, Functions[3])) {
				MusicMode = 4;
			}
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press Songs to open the Song File Browse. \n \n Press Now Playing to open the Music Player. \n \n Press Playlists to open the Playlist Menu. \n \n Press Themes to Select an Image for the Music Player. \n \n (You need the BG Mode in the Settings!)");
	}
}


// List.

void Music::DrawMusicList(void) const
{
	Gui::DrawFileBrowseBG();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	Gui::DrawString((400-(Gui::GetStringWidth(0.60f, path)))/2, 218, 0.60f, WHITE, path);
	std::string dirs;
	for (uint i=(selectedFile<5) ? 0 : selectedFile-5;i<dirContents.size()&&i<((selectedFile<5) ? 6 : selectedFile+1);i++) {
		if (i == selectedFile) {
			dirs +=  "> " + dirContents[i].name + "\n\n";
		} else {
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

		Gui::DrawString(26, 32, 0.53f, BLACK, dirs.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void Music::MusicListLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (dirChanged) {
				dirContents.clear();
				std::vector<DirEntry> dirContentsTemp;
				getDirectoryContents(dirContentsTemp, {"mp3", "ogg", "wav"});
				for(uint i=0;i<dirContentsTemp.size();i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}
		dirChanged = false;
	}


	if (hDown & KEY_A) {
		if (dirContents.size() == 0) {
			DisplayTimeMessage("What are you trying to do? :P");
		} else {
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
				togglePlayback(); // Since it would otherwise pause it.
			}
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			MusicMode = 0;
		} else {
			chdir("..");
			selectedFile = 0;
			dirChanged = true;
		}
	} else if (hDown & KEY_Y) {
		MusicMode = 5;
		dirChanged = true; // Not anymore!
	} else if (hDown & KEY_X) {
		MusicMode = 0;
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_START) {
		if (MusicPlays == true) {
			MusicMode = 2;
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE000 to Play the selected Song. \n \n Press \uE001 to go back a Folder. \n \n Press \uE002 to exit to the Music Player Menu. \n \n Press \uE003 to open the Playlist Menu. \n \n Press Start to open the Music Player.");
	}
}


// Player.

bool Music::Draw_ThemeImage(void) const
{
	return C2D_DrawImageAt(musicImage, 0, 25, 0.5);
}

bool Music::Draw_CoverImage(void) const
{
	return C2D_DrawImageAt(coverImage, 230, 25, 0.5);
}

std::string Music::secondsToString(u64 seconds) const
{
	int h = 0, m = 0, s = 0;
	h = (seconds / 3600);
	m = (seconds - (3600 * h)) / 60;
	s = (seconds - (3600 * h) - (m * 60));

	char string[35];

	if (h > 0) snprintf(string, sizeof(string), "%02d:%02d:%02d", h, m, s);
	else	snprintf(string, sizeof(string), "%02d:%02d", m, s);

	return string;
}

void Music::DrawPlayer(void) const
{

	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();

		// Placeholder for Metadata Stuff..
	 	if (Config::musicMode == 0) {
	} else if (Config::musicMode == 1) {

			if ((metadata.has_meta) && (metadata.title[0] != '\0') && (metadata.artist[0] != '\0') && (metadata.album[0] != '\0') && (metadata.year[0] != '\0')) {
			Gui::DrawString(15, 40, 0.5f, WHITE, strupr(metadata.title));
			Gui::DrawString(15, 60, 0.45f, WHITE, strupr(metadata.artist));
			Gui::DrawString(15, 80, 0.45f, WHITE, strupr(metadata.album));
			Gui::DrawString(15, 100, 0.45f, WHITE, strupr(metadata.year));
		} else {
			Gui::DrawString(15, 40, 0.5f, WHITE, "No Metadata Found.");
		}

		if (coverImageLoaded == true) {
			Draw_CoverImage();
		} else if (coverImageLoaded == false) {
			Gui::sprite(0, sprites_cover_glow_idx, 250, 30);
			Gui::sprite(0, sprites_cover_normal_idx, 250, 30);
		}

	} else if (Config::musicMode == 2) {
		if (themeImageLoaded == true) {
		Draw_ThemeImage();
	} else if (themeImageLoaded == false) {
			Gui::DrawString(15, 40, 0.5f, WHITE, "Please load an Image under `Themes`.");
	}
	}

	if(isPlaying()) {

		// Current Song.
		std::string nowPlayingText = "Current Song: " + currentSong.substr(currentSong.find_last_of("/")+1);
		Gui::DrawString(0, 0, 0.45f, WHITE, nowPlayingText.c_str());

		// Progressbar - Time.
		Draw_Rect(100, 179, 207, 10, GRAY);
		Gui::DrawString(110, 177, 0.45f, WHITE, (secondsToString(Audio_GetPosition()/Audio_GetRate()) + "                            " + secondsToString(Audio_GetLength()/Audio_GetRate())).c_str());
		// Progressbar - Display.
		Draw_Rect(120, 194, 150, 12, GRAY);
		Draw_Rect(120, 194, (((double)Audio_GetPosition()/(double)Audio_GetLength()) * 150.0), 12, Config::barColor);
	}


	if(!isPaused() && isPlaying()) {
		Gui::DrawString(80, 220, 0.45f, WHITE, "A : Pause   B : Back   X : Stop song");
	} else if(isPaused() && isPlaying()) {
		Gui::DrawString(80, 220, 0.45f, WHITE, "A : Play   B : Back   X : Stop song");
	} else {
		Gui::DrawString(0, 0, 0.72f, WHITE, "No Song Selected.");
		Gui::DrawString(80, 220, 0.45f, WHITE, "B : Back");
	}


	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
	DisplayTime();
	drawBatteryBot();

	Gui::Draw_ImageBlend(0, !isPaused() ? sprites_pause_icon_glow_idx : sprites_play_icon_glow_idx, Functions[4].x, Functions[4].y, Config::barColor);
	Gui::sprite(0, !isPaused() ? sprites_pause_icon_normal_idx : sprites_play_icon_normal_idx, Functions[4].x, Functions[4].y);

	Gui::Draw_ImageBlend(0, sprites_left_arrow_glow_idx, Functions[5].x, Functions[5].y, Config::barColor);
	Gui::sprite(0, sprites_left_arrow_normal_idx, Functions[5].x, Functions[5].y);

	Gui::Draw_ImageBlend(0, sprites_right_icon_glow_idx, Functions[6].x, Functions[6].y, Config::barColor);
	Gui::sprite(0, sprites_right_icon_normal_idx, Functions[6].x, Functions[6].y);

	Gui::Draw_ImageBlend(0, sprites_shuffle_icon_idx, Functions[7].x, Functions[7].y, (musicShuffle ? WHITE : Config::barColor));
	Gui::Draw_ImageBlend(0, sprites_repeat_icon_idx, Functions[8].x, Functions[8].y, (musicRepeat ? WHITE : Config::barColor));
	if (musicRepeat)	Gui::DrawString(Functions[8].x+11, Functions[8].y+9, 0.5f, BLACK, (musicRepeat == 1 ? "A" : "S"));
}

bool playlistSortPredicate(const Playlist &lhs, const Playlist &rhs) {
	if(lhs.position < rhs.position)	return true;
	else	return false;
}

bool playlistShufflePredicate(const Playlist &lhs, const Playlist &rhs) {
	return rand() % 2;
}

void Music::PlayerLogic(u32 hDown, touchPosition touch) {
	if (hDown & KEY_A) {
		togglePlayback();
	} else if (hDown & KEY_X) {
		stopPlayback();
	} else if (hDown & KEY_B) {
		MusicMode = 0;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, Functions[4])) {
			togglePlayback();
		} else if (touching(touch, Functions[5])) {
			goto prevSong;
		} else if (touching(touch, Functions[6])) {
			goto nextSong;
		} else if (touching(touch, Functions[7])) {
			goto toggleShuffle;
		} else if (touching(touch, Functions[8])) {
			goto toggleRepeat;
		}
	} else if (hDown & KEY_LEFT) {
		prevSong:
		if (isPlaying()) {
			if(locInPlaylist > 0) {
				locInPlaylist--;
			} else if (nowPlayingList.size() > 0) {
				locInPlaylist = nowPlayingList.size() - 1;
			}
			currentSong = nowPlayingList[locInPlaylist].name;
			playbackInfo_t playbackInfo;
			changeFile(currentSong.c_str(), &playbackInfo);
		}
	} else if (hDown & KEY_RIGHT) {
		nextSong:
		if (isPlaying()) {
			if(locInPlaylist < (int)nowPlayingList.size()-1) {
				locInPlaylist++;
			} else {
				locInPlaylist = 0;
			}
			currentSong = nowPlayingList[locInPlaylist].name;
			playbackInfo_t playbackInfo;
			changeFile(currentSong.c_str(), &playbackInfo);
		}
	} else if (hDown & KEY_START) {
		toggleRepeat:
		if (isPlaying()) {
			if (musicRepeat < 2 ) {
				musicRepeat++;
			} else {
				musicRepeat = 0;
			}
		}
	} else if (hDown & KEY_SELECT) {
		toggleShuffle:
		if (isPlaying()) {
			musicShuffle = !musicShuffle;
			std::sort(nowPlayingList.begin(), nowPlayingList.end(), musicShuffle ?  playlistShufflePredicate : playlistSortPredicate);
		}
	}
	aptSetSleepAllowed(false);
}


// Playlist Add.

void Music::DrawPlaylistAdd(void) const
{
	// Theme Stuff.
	Gui::DrawFileBrowseBG();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Music Playlist Menu"))/2, 0, 0.72f, WHITE, "Music Playlist Menu");
	mkdir("sdmc:/Universal-Manager/playlists/", 0777);


	std::string plstList;
	for (uint i=(selectedPlst<5) ? 0 : selectedPlst-5;i<plsts.size()&&i<((selectedPlst<5) ? 6 : selectedPlst+1);i++) {
		if (i == selectedPlst) {
			plstList += "> " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n\n";
		} else {
			plstList += "  " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n\n";
		}
	}
	for (uint i=0;i<((plsts.size()<6) ? 6-plsts.size() : 0);i++) {
		plstList += "\n\n";
	}
	Gui::DrawString(26, 32, 0.53f, BLACK, plstList.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void Music::PlaylistAddLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(keyRepeatDelay)	keyRepeatDelay--;

	if(dirChanged) {
		dirChanged = false;
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/playlists/");
		getDirectoryContents(plsts);
		chdir(startPath);
		DirEntry dirEntry;
		dirEntry.name = "Now Playing";
		dirEntry.isDirectory = false;
		plsts.insert(plsts.begin(), dirEntry);
	}


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
			MusicMode = 1;
	} else if (hDown & KEY_B) {
			MusicMode = 1;
	} else if (hDown & KEY_Y) {
		std::string newPlaylist = Input::getLine("Please type in the new Playlist's name.");
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
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlst < plsts.size()-1) {
			selectedPlst++;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE000 to Add this Song to the Selected Playlist. \n \n Press \uE001 to return to the Song List. \n \n Press \uE002 to delete a Playlist. \n \n Press \uE003 to create a Playlist.");
	}
}





void Music::DrawPlaylistEdit(void) const
{
	Gui::DrawFileBrowseBG();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Music Playlist Menu"))/2, 0, 0.72f, WHITE, "Music Playlist Menu");

	std::string plstList;
	for (uint i=(selectedPlstItem<5) ? 0 : selectedPlstItem-5;i<plstContents.size()&&i<((selectedPlstItem<5) ? 6 : selectedPlstItem+1);i++) {
		if (i == selectedPlstItem) {
			plstList += "> " + plstContents[i].substr(plstContents[i].find_last_of("/")+1) + "\n\n";
		} else if ((int)i == movingPlstItem) {
			plstList += "x " + plstContents[i].substr(plstContents[i].find_last_of("/")+1) + "\n\n";
		} else {
			plstList += "  " + plstContents[i].substr(plstContents[i].find_last_of("/")+1) + "\n\n";
		}
	}
	for (uint i=0;i<((plstContents.size()<6) ? 6-plstContents.size() : 0);i++) {
		plstList += "\n\n";
	}
	Gui::DrawString(26, 32, 0.53f, BLACK, plstList.c_str());
	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void Music::PlaylistEditLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;

	if(hDown & KEY_A) {
		FILE* plst = fopen(("sdmc:/Universal-Manager/playlists/"+plsts[selectedPlst].name).c_str(), "w");
		for(uint i=0;i<plstContents.size();i++) {
			fputs((plstContents[i]+"\n").c_str(), plst);
		}
		fclose(plst);
		MusicMode = 3;
	} else if(hDown & KEY_B) {
			MusicMode = 3;
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
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlstItem < plstContents.size()-1) {
			selectedPlstItem++;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE000 to Save The Playlist. \n \n Press \uE001 to return to the Playlist Screen. \n \n Press \uE002 to Delete a Song from the Playlist. \n \n Press \uE003 to move Songs.");
	}
}



void Music::DrawPlaylistPlay(void) const
{
	Gui::DrawFileBrowseBG();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.72f, "Music Playlist Menu"))/2, 0, 0.72f, WHITE, "Music Playlist Menu");
	mkdir("sdmc:/Universal-Manager/playlists/", 0777);



	std::string plstList;
	for (uint i=(selectedPlst<5) ? 0 : selectedPlst-5;i<plsts.size()&&i<((selectedPlst<5) ? 6 : selectedPlst+1);i++) {
		if (i == selectedPlst) {
			plstList += "> " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n\n";
		} else {
			plstList += "  " + plsts[i].name.substr(0, plsts[i].name.find_last_of(".")) + "\n\n";
		}
	}
	for (uint i=0;i<((plsts.size()<6) ? 6-plsts.size() : 0);i++) {
		plstList += "\n\n";
	}
	Gui::DrawString(26, 32, 0.53f, BLACK, plstList.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void Music::PlaylistPlayLogic(u32 hDown, u32 hHeld) {
	if(keyRepeatDelay)	keyRepeatDelay--;

	if(dirChanged) {
		dirChanged = false;
		char startPath[PATH_MAX];
		getcwd(startPath, PATH_MAX);
		chdir("sdmc:/Universal-Manager/playlists/");
		getDirectoryContents(plsts);
		chdir(startPath);
	}


	if(hDown & KEY_A) {
		if(confirmPopup("Would you like to add to these songs to",
						 "Now Playing or play them now?",
						 "Play Now",
						 "Add to Now Playing")) {
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
		MusicMode = 0;
		selection = 0;
		dirChanged = true;
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
		MusicMode = 6;
	} else if (hHeld & KEY_UP) {
		if (selectedPlst > 0 && !keyRepeatDelay) {
			selectedPlst--;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlst < plsts.size()-1) {
			selectedPlst++;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE000 to Play the selected Playlist. \n \n Press \uE001 to return to the Music Player Menu. \n \n Press \uE002 to delete a Playlist. \n \n Press \uE003 to edit a Playlist.");
	}
}





void Music::DrawThemeSelector(void) const
{
	Gui::DrawFileBrowseBG();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	char path[PATH_MAX];
	getcwd(path, PATH_MAX);
	Gui::DrawString((400-(Gui::GetStringWidth(0.60f, path)))/2, 218, 0.60f, WHITE, path);
	std::string dirs;
	for (uint i=(selectedFile<5) ? 0 : selectedFile-5;i<dirContents.size()&&i<((selectedFile<5) ? 6 : selectedFile+1);i++) {
		if (i == selectedFile) {
			dirs +=  "> " + dirContents[i].name + "\n\n";
		} else {
			dirs +=  dirContents[i].name + "\n\n";
		}
	}
	for (uint i=0;i<((dirContents.size()<6) ? 6-dirContents.size() : 0);i++) {
		dirs += "\n\n";
	}

		Gui::DrawString(26, 32, 0.53f, BLACK, dirs.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void Music::ThemeSelectorLogic(u32 hDown, u32 hHeld) {
	if (keyRepeatDelay)	keyRepeatDelay--;

			if (dirChanged) {
				dirContents.clear();
				std::vector<DirEntry> dirContentsTemp;
				getDirectoryContents(dirContentsTemp);
				for(uint i=0;i<dirContentsTemp.size();i++) {
				dirContents.push_back(dirContentsTemp[i]);
			}
		dirChanged = false;
	}


	if (hDown & KEY_A) {
		if (dirContents[selectedFile].isDirectory) {
			chdir(dirContents[selectedFile].name.c_str());
			selectedFile = 0;
			dirChanged = true;
		} else {
			if(dirContents[selectedFile].name != currentSong) {
			}
			if (Config::musicMode == 0) {
			DisplayMsg("Put the Music Mode to\n `BG` or `COVER`.");
			for (int i = 0; i < 60*4; i++) {
			gspWaitForVBlank();
			}
		} else if (Config::musicMode == 1) {
			if(confirmPopup("Do you want, to use this Image\nAs the Cover Image?")) {
			Draw_LoadImageFile(&coverImage, dirContents[selectedFile].name.c_str());
			coverImageLoaded = true;
			}
		} else if (Config::musicMode == 2) {
			if(confirmPopup("Do you want, to use this Image\nAs the Theme Image?")) {
			Draw_LoadImageFile(&musicImage, dirContents[selectedFile].name.c_str());
			themeImageLoaded = true;
			}
		}
		}
	} else if (hDown & KEY_B) {
		char path[PATH_MAX];
		getcwd(path, PATH_MAX);
		if(strcmp(path, "sdmc:/") == 0 || strcmp(path, "/") == 0) {
			MusicMode = 0;
		} else {
		chdir("..");
		selectedFile = 0;
		dirChanged = true;
		}
	} else if(hDown & KEY_X) {
			MusicMode = 0;
	} else if (hHeld & KEY_UP) {
		if (selectedFile > 0 && !keyRepeatDelay) {
			selectedFile--;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedFile < dirContents.size()-1) {
			selectedFile++;
			playScrollSfx();
			if (fastMode == true) {
				keyRepeatDelay = 3;
			} else if (fastMode == false){
				keyRepeatDelay = 6;
			}
		}
	} else if (hDown & KEY_R) {
		fastMode = true;
	} else if (hDown & KEY_L) {
		fastMode = false;
	} else if (hHeld & KEY_SELECT) {
		helperBox(" Press \uE000 to Select an Image. \n \n Press \uE001 to go back a Folder. \n \n Press \uE002 to exit to the Music Player Menu.");
	}
}