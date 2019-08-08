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
#include "fileBrowse.h"
#include "structs.hpp"

class ScriptCreator : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int scriptSelection1 = 0;
	int scriptSelection2 = 0;
	uint selectedFile;
	int screenPage = 0;
	int keyRepeatDelay;
	bool dirChanged;
	std::vector<DirEntry> dirContents;
	uint selectedScpt = 0;
	std::vector<DirEntry> scpts;

	std::ofstream scpt;

	uint selectedScptItem = 0;
	int movingScptItem = -1;
	std::vector<std::string> scptContents;

		// Structs.
	std::vector<Structs::ButtonPos> scriptCreatorFunctionButtonPos = {
		// First Page.
    	{0, 25, 149, 52, -1},
    	{170, 25, 149, 52, -1},
		{0, 90, 149, 52, -1},
		{170, 90, 149, 52, -1},
		{0, 150, 149, 52, -1},
    	{170, 150, 149, 52, -1},
		// Second Page.
		{0, 25, 149, 52, -1},
	};


	void drawSelection1(void) const;
	void drawSelection2(void) const;
	void scriptSelectionLogic1(u32 hDown);
	void scriptSelectionLogic2(u32 hDown);
};