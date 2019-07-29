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

#include "screens/screenCommon.hpp"
#include "settings.hpp"
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <vector>

struct ButtonPos {
	int x;
	int y;
	int w;
	int h;
	int link;
};

std::vector<std::string> months = {
	"January",
	"Febuary",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

extern bool touching(touchPosition touch, ButtonPos button);
inline C2D_Font systemFont;
inline C2D_TextBuf dynamicBuf, sizeBuf;

ButtonPos calendarButtonPos[] = {
		// Back Icon.
	{293, 213, 27, 27, -1},
};

static void DisplayMonth(void) {
	time_t unixTime       = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int month = timeStruct->tm_mon;

	Draw_Text(120, 0, 0.72f, WHITE, months[month].c_str());
}

std::string GetYear(void) {
	time_t unixTime       = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int year = timeStruct->tm_year + 1900;
	return StringUtils::format("%04i", year);
}

static void DisplayYear(void) {
	time_t unixTime       = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int month = timeStruct->tm_mon;

	Draw_Text(130+Draw_GetTextWidth(0.72f, (months[month].c_str())), 0, 0.72f, WHITE, (GetYear().c_str()));
}

static void draw31Days(void) {
	Draw_Rect(10, 25, 50, 30, GRAY);
	Draw_Text(30, 28, 0.72f, WHITE, "1");

	Draw_Rect(65, 25, 50, 30, GRAY);
	Draw_Text(85, 28, 0.72f, WHITE, "2");

	Draw_Rect(120, 25, 50, 30, GRAY);
	Draw_Text(140, 28, 0.72f, WHITE, "3");

	Draw_Rect(175, 25, 50, 30, GRAY);
	Draw_Text(195, 28, 0.72f, WHITE, "4");

	Draw_Rect(230, 25, 50, 30, GRAY);
	Draw_Text(250, 28, 0.72f, WHITE, "5");

	Draw_Rect(285, 25, 50, 30, GRAY);
	Draw_Text(305, 28, 0.72f, WHITE, "6");

	Draw_Rect(340, 25, 50, 30, GRAY);
	Draw_Text(360, 28, 0.72f, WHITE, "7");


	Draw_Rect(10, 60, 50, 30, GRAY);
	Draw_Text(30, 63, 0.72f, WHITE, "8");

	Draw_Rect(65, 60, 50, 30, GRAY);
	Draw_Text(85, 63, 0.72f, WHITE, "9");

	Draw_Rect(120, 60, 50, 30, GRAY);
	Draw_Text(132, 63, 0.72f, WHITE, "10");

	Draw_Rect(175, 60, 50, 30, GRAY);
	Draw_Text(187, 63, 0.72f, WHITE, "11");

	Draw_Rect(230, 60, 50, 30, GRAY);
	Draw_Text(242, 63, 0.72f, WHITE, "12");

	Draw_Rect(285, 60, 50, 30, GRAY);
	Draw_Text(297, 63, 0.72f, WHITE, "13");

	Draw_Rect(340, 60, 50, 30, GRAY);
	Draw_Text(352, 63, 0.72f, WHITE, "14");


	Draw_Rect(10, 95, 50, 30, GRAY);
	Draw_Text(22, 98, 0.72f, WHITE, "15");

	Draw_Rect(65, 95, 50, 30, GRAY);
	Draw_Text(77, 98, 0.72f, WHITE, "16");

	Draw_Rect(120, 95, 50, 30, GRAY);
	Draw_Text(132, 98, 0.72f, WHITE, "17");

	Draw_Rect(175, 95, 50, 30, GRAY);
	Draw_Text(187, 98, 0.72f, WHITE, "18");

	Draw_Rect(230, 95, 50, 30, GRAY);
	Draw_Text(242, 98, 0.72f, WHITE, "19");

	Draw_Rect(285, 95, 50, 30, GRAY);
	Draw_Text(297, 98, 0.72f, WHITE, "20");

	Draw_Rect(340, 95, 50, 30, GRAY);
	Draw_Text(352, 98, 0.72f, WHITE, "21");


	Draw_Rect(10, 130, 50, 30, GRAY);
	Draw_Text(22, 133, 0.72f, WHITE, "22");

	Draw_Rect(65, 130, 50, 30, GRAY);
	Draw_Text(77, 133, 0.72f, WHITE, "23");

	Draw_Rect(120, 130, 50, 30, GRAY);
	Draw_Text(132, 133, 0.72f, WHITE, "24");

	Draw_Rect(175, 130, 50, 30, GRAY);
	Draw_Text(187, 133, 0.72f, WHITE, "25");

	Draw_Rect(230, 130, 50, 30, GRAY);
	Draw_Text(242, 133, 0.72f, WHITE, "26");

	Draw_Rect(285, 130, 50, 30, GRAY);
	Draw_Text(297, 133, 0.72f, WHITE, "27");

	Draw_Rect(340, 130, 50, 30, GRAY);
	Draw_Text(352, 133, 0.72f, WHITE, "28");


	Draw_Rect(10, 165, 50, 30, GRAY);
	Draw_Text(22, 168, 0.72f, WHITE, "29");

	Draw_Rect(65, 165, 50, 30, GRAY);
	Draw_Text(77, 168, 0.72f, WHITE, "30");

	Draw_Rect(120, 165, 50, 30, GRAY);
	Draw_Text(132, 168, 0.72f, WHITE, "31");
}

static void draw30Days(void) {
	Draw_Rect(10, 25, 50, 30, GRAY);
	Draw_Text(30, 28, 0.72f, WHITE, "1");

	Draw_Rect(65, 25, 50, 30, GRAY);
	Draw_Text(85, 28, 0.72f, WHITE, "2");

	Draw_Rect(120, 25, 50, 30, GRAY);
	Draw_Text(140, 28, 0.72f, WHITE, "3");

	Draw_Rect(175, 25, 50, 30, GRAY);
	Draw_Text(195, 28, 0.72f, WHITE, "4");

	Draw_Rect(230, 25, 50, 30, GRAY);
	Draw_Text(250, 28, 0.72f, WHITE, "5");

	Draw_Rect(285, 25, 50, 30, GRAY);
	Draw_Text(305, 28, 0.72f, WHITE, "6");

	Draw_Rect(340, 25, 50, 30, GRAY);
	Draw_Text(360, 28, 0.72f, WHITE, "7");


	Draw_Rect(10, 60, 50, 30, GRAY);
	Draw_Text(30, 63, 0.72f, WHITE, "8");

	Draw_Rect(65, 60, 50, 30, GRAY);
	Draw_Text(85, 63, 0.72f, WHITE, "9");

	Draw_Rect(120, 60, 50, 30, GRAY);
	Draw_Text(132, 63, 0.72f, WHITE, "10");

	Draw_Rect(175, 60, 50, 30, GRAY);
	Draw_Text(187, 63, 0.72f, WHITE, "11");

	Draw_Rect(230, 60, 50, 30, GRAY);
	Draw_Text(242, 63, 0.72f, WHITE, "12");

	Draw_Rect(285, 60, 50, 30, GRAY);
	Draw_Text(297, 63, 0.72f, WHITE, "13");

	Draw_Rect(340, 60, 50, 30, GRAY);
	Draw_Text(352, 63, 0.72f, WHITE, "14");


	Draw_Rect(10, 95, 50, 30, GRAY);
	Draw_Text(22, 98, 0.72f, WHITE, "15");

	Draw_Rect(65, 95, 50, 30, GRAY);
	Draw_Text(77, 98, 0.72f, WHITE, "16");

	Draw_Rect(120, 95, 50, 30, GRAY);
	Draw_Text(132, 98, 0.72f, WHITE, "17");

	Draw_Rect(175, 95, 50, 30, GRAY);
	Draw_Text(187, 98, 0.72f, WHITE, "18");

	Draw_Rect(230, 95, 50, 30, GRAY);
	Draw_Text(242, 98, 0.72f, WHITE, "19");

	Draw_Rect(285, 95, 50, 30, GRAY);
	Draw_Text(297, 98, 0.72f, WHITE, "20");

	Draw_Rect(340, 95, 50, 30, GRAY);
	Draw_Text(352, 98, 0.72f, WHITE, "21");


	Draw_Rect(10, 130, 50, 30, GRAY);
	Draw_Text(22, 133, 0.72f, WHITE, "22");

	Draw_Rect(65, 130, 50, 30, GRAY);
	Draw_Text(77, 133, 0.72f, WHITE, "23");

	Draw_Rect(120, 130, 50, 30, GRAY);
	Draw_Text(132, 133, 0.72f, WHITE, "24");

	Draw_Rect(175, 130, 50, 30, GRAY);
	Draw_Text(187, 133, 0.72f, WHITE, "25");

	Draw_Rect(230, 130, 50, 30, GRAY);
	Draw_Text(242, 133, 0.72f, WHITE, "26");

	Draw_Rect(285, 130, 50, 30, GRAY);
	Draw_Text(297, 133, 0.72f, WHITE, "27");

	Draw_Rect(340, 130, 50, 30, GRAY);
	Draw_Text(352, 133, 0.72f, WHITE, "28");


	Draw_Rect(10, 165, 50, 30, GRAY);
	Draw_Text(22, 168, 0.72f, WHITE, "29");

	Draw_Rect(65, 165, 50, 30, GRAY);
	Draw_Text(77, 168, 0.72f, WHITE, "30");
}

static void draw29Days(void) {
	Draw_Rect(10, 25, 50, 30, GRAY);
	Draw_Text(30, 28, 0.72f, WHITE, "1");

	Draw_Rect(65, 25, 50, 30, GRAY);
	Draw_Text(85, 28, 0.72f, WHITE, "2");

	Draw_Rect(120, 25, 50, 30, GRAY);
	Draw_Text(140, 28, 0.72f, WHITE, "3");

	Draw_Rect(175, 25, 50, 30, GRAY);
	Draw_Text(195, 28, 0.72f, WHITE, "4");

	Draw_Rect(230, 25, 50, 30, GRAY);
	Draw_Text(250, 28, 0.72f, WHITE, "5");

	Draw_Rect(285, 25, 50, 30, GRAY);
	Draw_Text(305, 28, 0.72f, WHITE, "6");

	Draw_Rect(340, 25, 50, 30, GRAY);
	Draw_Text(360, 28, 0.72f, WHITE, "7");


	Draw_Rect(10, 60, 50, 30, GRAY);
	Draw_Text(30, 63, 0.72f, WHITE, "8");

	Draw_Rect(65, 60, 50, 30, GRAY);
	Draw_Text(85, 63, 0.72f, WHITE, "9");

	Draw_Rect(120, 60, 50, 30, GRAY);
	Draw_Text(132, 63, 0.72f, WHITE, "10");

	Draw_Rect(175, 60, 50, 30, GRAY);
	Draw_Text(187, 63, 0.72f, WHITE, "11");

	Draw_Rect(230, 60, 50, 30, GRAY);
	Draw_Text(242, 63, 0.72f, WHITE, "12");

	Draw_Rect(285, 60, 50, 30, GRAY);
	Draw_Text(297, 63, 0.72f, WHITE, "13");

	Draw_Rect(340, 60, 50, 30, GRAY);
	Draw_Text(352, 63, 0.72f, WHITE, "14");


	Draw_Rect(10, 95, 50, 30, GRAY);
	Draw_Text(22, 98, 0.72f, WHITE, "15");

	Draw_Rect(65, 95, 50, 30, GRAY);
	Draw_Text(77, 98, 0.72f, WHITE, "16");

	Draw_Rect(120, 95, 50, 30, GRAY);
	Draw_Text(132, 98, 0.72f, WHITE, "17");

	Draw_Rect(175, 95, 50, 30, GRAY);
	Draw_Text(187, 98, 0.72f, WHITE, "18");

	Draw_Rect(230, 95, 50, 30, GRAY);
	Draw_Text(242, 98, 0.72f, WHITE, "19");

	Draw_Rect(285, 95, 50, 30, GRAY);
	Draw_Text(297, 98, 0.72f, WHITE, "20");

	Draw_Rect(340, 95, 50, 30, GRAY);
	Draw_Text(352, 98, 0.72f, WHITE, "21");


	Draw_Rect(10, 130, 50, 30, GRAY);
	Draw_Text(22, 133, 0.72f, WHITE, "22");

	Draw_Rect(65, 130, 50, 30, GRAY);
	Draw_Text(77, 133, 0.72f, WHITE, "23");

	Draw_Rect(120, 130, 50, 30, GRAY);
	Draw_Text(132, 133, 0.72f, WHITE, "24");

	Draw_Rect(175, 130, 50, 30, GRAY);
	Draw_Text(187, 133, 0.72f, WHITE, "25");

	Draw_Rect(230, 130, 50, 30, GRAY);
	Draw_Text(242, 133, 0.72f, WHITE, "26");

	Draw_Rect(285, 130, 50, 30, GRAY);
	Draw_Text(297, 133, 0.72f, WHITE, "27");

	Draw_Rect(340, 130, 50, 30, GRAY);
	Draw_Text(352, 133, 0.72f, WHITE, "28");


	Draw_Rect(10, 165, 50, 30, GRAY);
	Draw_Text(22, 168, 0.72f, WHITE, "29");
}

static void draw28Days(void) {
	Draw_Rect(10, 25, 50, 30, GRAY);
	Draw_Text(30, 28, 0.72f, WHITE, "1");

	Draw_Rect(65, 25, 50, 30, GRAY);
	Draw_Text(85, 28, 0.72f, WHITE, "2");

	Draw_Rect(120, 25, 50, 30, GRAY);
	Draw_Text(140, 28, 0.72f, WHITE, "3");

	Draw_Rect(175, 25, 50, 30, GRAY);
	Draw_Text(195, 28, 0.72f, WHITE, "4");

	Draw_Rect(230, 25, 50, 30, GRAY);
	Draw_Text(250, 28, 0.72f, WHITE, "5");

	Draw_Rect(285, 25, 50, 30, GRAY);
	Draw_Text(305, 28, 0.72f, WHITE, "6");

	Draw_Rect(340, 25, 50, 30, GRAY);
	Draw_Text(360, 28, 0.72f, WHITE, "7");


	Draw_Rect(10, 60, 50, 30, GRAY);
	Draw_Text(30, 63, 0.72f, WHITE, "8");

	Draw_Rect(65, 60, 50, 30, GRAY);
	Draw_Text(85, 63, 0.72f, WHITE, "9");

	Draw_Rect(120, 60, 50, 30, GRAY);
	Draw_Text(132, 63, 0.72f, WHITE, "10");

	Draw_Rect(175, 60, 50, 30, GRAY);
	Draw_Text(187, 63, 0.72f, WHITE, "11");

	Draw_Rect(230, 60, 50, 30, GRAY);
	Draw_Text(242, 63, 0.72f, WHITE, "12");

	Draw_Rect(285, 60, 50, 30, GRAY);
	Draw_Text(297, 63, 0.72f, WHITE, "13");

	Draw_Rect(340, 60, 50, 30, GRAY);
	Draw_Text(352, 63, 0.72f, WHITE, "14");


	Draw_Rect(10, 95, 50, 30, GRAY);
	Draw_Text(22, 98, 0.72f, WHITE, "15");

	Draw_Rect(65, 95, 50, 30, GRAY);
	Draw_Text(77, 98, 0.72f, WHITE, "16");

	Draw_Rect(120, 95, 50, 30, GRAY);
	Draw_Text(132, 98, 0.72f, WHITE, "17");

	Draw_Rect(175, 95, 50, 30, GRAY);
	Draw_Text(187, 98, 0.72f, WHITE, "18");

	Draw_Rect(230, 95, 50, 30, GRAY);
	Draw_Text(242, 98, 0.72f, WHITE, "19");

	Draw_Rect(285, 95, 50, 30, GRAY);
	Draw_Text(297, 98, 0.72f, WHITE, "20");

	Draw_Rect(340, 95, 50, 30, GRAY);
	Draw_Text(352, 98, 0.72f, WHITE, "21");


	Draw_Rect(10, 130, 50, 30, GRAY);
	Draw_Text(22, 133, 0.72f, WHITE, "22");

	Draw_Rect(65, 130, 50, 30, GRAY);
	Draw_Text(77, 133, 0.72f, WHITE, "23");

	Draw_Rect(120, 130, 50, 30, GRAY);
	Draw_Text(132, 133, 0.72f, WHITE, "24");

	Draw_Rect(175, 130, 50, 30, GRAY);
	Draw_Text(187, 133, 0.72f, WHITE, "25");

	Draw_Rect(230, 130, 50, 30, GRAY);
	Draw_Text(242, 133, 0.72f, WHITE, "26");

	Draw_Rect(285, 130, 50, 30, GRAY);
	Draw_Text(297, 133, 0.72f, WHITE, "27");

	Draw_Rect(340, 130, 50, 30, GRAY);
	Draw_Text(352, 133, 0.72f, WHITE, "28");
}


static void drawCurrentDay(void) {
	time_t unixTime       = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int day = timeStruct->tm_mday;

			if (day == 1) {
	Draw_Rect(10, 25, 50, 30, Config::barColor);
	Draw_Text(30, 28, 0.72f, WHITE, "1");
	} else if (day == 2) {
	Draw_Rect(65, 25, 50, 30, Config::barColor);
	Draw_Text(85, 28, 0.72f, WHITE, "2");
	} else if (day == 3) {
	Draw_Rect(120, 25, 50, 30, Config::barColor);
	Draw_Text(140, 28, 0.72f, WHITE, "3");
	} else if (day == 4) {
	Draw_Rect(175, 25, 50, 30, Config::barColor);
	Draw_Text(195, 28, 0.72f, WHITE, "4");
	} else if (day == 5) {
	Draw_Rect(230, 25, 50, 30, Config::barColor);
	Draw_Text(250, 28, 0.72f, WHITE, "5");
	} else if (day == 6) {
	Draw_Rect(285, 25, 50, 30, Config::barColor);
	Draw_Text(305, 28, 0.72f, WHITE, "6");
	} else if (day == 7) {
	Draw_Rect(340, 25, 50, 30, Config::barColor);
	Draw_Text(360, 28, 0.72f, WHITE, "7");

	} else if (day == 8) {
	Draw_Rect(10, 60, 50, 30, Config::barColor);
	Draw_Text(30, 63, 0.72f, WHITE, "8");
	} else if (day == 9) {
	Draw_Rect(65, 60, 50, 30, Config::barColor);
	Draw_Text(85, 63, 0.72f, WHITE, "9");
	} else if (day == 10) {
	Draw_Rect(120, 60, 50, 30, Config::barColor);
	Draw_Text(132, 63, 0.72f, WHITE, "10");
	} else if (day == 11) {
	Draw_Rect(175, 60, 50, 30, Config::barColor);
	Draw_Text(187, 63, 0.72f, WHITE, "11");
	} else if (day == 12) {
	Draw_Rect(230, 60, 50, 30, Config::barColor);
	Draw_Text(242, 63, 0.72f, WHITE, "12");
	} else if (day == 13) {
	Draw_Rect(285, 60, 50, 30, Config::barColor);
	Draw_Text(297, 63, 0.72f, WHITE, "13");
	} else if (day == 14) {
	Draw_Rect(340, 60, 50, 30, Config::barColor);
	Draw_Text(352, 63, 0.72f, WHITE, "14");

	} else if (day == 15) {
	Draw_Rect(10, 95, 50, 30, Config::barColor);
	Draw_Text(22, 98, 0.72f, WHITE, "15");
	} else if (day == 16) {
	Draw_Rect(65, 95, 50, 30, Config::barColor);
	Draw_Text(77, 98, 0.72f, WHITE, "16");
	} else if (day == 17) {
	Draw_Rect(120, 95, 50, 30, Config::barColor);
	Draw_Text(132, 98, 0.72f, WHITE, "17");
	} else if (day == 18) {
	Draw_Rect(175, 95, 50, 30, Config::barColor);
	Draw_Text(187, 98, 0.72f, WHITE, "18");
	} else if (day == 19) {
	Draw_Rect(230, 95, 50, 30, Config::barColor);
	Draw_Text(242, 98, 0.72f, WHITE, "19");
	} else if (day == 20) {
	Draw_Rect(285, 95, 50, 30, Config::barColor);
	Draw_Text(297, 98, 0.72f, WHITE, "20");
	} else if (day == 21) {
	Draw_Rect(340, 95, 50, 30, Config::barColor);
	Draw_Text(352, 98, 0.72f, WHITE, "21");

	} else if (day == 22) {
	Draw_Rect(10, 130, 50, 30, Config::barColor);
	Draw_Text(22, 133, 0.72f, WHITE, "22");
	} else if (day == 23) {
	Draw_Rect(65, 130, 50, 30, Config::barColor);
	Draw_Text(77, 133, 0.72f, WHITE, "23");
	} else if (day == 24) {
	Draw_Rect(120, 130, 50, 30, Config::barColor);
	Draw_Text(132, 133, 0.72f, WHITE, "24");
	} else if (day == 25) {
	Draw_Rect(175, 130, 50, 30, Config::barColor);
	Draw_Text(187, 133, 0.72f, WHITE, "25");
	} else if (day == 26) {
	Draw_Rect(230, 130, 50, 30, Config::barColor);
	Draw_Text(242, 133, 0.72f, WHITE, "26");
	} else if (day == 27) {
	Draw_Rect(285, 130, 50, 30, Config::barColor);
	Draw_Text(297, 133, 0.72f, WHITE, "27");
	} else if (day == 28) {
	Draw_Rect(340, 130, 50, 30, Config::barColor);
	Draw_Text(352, 133, 0.72f, WHITE, "28");

	} else if (day == 29) {
	Draw_Rect(10, 165, 50, 30, Config::barColor);
	Draw_Text(22, 168, 0.72f, WHITE, "29");
	} else if (day == 30) {
	Draw_Rect(65, 165, 50, 30, Config::barColor);
	Draw_Text(77, 168, 0.72f, WHITE, "30");
	} else if (day == 31) {
	Draw_Rect(120, 165, 50, 30, Config::barColor);
	Draw_Text(132, 168, 0.72f, WHITE, "31");
	}
}

static void getTheDays(void) {
	time_t unixTime       = time(NULL);
	struct tm* timeStruct = gmtime((const time_t*)&unixTime);
	int month = timeStruct->tm_mon + 1;

	if (month == 1) {
	draw31Days();
	} else if (month == 2) {
	draw28Days();
	} else if (month == 3) {
	draw31Days();
	} else if (month == 4) {
	draw30Days();
	} else if (month == 5) {
	draw31Days();
	} else if (month == 6) {
	draw30Days();
	} else if (month == 7) {
	draw31Days();
	} else if (month == 8) {
	draw31Days();
	} else if (month == 9) {
	draw30Days();
	} else if (month == 10) {
	draw31Days();
	} else if (month == 11) {
	draw30Days();
	} else if (month == 12) {
	draw31Days();
	}
}

void drawCalendarScreen(void) {
	Gui::DrawBGTop();
	animatedBGTop();
	Gui::DrawBarsTop();
	DisplayTime();
	drawBatteryTop();
	DisplayMonth();
	DisplayYear();
	getTheDays();
	drawCurrentDay();
	

	Gui::DrawBGBot();
	animatedBGBot();
	Gui::DrawBarsBottomBack();
}

void calendarLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		screenTransition(utilsScreen);
	} else if (hDown & KEY_TOUCH) {
		if (touching(touch, calendarButtonPos[0])) {
			screenTransition(utilsScreen);
		}
	}
}