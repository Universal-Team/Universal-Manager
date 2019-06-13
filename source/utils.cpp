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

#include "3dsutils.hpp"
#include <algorithm>
#include <map>
#include <queue>
#include <vector>

std::string StringUtils::format(const std::string& fmt_str, ...)
{
    va_list ap;
    char* fp = NULL;
    va_start(ap, fmt_str);
    vasprintf(&fp, fmt_str.c_str(), ap);
    va_end(ap);
    std::unique_ptr<char, decltype(free)*> formatted(fp, free);
    return std::string(formatted.get());
}

std::u16string StringUtils::UTF8toUTF16(const std::string& src)
{
    std::u16string ret;
    for (size_t i = 0; i < src.size(); i++)
    {
        u16 codepoint = 0xFFFD;
        int iMod      = 0;
        if (src[i] & 0x80 && src[i] & 0x40 && src[i] & 0x20 && !(src[i] & 0x10) && i + 2 < src.size())
        {
            codepoint = src[i] & 0x0F;
            codepoint = codepoint << 6 | (src[i + 1] & 0x3F);
            codepoint = codepoint << 6 | (src[i + 2] & 0x3F);
            iMod      = 2;
        }
        else if (src[i] & 0x80 && src[i] & 0x40 && !(src[i] & 0x20) && i + 1 < src.size())
        {
            codepoint = src[i] & 0x1F;
            codepoint = codepoint << 6 | (src[i + 1] & 0x3F);
            iMod      = 1;
        }
        else if (!(src[i] & 0x80))
        {
            codepoint = src[i];
        }

        ret.push_back((char16_t)codepoint);
        i += iMod;
    }
    return ret;
}

static std::string utf16DataToUtf8(const char16_t* data, size_t size, char16_t delim = 0)
{
    std::string ret;
    char addChar[4] = {0};
    for (size_t i = 0; i < size; i++)
    {
        if (data[i] == delim)
        {
            return ret;
        }
        else if (data[i] < 0x0080)
        {
            addChar[0] = data[i];
            addChar[1] = '\0';
        }
        else if (data[i] < 0x0800)
        {
            addChar[0] = 0xC0 | ((data[i] >> 6) & 0x1F);
            addChar[1] = 0x80 | (data[i] & 0x3F);
            addChar[2] = '\0';
        }
        else
        {
            addChar[0] = 0xE0 | ((data[i] >> 12) & 0x0F);
            addChar[1] = 0x80 | ((data[i] >> 6) & 0x3F);
            addChar[2] = 0x80 | (data[i] & 0x3F);
            addChar[3] = '\0';
        }
        ret.append(addChar);
    }
    return ret;
}

std::string StringUtils::UTF16toUTF8(const std::u16string& src)
{
    return utf16DataToUtf8(src.data(), src.size());
}

std::string StringUtils::getString(const u8* data, int ofs, int len, char16_t term)
{
    return utf16DataToUtf8((char16_t*)(data + ofs), len, term);
}

void StringUtils::setString(u8* data, const std::u16string& v, int ofs, int len, char16_t terminator, char16_t padding)
{
    int i = 0;
    for (; i < std::min(len - 1, (int)v.size()); i++) // len includes terminator
    {
        *(u16*)(data + ofs + i * 2) = v[i];
    }
    *(u16*)(data + ofs + i++ * 2) = terminator; // Set terminator
    for (; i < len; i++)
    {
        *(u16*)(data + ofs + i * 2) = padding; // Set final padding bytes
    }
}

void StringUtils::setString(u8* data, const std::string& v, int ofs, int len, char16_t terminator, char16_t padding)
{
    setString(data, UTF8toUTF16(v), ofs, len, terminator, padding);
    // len *= 2;
    // u8 toinsert[len] = {0};
    // if (v.empty()) return;

    // char buf;
    // int nicklen = v.length(), r = 0, w = 0, i = 0;
    // while (r < nicklen || w > len)
    // {
    //     buf = v[r++];
    //     if ((buf & 0x80) == 0)
    //     {
    //         toinsert[w] = buf & 0x7f;
    //         i = 0;
    //     }
    //     else if ((buf & 0xe0) == 0xc0)
    //     {
    //         toinsert[w] = buf & 0x1f;
    //         i = 1;
    //     }
    //     else if ((buf & 0xf0) == 0xe0)
    //     {
    //         toinsert[w] = buf & 0x0f;
    //         i = 2;
    //     }
    //     else break;

    //     for (int j = 0; j < i; j++)
    //     {
    //         buf = v[r++];
    //         if (toinsert[w] > 0x04)
    //         {
    //             toinsert[w + 1] = (toinsert[w + 1] << 6) | (((toinsert[w] & 0xfc) >> 2) & 0x3f);
    //             toinsert[w] &= 0x03;
    //         }
    //         toinsert[w] = (toinsert[w] << 6) | (buf & 0x3f);
    //     }
    //     w += 2;
    // }
    // memcpy(data + ofs, toinsert, len);
}

