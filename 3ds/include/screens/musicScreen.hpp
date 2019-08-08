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
#include "structs.hpp"
#include "fileBrowse.h"

// Music Main.
class Music : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	Music();

private:
	int MusicMode = 0;

	void DrawMusicMain(void) const;
	void MusicMainLogic(u32 hDown, touchPosition touch);

	void DrawMusicList(void) const;
	void MusicListLogic(u32 hDown, u32 hHeld);

	bool Draw_ThemeImage(void) const;
	bool Draw_CoverImage(void) const;
	std::string secondsToString(u64 seconds) const;
	void DrawPlayer(void) const;
	void PlayerLogic(u32 hDown, touchPosition touch);

	void DrawPlaylistAdd(void) const;
	void PlaylistAddLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DrawPlaylistEdit(void) const;
	void PlaylistEditLogic(u32 hDown, u32 hHeld);

	void DrawPlaylistPlay(void) const;
	void PlaylistPlayLogic(u32 hDown, u32 hHeld);

	void DrawThemeSelector(void) const;
	void ThemeSelectorLogic(u32 hDown, u32 hHeld);

	int selection = 0;
	bool dirChanged = true;
	mutable bool themeImageLoaded = false;
	mutable bool coverImageLoaded = false;

	uint selectedFile = 0;
	int keyRepeatDelay = 3;
	std::vector<DirEntry> dirContents;
	uint selectedPlst = 0;

	std::vector<DirEntry> plsts;
	mutable bool decForRepeat2 = false;
	uint selectedPlstItem = 0;
	int movingPlstItem = -1;

	std::vector<std::string> plstContents;
	C2D_Image musicImage, coverImage;

	mutable bool firstSong = true;
	int locInPlaylist = 0;
	int musicRepeat = 0;
	mutable bool musicShuffle = 0;
	std::vector<Playlist> nowPlayingList;
	std::string currentSong = "";
	
	std::vector<Structs::ButtonPos> Functions = {
    	{0, 40, 149, 52, -1}, // 0
    	{170, 40, 149, 52, -1}, // 1
    	{0, 150, 149, 52, -1}, // 2
		{170, 150, 149, 52, -1}, // 3

		{130, 90, 60, 60, -1}, // 4
		{80, 100, 40, 40, -1}, // 5
		{200, 100, 40, 40, -1}, // 6
		{240, 170, 30, 30, -1}, // 7
		{280, 170, 30, 30, -1}, // 8
	};
};