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

#include "battery.hpp"
#include <3ds.h>
#include <citro3d.h>
#include <stdlib.h>
#include <errno.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "ptmu_x.h"
#include "universal-Settings.hpp"


void drawBatteryTop(void) {
	u8 batteryChargeState = 0;

	u8 batteryPercent;
	mcuGetBatteryLevel(&batteryPercent);
	if(batteryPercent == 0) {
	volt_draw_texture(Battery0, 330, 15);
	} else if (batteryPercent > 0 && batteryPercent <= 15) {
	volt_draw_texture(Battery15, 330, 5);
	} else if(batteryPercent > 15 && batteryPercent <= 28) {
	volt_draw_texture(Battery28, 330, 5);
	} else if(batteryPercent > 28 && batteryPercent <= 43) {
    volt_draw_texture(Battery43, 330, 5);
	} else if(batteryPercent > 43 && batteryPercent <= 57) {
	volt_draw_texture(Battery57, 330, 5);
	} else if(batteryPercent > 57 && batteryPercent <= 71) {
	volt_draw_texture(Battery71, 330, 5);
	} else if(batteryPercent > 71 && batteryPercent <= 99) {
	volt_draw_texture(Battery85, 330, 5);
	} else if(batteryPercent == 100) {
	volt_draw_texture(Battery100, 330, 5);
	}
	
	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryChargeState)) && batteryChargeState) {
		volt_draw_texture(BatteryCharge, 330, 5);
	}
	if (settings.universal.battery == 0) {
	if(batteryPercent == 100) {
		volt_draw_textf(348, 4, 0.7f, 0.7f, WHITE, "100%%");
	} else {
		volt_draw_textf(348, 4, 0.7f, 0.7f, WHITE, "%2d%%", batteryPercent);
}
} else if (settings.universal.battery == 1) {
}
}

void drawBatteryBot(void) {
	u8 batteryChargeState = 0;

	u8 batteryPercent;
	mcuGetBatteryLevel(&batteryPercent);

	if(batteryPercent == 0) {
		volt_draw_texture(Battery0, 250, 15);
	} else if (batteryPercent > 0 && batteryPercent <= 15) {
	volt_draw_texture(Battery15, 250, 5);
	} else if(batteryPercent > 15 && batteryPercent <= 28) {
	volt_draw_texture(Battery28, 250, 5);
	} else if(batteryPercent > 28 && batteryPercent <= 43) {
    volt_draw_texture(Battery43, 250, 5);
	} else if(batteryPercent > 43 && batteryPercent <= 57) {
	volt_draw_texture(Battery57, 250, 5);
	} else if(batteryPercent > 57 && batteryPercent <= 71) {
	volt_draw_texture(Battery71, 250, 5);
	} else if(batteryPercent > 71 && batteryPercent <= 99) {
	volt_draw_texture(Battery85, 250, 5);
	} else if(batteryPercent == 100) {
	volt_draw_texture(Battery100, 250, 5);
	}
	
	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryChargeState)) && batteryChargeState) {
		volt_draw_texture(BatteryCharge, 250, 5);
	}
	if (settings.universal.battery == 0) {
	if(batteryPercent == 100) {
		volt_draw_textf(268, 4, 0.7f, 0.7f, WHITE, "100%%");
	} else {
		volt_draw_textf(268, 4, 0.7f, 0.7f, WHITE, "%2d%%", batteryPercent);
}
} else if (settings.universal.battery == 1) {
}
}