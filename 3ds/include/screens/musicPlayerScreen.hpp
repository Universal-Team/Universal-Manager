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

#include "screen.hpp"
#include <string>
#include <vector>
#include "button.hpp"
#include "fileBrowse.h"

// Music Main.
class MusicPlayer : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	bool themeImageLoaded = false;
	bool coverImageLoaded = false;

	uint selectedFile = 0;
	int keyRepeatDelay = 3;
	bool dirChanged = true;
	std::vector<DirEntry> dirContents;
	std::string scanDir;
	std::string currentSong = "";
	int musicRepeat = 0;
	bool musicShuffle = 0;

	bool firstSong = true;
	int locInPlaylist = 0;
	uint selectedPlst = 0;
	std::vector<DirEntry> plsts;
	std::vector<Playlist> nowPlayingList;
	bool decForRepeat2 = false;

	uint selectedPlstItem = 0;
	int movingPlstItem = -1;
	std::vector<std::string> plstContents;
	int selection = 0;
	C2D_Image musicImage, coverImage;

	std::vector<Structs::ButtonPos> playerButtonPos = {
		{130, 90, 60, 60, -1},
		{80, 100, 40, 40, -1},
		{200, 100, 40, 40, -1},
		{240, 170, 30, 30, -1},
		{280, 170, 30, 30, -1},
	};

	std::string secondsToString(u64 seconds) const;

	bool Draw_CoverImage(void) const;
	bool Draw_ThemeImage(void) const;
};