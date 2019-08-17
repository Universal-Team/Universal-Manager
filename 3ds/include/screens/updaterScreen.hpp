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

#include "screens/screen.hpp"
#include "structs.hpp"
#include <vector>

extern "C" {
	#include "cia.h"
}

class Updater : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int Selection = 0;
	int updaterMode = 0;
	int buttonsAmount = 4;
	int titleButtons = 0;
	mutable bool updatingSelf = false;

	void DrawButtons(void) const;
	void DrawTitleButtons(void) const;
	void DrawText(void) const;
	void ButtonLogic(u32 hDown, u32 hHeld);
	void TouchLogic(u32 hDown, touchPosition touch);
	void DrawTitle(void) const;
	void DrawCurrentPage(void) const;
	void DrawSelection(void) const;

	std::vector<Structs::ButtonPos> Functions = {
		{129, 48, 87, 33, -1}, // 0
		{220, 48, 87, 33, -1}, // 1
		{129, 88, 87, 33, -1}, // 2
		{220, 88, 87, 33, -1}, // 3
		{129, 128, 87, 33, -1}, // 4
		{220, 128, 87, 33, -1}, // 5
		{38, 48, 87, 33, -1}, // 6
		{129, 48, 87, 33, -1}, // 7
		{220, 48, 87, 33, -1}, // 8
		{38, 88, 87, 33, -1}, // 9
		{293, 213, 27, 27, -1}, // 10
		{129, 168, 87, 33, -1}, // 11
		{220, 168, 87, 33, -1}, // 12

	};
};