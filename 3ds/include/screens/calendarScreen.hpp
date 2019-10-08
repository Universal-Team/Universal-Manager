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
#include "lang/langStrings.h"

#include <citro2d.h>
#include <string>
#include <vector>

// CALENDAR
class Calendar : public SCREEN
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	C2D_Font systemFont;
	C2D_TextBuf dynamicBuf, sizeBuf;

	std::vector<std::string> months = {
		Lang::Calendar[0],
		Lang::Calendar[1],
		Lang::Calendar[2],
		Lang::Calendar[3],
		Lang::Calendar[4],
		Lang::Calendar[5],
		Lang::Calendar[6],
		Lang::Calendar[7],
		Lang::Calendar[8],
		Lang::Calendar[9],
		Lang::Calendar[10],
		Lang::Calendar[11],
	};

	std::vector<std::string> weekDays = {
		Lang::Calendar[12],
		Lang::Calendar[13],
		Lang::Calendar[14],
		Lang::Calendar[15],
		Lang::Calendar[16],
		Lang::Calendar[17],
		Lang::Calendar[18],
	};

	std::vector<Structs::ButtonPos> calendarButtonPos = {
		// Back Icon.
		{293, 213, 27, 27, -1},
	};

	void displayDays(void) const;
	void displayMonthYear(void) const;
	void drawWeekDays(void) const;
};