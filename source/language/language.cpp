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

#include "language/language.h"
#include "language/textfns.h"
#include "utils/settings.hpp"

#include <malloc.h>

#include <3ds.h>

/** Language data. **/
// NOTE: These strings must be encoded as UTF-8.
#include "language/strings/japanese.h"
#include "language/strings/english.h"
#include "language/strings/french.h"
#include "language/strings/german.h"
#include "language/strings/italian.h"
#include "language/strings/spanish.h"
#include "language/strings/dutch.h"
#include "language/strings/portuguese.h"
#include "language/strings/korean.h"
#include "language/strings/russian.h"
#include "language/strings/turkish.h"
#include "language/strings/finnish.h"

// All languages.
static const char *const *lang_all[14] = {
	lang_JP,	// Japanese
	lang_EN,	// English
	lang_FR,	// French
	lang_DE,	// German
	lang_IT,	// Italian
	lang_ES,	// Spanish
	lang_EN,	// Simplified Chinese (TODO)
	lang_KO,	// Korean
	lang_NL,	// Dutch
	lang_PT,	// Portuguese
	lang_RU,	// Russian
	lang_EN,	// Traditional Chinese (TODO)
	lang_TR,	// Turkish
	lang_FI,	// Finnish
};

/** Functions. **/

// System language setting.
u8 language = 1;	// UI language ID.
u8 sys_language = 1;	// System language ID.
static const char *const *lang_data = lang_all[1];

// Translation cache.
static wchar_t *lang_cache[STR_MAX] = { };

/**
 * Initialize translations.
 * Uses the language ID specified in settings.universal.language.
 */
void langInit(void)
{
	if (R_FAILED(CFGU_GetSystemLanguage(&sys_language)) ||
	    (sys_language < 0 || sys_language >= 12))
	{
		// Invalid system language ID.
		// Default to English.
		sys_language = 1;
	}

	language = settings.universal.language;
	if (language < 0 || language >= 14) {
		// Invalid language ID.
		// Default to the system language setting.
		language = sys_language;
	}

	// Clear the language cache.
	langClear();
	// Set the selected language.
	lang_data = lang_all[language];
}

/**
 * Clear the translations cache.
 */
void langClear(void)
{
	for (int i = STR_MAX-1; i >= 0; i--) {
		free(lang_cache[i]);
		lang_cache[i] = NULL;
	}
}

/**
 * Get a translation.
 *
 * NOTE: Call langInit() before using TR().
 *
 * @param strID String ID.
 * @return Translation, or error string if strID is invalid.
 */
const wchar_t *TR(StrID strID)
{
	if (strID < 0 || strID >= STR_MAX) {
		// Invalid string ID.
		return L"STRID ERR";
	}

	if (lang_cache[strID]) {
		// String has already been converted to wchar_t*.
		return lang_cache[strID];
	}

	// Convert the string to wchar_t*.
	lang_cache[strID] = utf8_to_wchar(lang_data[strID]);
	return lang_cache[strID];
}
