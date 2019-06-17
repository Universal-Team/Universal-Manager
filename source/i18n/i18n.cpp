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

#include "i18n.hpp"

static LanguageStrings* jp = nullptr;
static LanguageStrings* en = nullptr;
static LanguageStrings* fr = nullptr;
static LanguageStrings* it = nullptr;
static LanguageStrings* de = nullptr;
static LanguageStrings* es = nullptr;
static LanguageStrings* ko = nullptr;
static LanguageStrings* zh = nullptr;
static LanguageStrings* tw = nullptr;
static LanguageStrings* nl = nullptr;
static LanguageStrings* pt = nullptr;
static LanguageStrings* ru = nullptr;

static const std::string emptyString                = "";
static const std::vector<std::string> emptyVector   = {};
static const std::map<u16, std::string> emptyU16Map = {};

void i18n::init(void)
{
    jp = new LanguageStrings(Language::JP);
    en = new LanguageStrings(Language::EN);
    fr = new LanguageStrings(Language::FR);
    it = new LanguageStrings(Language::IT);
    de = new LanguageStrings(Language::DE);
    es = new LanguageStrings(Language::ES);
    ko = new LanguageStrings(Language::KO);
    zh = new LanguageStrings(Language::ZH);
    tw = new LanguageStrings(Language::TW);
    nl = new LanguageStrings(Language::NL);
    pt = new LanguageStrings(Language::PT);
    ru = new LanguageStrings(Language::RU);
}

void i18n::exit(void)
{
    delete jp;
    delete en;
    delete fr;
    delete it;
    delete de;
    delete es;
    delete ko;
    delete zh;
    delete tw;
    delete nl;
    delete pt;
    delete ru;
}

const std::string& i18n::localize(Language lang, const std::string& val)
{
    switch (lang)
    {
        case Language::DE:
            return de->localize(val);
        case Language::EN:
            return en->localize(val);
        case Language::ES:
            return es->localize(val);
        case Language::FR:
            return fr->localize(val);
        case Language::IT:
            return it->localize(val);
        case Language::JP:
            return jp->localize(val);
        case Language::KO:
            return ko->localize(val);
        case Language::NL:
            return nl->localize(val);
        case Language::PT:
            return pt->localize(val);
        case Language::ZH:
            return zh->localize(val);
        case Language::TW:
            return tw->localize(val);
        case Language::RU:
            return ru->localize(val);
        default:
            return emptyString;
    }
    return emptyString;
}

 const std::string& i18n::localize(const std::string& index)
{
    return localize(Configuration::getInstance().language(), index);
}

const std::string& i18n::langString(Language l)
{
    static const std::string JPN = "JPN";
    static const std::string ENG = "ENG";
    static const std::string FRE = "FRE";
    static const std::string ITA = "ITA";
    static const std::string GER = "GER";
    static const std::string SPA = "SPA";
    static const std::string KOR = "KOR";
    static const std::string CHS = "CHS";
    static const std::string CHT = "CHT";
    switch (l)
    {
        case Language::JP:
            return JPN;
        case Language::EN:
            return ENG;
        case Language::FR:
            return FRE;
        case Language::IT:
            return ITA;
        case Language::DE:
            return GER;
        case Language::ES:
            return SPA;
        case Language::KO:
            return KOR;
        case Language::ZH:
            return CHS;
        case Language::TW:
            return CHT;
        default:
            return ENG;
    }
}

Language i18n::langFromString(const std::string& value)
{
    if (value == "JPN")
    {
        return Language::JP;
    }
    if (value == "ENG")
    {
        return Language::EN;
    }
    if (value == "FRE")
    {
        return Language::FR;
    }
    if (value == "ITA")
    {
        return Language::IT;
    }
    if (value == "GER")
    {
        return Language::DE;
    }
    if (value == "SPA")
    {
        return Language::ES;
    }
    if (value == "KOR")
    {
        return Language::KO;
    }
    if (value == "CHS")
    {
        return Language::ZH;
    }
    if (value == "CHT")
    {
        return Language::TW;
    }
    return Language::EN;
}