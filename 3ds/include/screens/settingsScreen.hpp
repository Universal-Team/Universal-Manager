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

class Settings : public SCREEN
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int SettingsPage = 1;

	void DrawBottom(void) const;
	void DrawCurrentPage(void) const;

	void ButtonLogic(u32 hDown, u32 hHeld);
	void TouchLogic(u32 hDown, touchPosition touch);

	// Struct.
	std::vector<Structs::ButtonPos> uiSettingsButtonPos = {
		// Bars
		{17, 85, 95, 41, -1},
		{112, 85, 95, 41, -1},
		{207, 85, 95, 41, -1},
		// Background

		{17, 165, 95, 41, -1},
		{112, 165, 95, 41, -1},
		{207, 165, 95, 41, -1},

		{293, 213, 27, 27, -1},

		// Music BG
		{207, 31, 95, 41, -1},

		// Bubble Color.
		{17, 100, 95, 41, -1},
		{112, 100, 95, 41, -1},
		{207, 100, 95, 41, -1},

		// Animation enable.
		{207, 31, 95, 41, -1},

		// Battery percent.
		{207, 165, 87, 33, -1},

		// Bars Layout.
		{17, 165, 95, 41, -1},

		// BG Layout.
		{17, 31, 95, 41, -1},

		// Selected Text Color.

		{17, 105, 95, 41, -1},
		{112, 105, 95, 41, -1},
		{207, 105, 95, 41, -1},

		// Unselected Text Color.

		{17, 165, 95, 41, -1},
		{112, 165, 95, 41, -1},
		{207, 165, 95, 41, -1},

		{17, 31, 95, 41, -1}, // Randomize all colors. :P
	};
};

namespace ColorHelper {
	int getColorValue(int color, int bgr);
	std::string getColorName(int color, int bgr);
}