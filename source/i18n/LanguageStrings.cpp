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

#include "LanguageStrings.hpp"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

bool io::exists(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

std::string LanguageStrings::folder(Language lang) const
{
    switch (lang)
    {
        case Language::EN:
            return "en";
        case Language::ES:
            return "es";
        case Language::DE:
            return "de";
        case Language::FR:
            return "fr";
        case Language::IT:
            return "it";
        case Language::JP:
            return "jp";
        case Language::KO:
            return "ko";
        case Language::NL:
            return "nl";
        case Language::PT:
            return "pt";
        case Language::ZH:
            return "zh";
        case Language::TW:
            return "tw";
        default:
            return "en";
    }

    return "en";
}

LanguageStrings::LanguageStrings(Language lang)
{
    loadGui(lang);
}

void LanguageStrings::load(Language lang, const std::string name, std::vector<std::string>& array)
{
    static const std::string base = "romfs:/i18n/";
    std::string path              = io::exists(base + folder(lang) + name) ? base + folder(lang) + name : base + folder(Language::EN) + name;

    std::string tmp;
    FILE* values = fopen(path.c_str(), "rt");
    if (ferror(values))
    {
        fclose(values);
        return;
    }
    char* data  = (char*)malloc(128);
    size_t size = 0;
    while (!feof(values) && !ferror(values))
    {
        size = std::max(size, (size_t)128);
        if (__getline(&data, &size, values) >= 0)
        {
            tmp = std::string(data);
            tmp = tmp.substr(0, tmp.find('\n'));
            array.push_back(tmp.substr(0, tmp.find('\r')));
        }
        else
        {
            break;
        }
    }
    fclose(values);
    free(data);
}

void LanguageStrings::loadMap(Language lang, const std::string name, std::map<u16, std::string>& map)
{
    static const std::string base = "romfs:/i18n/";
    std::string path              = io::exists(base + folder(lang) + name) ? base + folder(lang) + name : base + folder(Language::EN) + name;

    std::string tmp;
    FILE* values = fopen(path.c_str(), "rt");
    if (ferror(values))
    {
        fclose(values);
        return;
    }
    char* data  = (char*)malloc(128);
    size_t size = 0;
    while (!feof(values) && !ferror(values))
    {
        size = std::max(size, (size_t)128);
        if (__getline(&data, &size, values) >= 0)
        {
            tmp      = std::string(data);
            tmp      = tmp.substr(0, tmp.find('\n'));
            u16 val  = std::stoi(tmp.substr(0, 4), 0, 16);
            map[val] = tmp.substr(0, tmp.find('\r')).substr(5);
        }
        else
        {
            break;
        }
    }
    fclose(values);
    free(data);
}

void LanguageStrings::loadGui(Language lang)
{
    static const std::string base = "romfs:/i18n/";
    std::string path = io::exists(base + folder(lang) + "/gui.json") ? base + folder(lang) + "/gui.json" : base + folder(Language::EN) + "/gui.json";

    FILE* values = fopen(path.c_str(), "rt");
    gui          = nlohmann::json::parse(values, nullptr, false);
    fclose(values);
}

const std::string& LanguageStrings::localize(const std::string& v) const
{
    static std::string MISSING = "MISSING: ";
    if (MISSING != "MISSING: ")
    {
        MISSING = "MISSING: ";
    }
    if (gui.count(v))
    {
        return gui.at(v).get_ref<const std::string&>();
    }
    else
    {
        MISSING += v;
        return MISSING;
    }
}
