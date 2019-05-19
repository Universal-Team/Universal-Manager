/*
*   This file is part of Universal-Manager
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz
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

void drawUpdaterScreen(void) {
		drawBgTop();
		drawBarsTop();
    volt_draw_text(110, 4, 0.72f, 0.72f, WHITE, "Updater");
    volt_draw_text(1, 90, 0.72f, 0.72f, WHITE, "Press \uE002 to download NDS-Bootstrap Nightly.");
    
    // Draw the Main Bottom Screen Background.
		drawBgBot();
		drawBarsBot();

    // TWL Menu Buttons.
    volt_draw_texture(UpdaterButton, 129, 48);
    volt_draw_texture(UpdaterButton, 220, 48);
    volt_draw_text(0, 58, BIG_SIZE, BIG_SIZE, BLACK, "TWLMENU++");
    volt_draw_text(140, 58, 0.7f, 0.7f, BLACK, "Release");
    volt_draw_text(229, 58, 0.7f, 0.7f, BLACK, "Nightly");

    // NDS-Bootstrap Buttons.
    volt_draw_texture(UpdaterButton, 129, 88);
    volt_draw_texture(UpdaterButton, 220, 88);
    volt_draw_text(0, 98, 0.7f, 0.7f, BLACK, "NDS-Bootstrap");
    volt_draw_text(140, 98, 0.7f, 0.7f, BLACK, "Release");
    volt_draw_text(229, 98, 0.7f, 0.7f, BLACK, "Nightly");


    // Draw The Pages and Back Icon.
    volt_draw_text(170, 4, 0.50, 0.50, WHITE, "Current Page:");
    volt_draw_text(260, 4, 0.50, 0.50, WHITE, "1"); //Draw First Page Number.
    volt_draw_texture(BackIcon, 288, 208);
    volt_end_draw();
}