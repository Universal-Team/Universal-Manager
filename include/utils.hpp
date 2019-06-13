/*
 *   This file is part of PKSM
 *   Copyright (C) 2016-2019 Bernardo Giordano, Admiral Fish, piepie62
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

#ifndef UTILS_HPP
#define UTILS_HPP

#include <3ds.h>
#include <3ds/types.h>
#include <codecvt>
#include <locale>
#include <memory>
#include <optional>
#include <stdarg.h>
#include <string.h>
#include <string>

namespace StringUtils
{
    std::string format(const std::string& fmt_str, ...);
    std::u16string UTF8toUTF16(const std::string& src);
    std::string UTF16toUTF8(const std::u16string& src);
    std::string getString(const u8* data, int ofs, int len, char16_t term = 0);
    void setString(u8* data, const std::u16string& v, int ofs, int len, char16_t terminator = 0, char16_t padding = 0);
    void setString(u8* data, const std::string& v, int ofs, int len, char16_t terminator = 0, char16_t padding = 0);
    std::string splitWord(const std::string& word, float scaleX, float maxWidth);
    float textWidth(const std::string& str, float scaleX);
    float textWidth(const std::u16string& str, float scaleX);
    // Just wraps according to spaces
    std::string wrap(const std::string& str, float scaleX, float maxWidth);
    // Wraps and truncates to X lines, adding an ellipsis to the end
    std::string wrap(const std::string& str, float scaleX, float maxWidth, size_t lines);
}

#endif
