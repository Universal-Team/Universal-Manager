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

// Text functions.
#ifndef TEXTFNS_H
#define TEXTFNS_H

#include <3ds/types.h>
#include <string>
#include <vector>

/**
 * Convert a UTF-16 string to wstring.
 * @param str UTF-16 string.
 * @return wstring. (UTF-32)
 */
std::wstring utf16_to_wstring(const u16 *str);

/**
 * Convert a UTF-16 string to wchar_t*.
 * @param str UTF-16 string.
 * @return malloc()'d wchar_t*. (UTF-32) (NOTE: If str is nullptr, this returns nullptr.)
 */
wchar_t *utf16_to_wchar(const u16 *str);

/**
 * Convert a UTF-16 string with newlines to a vector of wstrings.
 * @param str UTF-16 string with newlines. (not necessarily NULL-terminated)
 * @param len Length of str.
 * @return vector<wstring>, split on newline boundaries.
 */
std::vector<std::wstring> utf16_nl_to_vwstring(const u16 *str, int len);

/**
 * Convert a UTF-8 string to wstring.
 * @param str UTF-8 string.
 * @return wstring. (UTF-32)
 */
std::wstring utf8_to_wstring(const char *str);

/**
 * Convert a UTF-8 string to wchar_t*.
 * @param str UTF-8 string.
 * @return malloc()'d wchar_t*. (UTF-32) (NOTE: If str is nullptr, this returns nullptr.)
 */
wchar_t *utf8_to_wchar(const char *str);

/**
 * Convert a Latin-1 string to wstring.
 * @param str Latin-1 string.
 * @return wstring. (UTF-32)
 */
std::wstring latin1_to_wstring(const char *str);

/**
 * Convert a Latin1- string to wchar_t*.
 * @param str Latin-1 string.
 * @return malloc()'d wchar_t*. (UTF-32) (NOTE: If str is nullptr, this returns nullptr.)
 */
wchar_t *latin1_to_wchar(const char *str);

#endif /* TEXTFNS_H */
