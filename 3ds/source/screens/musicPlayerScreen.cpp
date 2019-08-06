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
#include "musicPlayerScreen.hpp"
#include "settings.hpp"

extern "C" {
	#include "music/error.h"
	#include "music/playback.h"
}

extern bool touching(touchPosition touch, Structs::ButtonPos button);

bool MusicPlayer::Draw_ThemeImage(void) const
{
	return C2D_DrawImageAt(musicImage, 0, 25, 0.5);
}

bool MusicPlayer::Draw_CoverImage(void) const
{
	return C2D_DrawImageAt(coverImage, 230, 25, 0.5);
}

std::string MusicPlayer::secondsToString(u64 seconds) const
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

void MusicPlayer::Draw(void) const
{

	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();

		// Placeholder for Metadata Stuff..
	 	if (Config::musicMode == 0) {
	} else if (Config::musicMode == 1) {

			if ((metadata.has_meta) && (metadata.title[0] != '\0') && (metadata.artist[0] != '\0') && (metadata.album[0] != '\0') && (metadata.year[0] != '\0')) {
			Draw_Text(15, 40, 0.5f, WHITE, strupr(metadata.title));
			Draw_Text(15, 60, 0.45f, WHITE, strupr(metadata.artist));
			Draw_Text(15, 80, 0.45f, WHITE, strupr(metadata.album));
			Draw_Text(15, 100, 0.45f, WHITE, strupr(metadata.year));
		} else {
			Draw_Text(15, 40, 0.5f, WHITE, "No Metadata Found.");
		}

		if (coverImageLoaded == true) {
			Draw_CoverImage(); 
		} else if (coverImageLoaded == false) {
			Gui::sprite(sprites_cover_glow_idx, 250, 30);
			Gui::sprite(sprites_cover_normal_idx, 250, 30);
		}

	} else if (Config::musicMode == 2) {
		if (themeImageLoaded == true) {
		Draw_ThemeImage(); 
	} else if (themeImageLoaded == false) {
			Draw_Text(15, 40, 0.5f, WHITE, "Please load an Image under `Themes`.");
	}
	}
	
	if(isPlaying()) {

		// Current Song.
		std::string nowPlayingText = "Current Song: " + currentSong.substr(currentSong.find_last_of("/")+1);
		Draw_Text(0, 0, 0.45f, WHITE, nowPlayingText.c_str());


		// If Filetype is MP3, display the Progressbar, else not.
		if ((strcasecmp(dirContents[selectedFile].name.substr(dirContents[selectedFile].name.length()-3, 3).c_str(), "mp3") == 0)) {
		// Progressbar - Time.
		Draw_Rect(100, 179, 207, 10, GRAY);
		Draw_Text(110, 177, 0.45f, WHITE, (secondsToString(Audio_GetPosition()/Audio_GetRate()) + "                            " + secondsToString(Audio_GetLength()/Audio_GetRate())).c_str());
		// Progressbar - Display.
		Draw_Rect(120, 194, 150, 12, GRAY);
		Draw_Rect(120, 194, (((double)Audio_GetPosition()/(double)Audio_GetLength()) * 150.0), 12, Config::barColor);
		} else {
			Draw_Text(40, 177, 0.45f, WHITE, "Progressbar not available for this Music Format yet.");
		}

	
	if(!isPaused() && isPlaying()) {
		Draw_Text(80, 220, 0.45f, WHITE, "A : Pause   B : Back   X : Stop song");
	} else if(isPaused() && isPlaying()) {
		Draw_Text(80, 220, 0.45f, WHITE, "A : Play   B : Back   X : Stop song");
	} else {
		Draw_Text(0, 0, 0.72f, WHITE, "No Song Selected.");
		Draw_Text(80, 220, 0.45f, WHITE, "B : Back");
	}


	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBot();
	DisplayTime();
	drawBatteryBot();

	Gui::Draw_ImageBlend(!isPaused() ? sprites_pause_icon_glow_idx : sprites_play_icon_glow_idx, playerButtonPos[0].x, playerButtonPos[0].y, Config::barColor);
	Gui::sprite(!isPaused() ? sprites_pause_icon_normal_idx : sprites_play_icon_normal_idx, playerButtonPos[0].x, playerButtonPos[0].y);

	Gui::Draw_ImageBlend(sprites_left_arrow_glow_idx, playerButtonPos[1].x, playerButtonPos[1].y, Config::barColor);
	Gui::sprite(sprites_left_arrow_normal_idx, playerButtonPos[1].x, playerButtonPos[1].y);

	Gui::Draw_ImageBlend(sprites_right_icon_glow_idx, playerButtonPos[2].x, playerButtonPos[2].y, Config::barColor);
	Gui::sprite(sprites_right_icon_normal_idx, playerButtonPos[2].x, playerButtonPos[2].y);

	Gui::Draw_ImageBlend(sprites_shuffle_icon_idx, playerButtonPos[3].x, playerButtonPos[3].y, (musicShuffle ? WHITE : Config::barColor));
	Gui::Draw_ImageBlend(sprites_repeat_icon_idx, playerButtonPos[4].x, playerButtonPos[4].y, (musicRepeat ? WHITE : Config::barColor));
	if (musicRepeat)	Draw_Text(playerButtonPos[4].x+11, playerButtonPos[4].y+9, 0.5f, BLACK, (musicRepeat == 1 ? "A" : "S"));
}
}

bool playlistSortPredicate(const Playlist &lhs, const Playlist &rhs) {
	if(lhs.position < rhs.position)	return true;
	else	return false;
}

bool playlistShufflePredicate(const Playlist &lhs, const Playlist &rhs) {
	return rand() % 2;
}
void MusicPlayer::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_A) {
		togglePlayback();
	} else if (hDown & KEY_X) {
		stopPlayback();
	} else if (hDown & KEY_B) {
		Gui::screenBack();
		return;
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