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

#include "structs.hpp"
#include "screens/screen.hpp"
#include "utils/fileBrowse.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <vector>

class Script : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int ScriptMode = 0;
	int Selection = 0;
	int ScriptPage = 1;
	uint selectedFile = 0;
	int keyRepeatDelay = 3;
	mutable bool refresh = true;
	std::vector<DirEntry> dirContents;
	std::ofstream scpt;

		// Structs.
	std::vector<Structs::ButtonPos> Functions = {
		// First Page.
    	{0, 25, 149, 52, -1}, // 0
    	{170, 25, 149, 52, -1}, // 1
		{0, 90, 149, 52, -1}, // 2
		{170, 90, 149, 52, -1}, // 3
		{0, 150, 149, 52, -1}, // 4
    	{170, 150, 149, 52, -1}, // 5
		// Second Page.
		{0, 25, 149, 52, -1}, // 6
	};

	void DrawScriptBrowse(void) const;
	void ScriptBrowseLogic(u32 hDown, u32 hHeld, touchPosition touch);


	void DrawScriptCreator(void) const;
	void DrawBottom(void) const;
	void DrawSelection(void) const;
	void DrawCurrentPage(void) const;

	void ScriptCreatorSelectionLogic(u32 hDown, u32 hHeld);
	void ScriptCreatorLogic(u32 hDown, u32 hHeld, touchPosition touch);
};