static std::map<u16, charWidthInfo_s*> widthCache;
static std::queue<u16> widthCacheOrder;

std::string StringUtils::splitWord(const std::string& text, float scaleX, float maxWidth)
{
    std::string word   = text;
    float currentWidth = 0.0f;
    if (StringUtils::textWidth(word, scaleX) > maxWidth)
    {
        for (size_t i = 0; i < word.size(); i++)
        {
            u16 codepoint = 0xFFFF;
            int iMod      = 0;
            if (word[i] & 0x80 && word[i] & 0x40 && word[i] & 0x20 && !(word[i] & 0x10) && i + 2 < word.size())
            {
                codepoint = word[i] & 0x0F;
                codepoint = codepoint << 6 | (word[i + 1] & 0x3F);
                codepoint = codepoint << 6 | (word[i + 2] & 0x3F);
                iMod      = 2;
            }
            else if (word[i] & 0x80 && word[i] & 0x40 && !(word[i] & 0x20) && i + 1 < word.size())
            {
                codepoint = word[i] & 0x1F;
                codepoint = codepoint << 6 | (word[i + 1] & 0x3F);
                iMod      = 1;
            }
            else if (!(word[i] & 0x80))
            {
                codepoint = word[i];
            }
            float charWidth;
            auto width = widthCache.find(codepoint);
            if (width != widthCache.end())
            {
                charWidth = width->second->charWidth * scaleX;
            }
            else
            {
                widthCache.insert_or_assign(codepoint, fontGetCharWidthInfo(fontGlyphIndexFromCodePoint(codepoint)));
                widthCacheOrder.push(codepoint);
                if (widthCache.size() > 512)
                {
                    widthCache.erase(widthCacheOrder.front());
                    widthCacheOrder.pop();
                }
                charWidth = widthCache[codepoint]->charWidth * scaleX;
            }
            currentWidth += charWidth;
            if (currentWidth > maxWidth)
            {
                word.insert(i, 1, '\n');
                currentWidth = charWidth;
            }

            i += iMod; // Yay, variable width encodings
        }
    }
    return word;
}

float StringUtils::textWidth(const std::string& text, float scaleX)
{
    float ret        = 0.0f;
    float largestRet = 0.0f;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == '\n')
        {
            largestRet = std::max(largestRet, ret);
            ret        = 0.0f;
            continue;
        }
        u16 codepoint = 0xFFFF;
        if (text[i] & 0x80 && text[i] & 0x40 && text[i] & 0x20 && !(text[i] & 0x10) && i + 2 < text.size())
        {
            codepoint = text[i] & 0x0F;
            codepoint = codepoint << 6 | (text[i + 1] & 0x3F);
            codepoint = codepoint << 6 | (text[i + 2] & 0x3F);
            i += 2;
        }
        else if (text[i] & 0x80 && text[i] & 0x40 && !(text[i] & 0x20) && i + 1 < text.size())
        {
            codepoint = text[i] & 0x1F;
            codepoint = codepoint << 6 | (text[i + 1] & 0x3F);
            i += 1;
        }
        else if (!(text[i] & 0x80))
        {
            codepoint = text[i];
        }
        float charWidth;
        auto width = widthCache.find(codepoint);
        if (width != widthCache.end())
        {
            charWidth = width->second->charWidth * scaleX;
        }
        else
        {
            widthCache.insert_or_assign(codepoint, fontGetCharWidthInfo(fontGlyphIndexFromCodePoint(codepoint)));
            widthCacheOrder.push(codepoint);
            if (widthCache.size() > 1000)
            {
                widthCache.erase(widthCacheOrder.front());
                widthCacheOrder.pop();
            }
            charWidth = widthCache[codepoint]->charWidth * scaleX;
        }
        ret += charWidth;
    }
    return std::max(largestRet, ret);
}

float StringUtils::textWidth(const std::u16string& text, float scaleX)
{
    float ret        = 0.0f;
    float largestRet = 0.0f;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (text[i] == u'\n')
        {
            largestRet = std::max(ret, largestRet);
            ret        = 0.0f;
            continue;
        }
        float charWidth;
        auto width = widthCache.find(text[i]);
        if (width != widthCache.end())
        {
            charWidth = width->second->charWidth * scaleX;
        }
        else
        {
            widthCache.insert_or_assign(text[i], fontGetCharWidthInfo(fontGlyphIndexFromCodePoint(text[i])));
            widthCacheOrder.push(text[i]);
            if (widthCache.size() > 512)
            {
                widthCache.erase(widthCacheOrder.front());
                widthCacheOrder.pop();
            }
            charWidth = widthCache[text[i]]->charWidth * scaleX;
        }
        ret += charWidth;
    }
    return std::max(largestRet, ret);
}

float StringUtils::textWidth(const C2D_Text& text, float scaleX)
{
    return ceilf(text.width * scaleX);
}

