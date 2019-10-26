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

#include <array>

class Calculator : public SCREEN
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	char op; // The operation like '-' '+' '*' '/'.
	float num1, num2; // The Numbers like '1' '5' or whatever.
	std::string display = "Not Implemented yet."; // Display the Values and stuff.

	// Functions.
	void drawCalculatorKeyboard(void) const;

// To-Do -> Make the positions correctly.
	std::array<Structs::Key, 19> calculatorKeys = {{
		// Numbers.
		{"1", 10, 25},
		{"2", 80, 25},
		{"3", 150, 25},
		{"4", 10, 75},
		{"5", 80, 75},
		{"6", 150, 75},
		{"7", 10, 125},
		{"8", 80, 125},
		{"9", 150, 125},
		{"0", 80, 175},
		{".", 10, 175},

		// Operations.
		{"+", 220, 25},
		{"-", 220, 75},
		{"/", 220, 125},
		{"*", 220, 175},
		{"=", 150, 175},

		// Backspace and such.
		{"E", 290, 40},
		{"B", 290, 90},
		{"C", 290, 140}
	}};
};