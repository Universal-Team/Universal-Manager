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

/**

	This method is created by Jolty95 (umbjolt in gbatemp) to parse KeyBoard input to TWLoader.
	This code is an application of libctru swkbd using native 3DS software keyboard.
	Please, do not modify it if there is not reason to do it.
	
	Use:
		#std::string keyboardInput(const char*)  return a C++ string with the input written by the user using touch screen or buttons.
		#int keyboardInputInt(void)  return a int with the input written by the user using touch screen or buttons. If input is >255 return = 255 and if cancel, return 0

*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>

/**
 * This method show the software original keyboard from the 3ds OS. 
 * @param: const char* text to show a hint in keyboard
 * @return: std::string with user input
 */

std::string keyboardInput(const char* hint);

/**
 * This method show the software original keyboard from the 3ds OS. 
 * @param: const char* text to show a hint in keyboard
 * @return: int with user input
 */

int keyboardInputInt(const char* hint);

#endif // KEYBOARD_H