std::string StringUtils::wrap(const std::string& text, float scaleX, float maxWidth)
{
    if (textWidth(text, scaleX) <= maxWidth)
    {
        return text;
    }
    std::string dst, line, word;
    dst = line = word = "";

    for (std::string::const_iterator it = text.begin(); it != text.end(); it++)
    {
        word += *it;
        if (*it == ' ')
        {
            // split single words that are bigger than maxWidth
            if (StringUtils::textWidth(line + word, scaleX) <= maxWidth)
            {
                line += word;
            }
            else
            {
                if (StringUtils::textWidth(word, scaleX) > maxWidth)
                {
                    line += word;
                    line = StringUtils::splitWord(line, scaleX, maxWidth);
                    word = line.substr(line.find('\n') + 1, std::string::npos);
                    line = line.substr(0, line.find('\n')); // Split line on first newLine; assign second part to word and first to line
                }
                if (line[line.size() - 1] == ' ')
                {
                    dst += line.substr(0, line.size() - 1) + '\n';
                }
                else
                {
                    dst += line + '\n';
                }
                line = word;
            }
            word = "";
        }
    }

    // "Another iteration" of the loop b/c it probably won't end with a space
    // If it does, no harm done
    // word = StringUtils::splitWord(word, scaleX, maxWidth);
    if (StringUtils::textWidth(line + word, scaleX) <= maxWidth)
    {
        dst += line + word;
    }
    else
    {
        if (StringUtils::textWidth(word, scaleX) > maxWidth)
        {
            line += word;
            line = StringUtils::splitWord(line, scaleX, maxWidth);
            word = line.substr(line.find('\n') + 1, std::string::npos);
            line = line.substr(0, line.find('\n'));
        }
        if (line[line.size() - 1] == ' ')
        {
            dst += line.substr(0, line.size() - 1) + '\n' + word;
        }
        else
        {
            dst += line + '\n' + word;
        }
    }
    return dst;
}

std::string StringUtils::wrap(const std::string& text, float scaleX, float maxWidth, size_t lines)
{
    if (textWidth(text, scaleX) <= maxWidth)
    {
        return text;
    }

    // Get the wrapped string
    std::string wrapped = wrap(text, scaleX, maxWidth);
    if (lines == 0)
    {
        return wrapped;
    }

    // string.split('\n')
    std::vector<std::string> split;
    for (size_t i = 0; i < wrapped.size(); i++)
    {
        if (wrapped[i] == '\n')
        {
            split.push_back(wrapped.substr(0, i));
            wrapped = wrapped.substr(i + 1, std::string::npos);
            i       = 0;
        }
    }
    if (!wrapped.empty())
    {
        split.push_back(wrapped);
    }

    // If it's already the correct amount of lines, return it
    if (split.size() <= lines)
    {
        wrapped = split[0];
        for (size_t i = 1; i < split.size(); i++)
        {
            wrapped += '\n' + split[i];
        }
        return wrapped;
    }

    // Otherwise truncate it to the correct amount
    for (size_t i = split.size(); i > lines; i--)
    {
        split.pop_back();
    }

    const float ellipsis = fontGetCharWidthInfo(fontGlyphIndexFromCodePoint('.'))->charWidth * 3 * scaleX;

    // If there's space for the ellipsis, add it
    if (textWidth(split[lines - 1], scaleX) + ellipsis <= maxWidth)
    {
        split[lines - 1] += "...";
    }
    // Otherwise do some sort of magic
    else
    {
        std::string& finalLine = split[lines - 1];
        // If there's a long enough word and a large enough space on the top line, move stuff up & add ellipsis to the end
        if (lines > 1 && textWidth(split[lines - 2], scaleX) <= maxWidth / 2 &&
            textWidth(finalLine.substr(0, finalLine.find(' ')), scaleX) > maxWidth * 0.75f)
        {
            std::string sliced = wrap(finalLine, scaleX, maxWidth * 0.4f);
            split[lines - 2] += ' ' + sliced.substr(0, sliced.find('\n'));
            sliced = sliced.substr(sliced.find('\n') + 1);
            for (size_t i = sliced.size(); i > 0; i--)
            {
                if (sliced[i - 1] == '\n')
                {
                    sliced.erase(i - 1, 1);
                }
            }
            finalLine = sliced + "...";
        }
        // Or get rid of enough characters for it to fit
        else
        {
            for (size_t i = finalLine.size(); i > 0; i--)
            {
                if ((finalLine[i - 1] & 0x80 && finalLine[i - 1] & 0x40) || !(finalLine[i - 1] & 0x80)) // Beginning UTF-8 byte
                {
                    if (textWidth(finalLine.substr(0, i - 1), scaleX) + ellipsis <= maxWidth)
                    {
                        finalLine = finalLine.substr(0, i - 1) + "...";
                    }
                }
            }
        }
    }

    // Concatenate them and return
    wrapped = split[0];
    for (size_t i = 1; i < split.size(); i++)
    {
        wrapped += '\n' + split[i];
    }

    return wrapped;
}
