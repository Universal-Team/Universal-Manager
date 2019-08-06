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

#include "screenCommon.hpp"
#include "playlistAddScreen.hpp"
#include "settings.hpp"
#include "keyboard.hpp"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include "sound.h"

void PlaylistAdd::Draw(void) const
{
	// Theme Stuff.
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	Draw_Text((400-Draw_GetTextWidth(0.72f, "Music Playlist Menu"))/2, 0, 0.72f, WHITE, "Music Playlist Menu");
	mkdir("sdmc:/Universal-Manager/playlists/", 0777);


	std::string plstList;
	std::string plstList2;
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
	plstList2 += "\n\uE000 : Add to "+plsts[selectedPlst].name+"";
	Draw_Text(26, 32, 0.53f, WHITE, plstList.c_str());
	Draw_Text(26, 210, 0.45f, WHITE, plstList2.c_str());

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
}

void PlaylistAdd::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if(keyRepeatDelay)	keyRepeatDelay--;
	gspWaitForVBlank();

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
			Gui::screenBack();
			return;
	} else if (hDown & KEY_B) {
			Gui::screenBack();
			return;
	} else if (hDown & KEY_Y) {
		std::string newPlaylist = Input::getLine();
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
			keyRepeatDelay = 3;
		}
	} else if (hHeld & KEY_DOWN && !keyRepeatDelay) {
		if (selectedPlst < plsts.size()-1) {
			selectedPlst++;
			playScrollSfx();
			keyRepeatDelay = 3;
		}
	}
}