	/*
*   This file is part of Universal-Manager-DSi
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

void drawMainMenu(void) {
	//drawRectangle(0, 0, 256, 192, BGR15(0xff, 0, 0), false);

	// Top
	drawRectangle(0, 20, 256, 152, BGR15(0xff, 0, 0), true); //BG.
	drawRectangle(0, 172, 256, 20, BGR15(0x00, 0, 0xff), true);

	// Bottom
	drawRectangle(0, 20, 256, 152, BGR15(0xff, 0, 0), false); //BG.
	drawRectangle(0, 0, 256, 20, BGR15(0x00, 0, 0xff), false);
	drawRectangle(0, 172, 256, 20, BGR15(0x00, 0, 0xff), false);

       {
        for (int x = 0; x < 256; x += 14)
        {
            for (int y = 20; y < 162; y += 14)
            {
                drawImage(x, y, stripesData.width, stripesData.height, stripes, true);
            }
        }
    }

	printTextTinted("Universal-Manager", DARK_BLUE, 60, 5, true);

	
	drawImage(217, 0, batteryChargeData.width, batteryChargeData.height, batteryCharge, true);
}
