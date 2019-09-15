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

#include "screens/calendarScreen.hpp"
#include "screens/screenCommon.hpp"
#include "utils/settings.hpp"

#include <algorithm>
#include <fstream>
#include <unistd.h>

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void Calendar::displayMonthYear(void) const {
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int month = timeStruct->tm_mon;
	int year = timeStruct->tm_year + 1900;

	Gui::DrawString(200-(Gui::GetStringWidth(0.72f, months[month]+" "+std::to_string(year))/2), 0, 0.72f, WHITE, (months[month]+" "+std::to_string(year)).c_str());
}

void drawDays(int year, int month, int day) {
	tm timeIn = {0, 0, 0, 1, month, year};

	time_t timeTemp = mktime(&timeIn);
	tm* time = localtime(&timeTemp);

	int days = 0;
	if(month == 2) {
		day = (year%4) ? 28 : 29;
	} else if(month == 4 || month == 6 || month == 9 || month == 11) {
		days = 30;
	} else {
		days = 31;
	}

	for(int i=1;i<days;i++) {
		int j = i-1+time->tm_wday;
		int x = j-((j/7)*7);
		int y = j/7;
		Draw_Rect(10+(x*55), 35+(y*35), 50, 30, i == day ? Config::barColor : GRAY);
		Gui::DrawString(35+(x*55)-(Gui::GetStringWidth(0.72f, std::to_string(i))/2), 38+(y*35), 0.72f, WHITE, std::to_string(i));
	}
}

void Calendar::drawWeekDays(void) const {
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int weekDay = timeStruct->tm_wday;
	std::string currentDay = Lang::Calendar[19];
	currentDay += weekDays[weekDay];

	Gui::DrawString(200-(Gui::GetStringWidth(0.72f, currentDay.c_str())/2), 217, 0.72f, WHITE, currentDay.c_str());
}

void Calendar::displayDays(void) const {
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);

	drawDays(timeStruct->tm_year, timeStruct->tm_mon, timeStruct->tm_mday);
}

void Calendar::Draw(void) const {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	displayMonthYear();
	displayDays();
	drawWeekDays();

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();
}

void Calendar::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, calendarButtonPos[0])) {
			Gui::screenBack();
			return;
		}
	}
}
