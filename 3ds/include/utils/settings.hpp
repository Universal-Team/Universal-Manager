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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

namespace Config {
	extern int barColor, bgColor, musicMode, percentDisplay, layout, layoutBG, Language; // [UI]
	extern int animation, animationColor; // [ANIMATED]
	extern int selectedText, unselectedText; // [TEXTCOLOR]
	extern int Logging; // [MISC]
	extern int Credits; // [CREDITS]
	extern int selector;
	extern int Points; // The Points in Pong's Hidden/Secret Mode.
	extern int FastMode; // Speed of the animations!

	void loadConfig();
	void saveConfig();
	void setCredits();
	void loadPoints(); // Load the current Record / Points.
	void setPongPoints(int points); // Save the new Record, if reached.
	void setLang(); // -> To-Do.
}

#endif /* SETTINGS_HPP */