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

// Language functions.
#ifndef UNIVERSAL_MANAGER_LANGUAGE_H
#define UNIVERSAL_MANAGER_LANGUAGE_H

#include <3ds/types.h>

// Active language ID.
extern u8 language;
// System language ID.
extern u8 sys_language;

/**
 * Initialize translations.
 * Uses the language ID specified in settings.ui.language.
 *
 * Check the language variable outside of settings to determine
 * the actual language in use.
 */
void langInit(void);

/**
 * Clear the translations cache.
 */
void langClear(void);

// String IDs.
typedef enum _StrID {
	
	/** Main Menu Stuff. **/
	STR_MAINMENU_TITLE = 0,										// "Universal-Manager"
	STR_MAINMENU_FILEMANAGER,									// "Filemanager"
	STR_MAX
} StrID;

/**
 * Get a translation.
 *
 * NOTE: Call langInit() before using TR().
 *
 * @param strID String ID.
 * @return Translation, or error string if strID is invalid.
 */
const wchar_t *TR(StrID strID);

#endif /* UNIVERSAL_MANAGER_LANGUAGE_H */
