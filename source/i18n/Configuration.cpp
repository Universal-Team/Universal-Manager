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

#include "Configuration.hpp"
#include "gui.hpp"

Configuration::Configuration()
{
        loadFromRomfs();
    }

void Configuration::loadFromRomfs()
{
    // load system language
    u8 systemLanguage;
    CFGU_GetSystemLanguage(&systemLanguage);
    switch (systemLanguage)
    {
        case CFG_LANGUAGE_JP:
            systemLanguage = Language::JP;
            break;
        case CFG_LANGUAGE_EN:
            systemLanguage = Language::EN;
            break;
        case CFG_LANGUAGE_FR:
            systemLanguage = Language::FR;
            break;
        case CFG_LANGUAGE_DE:
            systemLanguage = Language::DE;
            break;
        case CFG_LANGUAGE_IT:
            systemLanguage = Language::IT;
            break;
        case CFG_LANGUAGE_ES:
            systemLanguage = Language::ES;
            break;
        case CFG_LANGUAGE_ZH:
            systemLanguage = Language::ZH;
            break;
        case CFG_LANGUAGE_KO:
            systemLanguage = Language::KO;
            break;
        case CFG_LANGUAGE_NL:
            systemLanguage = Language::NL;
            break;
        case CFG_LANGUAGE_PT:
            systemLanguage = Language::PT;
            break;
        case CFG_LANGUAGE_RU:
            systemLanguage = Language::RU;
            break;
        case CFG_LANGUAGE_TW:
            systemLanguage = Language::TW;
            break;
        default:
            systemLanguage = Language::EN;
            break;
    }
    mJson["language"] = systemLanguage;
}
