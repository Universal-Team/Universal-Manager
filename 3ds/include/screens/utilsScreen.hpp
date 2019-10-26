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

#include <vector>

class Utils : public SCREEN
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int Selection = 0;
	// Functions.
	void drawSelection(void) const;
	void SelectionLogic(u32 hDown);

	std::vector<Structs::ButtonPos> utilsButtonPos = {
			// Calendar.
		{1, 40, 149, 52, -1},
			// Button Tester. ;P
		{162, 40, 149, 52, -1},
			// Calculator.
		{1, 150, 149, 52, -1},
			// Back Icon.
		{293, 213, 27, 27, -1},
	};
};